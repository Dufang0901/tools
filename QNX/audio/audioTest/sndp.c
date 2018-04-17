/******************************************************************************/
/*@F_NAME:           sndp.c                                                */
/*@F_PURPOSE:        source file for Audio application API                   */
/*@F_CREATED_BY:     Jun Ma                                    */
/*@F_CREATION_DATE:   2017-3-17                                          */
/*@F_MPROC_TYPE:     up independent                                           */
/************************************** (C) Copyright 2011 Magneti Marelli ****/

#include <errno.h>
#include <fcntl.h>
#include <gulliver.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/termio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/slogcodes.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <pthread.h>
#include <sys/pps.h>
#include <sys/asoundlib.h>
#include <audio/audio_manager_routing.h>

#include <semaphore.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/mman.h>
#include "sndp.h"
#include "sndp_priv.h"

/************************LOCAL DEFINES***************************/
/*config file nodes description*/
#define   SNDP_CFG_ROOT_NODE  			"root"
#define   SNDP_CFG_VERSION_NODE  		"version"
#define   SNDP_CFG_DESCRIPTION_NODE  	"description"
#define   SNDP_CFG_AUDIONODES_NODE   	"audioNodes"
#define   SNDP_CFG_AUDIONODE_NODE   	"audioNode"
#define   SNDP_CFG_IDNODE_NODE   		"id"
#define   SNDP_CFG_NAMENODE_NODE   		"name"
#define   SNDP_AUDIOROMPATHPREFIX   			"/appfs/audio/"
#define   SNDP_AUDIO_CONFIG_ROMFILE  			"/appfs/audio/audioCfg.xml"
#define   SNDP_AUDIOROMPATHFILES         "/appfs/audio/*"
#define   SNDP_AUDIOPATHPREFIX        "/tmp/audio/"
#define   SNDP_AUDIO_CONFIG_FILE        "/tmp/audio/audioCfg.xml"
#define   SNDP_PCM_SAMPLE_BYTES     			(4096*4)
#define   SNDP_STATU_PLAYING 2
#define   SNDP_STATU_NORMAL  1
#define   SNDP_STATU_ERROR   0

#define SYST_DEBUG(Condition, Action) if(Condition) {Action;}
#define SYST_Printf(Level,fmt, ...) {slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), Level | _SLOG_TEXTBIT, fmt,  ##__VA_ARGS__);}



#define   SNDP_NONBLOCK_USED    FALSE
/*#define SNDP_DEBUG*/

typedef enum
{
  Sndp_OK,
  Sndp_ERROR_INPUT,
  Sndp_ERROR_AUDIO
}Sndp_RetValue_t;

typedef struct
{
  int PcmFd;
  int Card;
  int Dev;
}Sndp_PcmHandle_t;

typedef struct
{
  int MixerFd;
}Sndp_SndMixer_t;

typedef struct
{
  ubyte tag[4];
  int length;
} Sndp_RiffTag_t;

typedef struct
{
  ubyte Riff[4];
  int Size;
  ubyte Wave[4];
} Sndp_RiffHdr_t;

typedef struct
{
  FILE *file1;
  struct timespec start_time;
} Sndp_WriterData;

typedef struct
{
  sshort FormatTag;
  sshort Channels;
  int SamplesPerSec;
  int AvgBytesPerSec;
  sshort BlockAlign;
  sshort BitsPerSample;
} Sndp_WaveHdr_t;
typedef struct
{
  FILE *Fp;
  char FileName[128];
  int FormatTag;
  int Format;
  int Channels;
  int SamplesPerSec;
  int AvgBytesPerSec;
  int BitsPerSample;
  int DataSize;
  int BlockSize;
  int PlayedSize;
  int DoneFlag;
}Sndp_AudioInfo_t;

typedef struct
{
  SNDP_STATUS Status;
  unsigned int AudioId;
  float Volume;
}Sndp_StatusInfo_t;

/***************************************Module data***********************************/
static char *Sndp_AudioXmlBuffer = NULL;
static int SndpAudioXmlBufferSize = 0;
static float SndpVolumLevel = 40;
static int SndpCurrentMusicID = 0;
static int SndpCurrentTimes = 0;

static bool_t Sndp_TaskRunningFlag = FALSE;
static sem_t Sndp_Sem = { 0 };
static char SndpFullAudioPath[512] = { 0 };
static const char *SndpkRiffId = "RIFF";
static const char *SndpkRifxId = "RIFX";
static const char *SndpkWaveId = "WAVE";
static bool SndpRunning = true;
static int Sndp_n = 0;
static int Sndp_N = 0;
static int SndpPrint_timing = 0;
static int SndpBsize = 0;
static snd_mixer_group_t SndpGroup = { 0 };
static snd_mixer_t *SndpMixer_handle = NULL;
static snd_pcm_t *SndpPcm_handle = NULL;
static unsigned int SndpMSamples = 0;
static bool SndpmBigEndian = false;
static char SndpmSampleBfr1[SNDP_PCM_SAMPLE_BYTES] = { 0 };
static Sndp_StatusInfo_t Sndp_Status = { 0 };
static Sndp_PcmHandle_t Snd_PcmHandle;
static Sndp_SndMixer_t Snd_SndMixer;
int SndpCurrentStatus = SNDP_IDLE;

/**********************************APIS**********************************************/
int SNDP_FeedWatchDogReq()
{
  return 0;
}
/******************************************************************************/
/*Name : SndpErr                                                 */
/*Role :                         */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                      */
/*OD                                                                          */
/******************************************************************************/
static int SndpErr(ubyte *msg)
{
  if(NULL != msg)
  {
    perror(msg);
  }
  return -1;
}
/******************************************************************************/
/*Name : SndpFindTag                                                 */
/*Role :                         */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                      */
/*OD                                                                          */
/******************************************************************************/
static int SndpFindTag(FILE * fp, const ubyte *tag)
{
  int retVal = 0;
  Sndp_RiffTag_t tagBfr = { "", 0 };

  if((NULL != fp) && (NULL != tag))
  {
    /* Keep reading until we find the tag or hit the EOF.*/
    while (0 < fread((ubyte *) &tagBfr, sizeof(tagBfr), 1, fp))
    {
      /* If this is our tag, set the length and break.*/
      if(0 == strncmp(tag, tagBfr.tag, sizeof tagBfr.tag))
      {
        retVal = ENDIAN_LE32(tagBfr.length);
        printf("########## length is %d retVal is %d\n",tagBfr.length,retVal);
        break;
      }
      /* Skip ahead the specified number of bytes in the stream*/
      fseek(fp, tagBfr.length, SEEK_CUR);
    }
  }
  else
  {
    retVal = -1;
  }
  /* Return the result of our operation*/
  return (retVal);
}
/******************************************************************************/
/*Name : SndpCheckHdr                                                 */
/*Role :                         */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                      */
/*OD                                                                          */
/******************************************************************************/
static int SndpCheckHdr(FILE * fp)
{
  Sndp_RiffHdr_t riffHdr = { 0 };
  int ret = 0;

  if(NULL != fp)
  {
    /* Read the header and, if successful, play the file
     file or WAVE file.*/
    fseek(fp, 0, SEEK_SET);
    memset(&riffHdr, 0, sizeof(riffHdr));
    if(fread((ubyte *) &riffHdr, sizeof(Sndp_RiffHdr_t), 1, fp) <= 0)
    {
      ret = -1;
    }
    else
    {
      if(strncmp(riffHdr.Riff, SndpkRiffId, strlen(SndpkRiffId))
          || strncmp(riffHdr.Wave, SndpkWaveId, strlen(SndpkWaveId)))
      {
        ret = -1;
      }
    }
  }
  else
  {
    ret = -1;/*error do nothing */
  }
  
  return ret;
}
/******************************************************************************/
/*Name : SndpDump_info                                                   */
/*Role :dump info                       */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*                                        */
/*OD                                                                          */
/******************************************************************************/
static void SndpDump_info(int card, int dev, const char* name, int channel)
{
  int rtn = 0;
  snd_pcm_t * SndpPcm_handle = 0;
  snd_pcm_channel_info_t pi = { 0 };
  int open_mode =
      (channel == SND_PCM_CHANNEL_PLAYBACK) ?
          SND_PCM_OPEN_PLAYBACK : SND_PCM_OPEN_CAPTURE;

  if(NULL != name)
  {
    if(name[0] != '\0')
    {
      rtn = snd_pcm_open_name(&SndpPcm_handle, name, open_mode);
    }
    else if(card == -1)
    {
      rtn = snd_pcm_open_preferred(&SndpPcm_handle, NULL, NULL, open_mode);
    }
    else
    {
      rtn = snd_pcm_open(&SndpPcm_handle, card, dev, open_mode);
    }
    if(rtn < 0)
    {
      SYST_DEBUG(TRUE,
          SYST_Printf(_SLOG_WARNING,"Cannot open %s device \n",channel == SND_PCM_CHANNEL_PLAYBACK?"playback":"capture"));
    }
    else
    {
      memset(&pi, 0, sizeof(pi));
      pi.channel = channel;
      if((rtn = snd_pcm_channel_info(SndpPcm_handle, &pi)) == 0)
      {
#ifdef SNDP_DEBUG
        SYST_DEBUG(TRUE,SYST_Printf_Std("\n%s %s Info\n", pi.subname, channel == SND_PCM_CHANNEL_PLAYBACK?"Playback":"Capture"));
        SYST_DEBUG(TRUE,SYST_Printf_Std("flags 0x%X\n", pi.flags));
        SYST_DEBUG(TRUE,SYST_Printf_Std("formats 0x%X\n", pi.formats);
        SYST_DEBUG(TRUE,SYST_Printf_Std("fragment_align %d\n", pi.fragment_align));
        SYST_DEBUG(TRUE,SYST_Printf_Std("max_buffer_size %d\n", pi.max_buffer_size));
        SYST_DEBUG(TRUE,SYST_Printf_Std("max_fragment_size %d\n", pi.max_fragment_size));
        SYST_DEBUG(TRUE,SYST_Printf_Std("min_fragment_size %d\n", pi.min_fragment_size));
        SYST_DEBUG(TRUE,SYST_Printf_Std("rates 0x%X\n", pi.rates));
        SYST_DEBUG(TRUE,SYST_Printf_Std("max_rate %d\n", pi.max_rate));
        SYST_DEBUG(TRUE,SYST_Printf_Std("min_rate %d\n", pi.min_rate));
        SYST_DEBUG(TRUE,SYST_Printf_Std("max_voices %d\n", pi.max_voices));
        SYST_DEBUG(TRUE,SYST_Printf_Std("min_voices %d\n", pi.min_voices));
        SYST_DEBUG(TRUE,SYST_Printf_Std("subdevice %d\n", pi.subdevice));
        SYST_DEBUG(TRUE,SYST_Printf_Std("subname %s\n", pi.subname == NULL ? "NULL" : pi.subname));
        SYST_DEBUG(TRUE,SYST_Printf_Std("transfer_block_size %d\n", pi.transfer_block_size));
#endif
      }
      else
      {
        SYST_DEBUG(TRUE,
            SYST_Printf(_SLOG_WARNING,"Error retrieving %s info\n",channel == SND_PCM_CHANNEL_PLAYBACK?"playback":"capture"));
      }
      snd_pcm_close(SndpPcm_handle);
    }
  }
  return;
}
/******************************************************************************/
/*Name : Sndp_HandleXMLNode                                                   */
/*Role :handle audio node                         */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*                                        */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_HandleXMLNode(xmlNodePtr itemNodePtr, int ID, char *fileName,
    char maxSize)
{
  int ret = -1;
  xmlChar *key = NULL;
  xmlNodePtr itemNodePtr2 = NULL;
  char audioID = -1;

  if(fileName != NULL )
  {
    while (itemNodePtr != NULL )
    {
      if((!xmlStrcmp(itemNodePtr->name,
          (const xmlChar *) SNDP_CFG_AUDIONODE_NODE)))
      {
        itemNodePtr2 = itemNodePtr->xmlChildrenNode;
        audioID = -1;
        while (itemNodePtr2 != NULL )/*search the file nodes*/
        {
          if((!xmlStrcmp(itemNodePtr2->name,
              (const xmlChar *) SNDP_CFG_IDNODE_NODE)))
          {
            key = xmlNodeGetContent(itemNodePtr2);
            if(NULL != key)
            {
              audioID = atoi(key);
              xmlFree(key);
              if(audioID != ID)
              {
                break; /*Not match ,next node*/
              }
            }
            else
            {
              break;/*Not match ,next node*/
            }
          }
          if((!xmlStrcmp(itemNodePtr2->name,
              (const xmlChar *) SNDP_CFG_NAMENODE_NODE)))
          {
            key = xmlNodeGetContent(itemNodePtr2);
            if(NULL != key)
            {
              if(strlen(key) < maxSize)
              {
                strncpy(fileName, key, strlen(key) + 1);
                xmlFree(key);
                ret = 0;
                break;
              }
              xmlFree(key);
            }
          }
          itemNodePtr2 = itemNodePtr2->next;
        }
      }
      if(-1 == ret)
      {
        itemNodePtr = itemNodePtr->next;
      }
      else
      {
        break;
      }
    }
  }
  return ret;
}

/******************************************************************************/
/*Name : Sndp_GetAudioFileNameByIDInXMLCFG                                                   */
/*Role :get audio file name by ID from XML config                          */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*                                        */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_GetAudioFileNameByIDInXMLCFG(char *xmlFile, int ID,
    char *fileName, char maxSize)
{
  int ret = -1;
  xmlDocPtr doc = NULL;
  xmlNodePtr cur = NULL;
  xmlChar *key = NULL;
  xmlNodePtr propNodePtr = cur;
  xmlNodePtr itemNodePtr = NULL;

  if((NULL != xmlFile) && (NULL != fileName))
  {
    /*parse the xml*/
    doc = xmlParseMemory(xmlFile, strlen(xmlFile));
    if(NULL != doc)
    {
      cur = xmlDocGetRootElement(doc); /*get the root node*/
      if(cur != NULL )
      {
        if(0 == xmlStrcmp(cur->name, (const xmlChar *) SNDP_CFG_ROOT_NODE))
        {
          cur = cur->xmlChildrenNode;
          while (cur != NULL )
          {
            if((!xmlStrcmp(cur->name, (const xmlChar *) SNDP_CFG_VERSION_NODE)))
            {
              key = xmlNodeGetContent(cur);
              xmlFree(key);
            }
            if((!xmlStrcmp(cur->name,
                (const xmlChar *) SNDP_CFG_DESCRIPTION_NODE)))
            {
              key = xmlNodeGetContent(cur);
              xmlFree(key);
            }
            if((!xmlStrcmp(cur->name,
                (const xmlChar *) SNDP_CFG_AUDIONODES_NODE)))
            {
              itemNodePtr = cur->xmlChildrenNode;
              ret = Sndp_HandleXMLNode(itemNodePtr, ID, fileName, maxSize);
              if(0 == ret)
              {
                break;
              }
            }
            cur = cur->next;
          }
        }
        else
        {
          SYST_DEBUG(TRUE,
              SYST_Printf(_SLOG_WARNING,"document of the wrong type, root node != root"));
        }
      }
      else
      {
      }
      xmlFreeDoc(doc);
    }
  }
  return ret;
}

/******************************************************************************/
/*Name : Sndp_InitAudioChanal                                                   */
/*Role :initialize the audio channel                            */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [initialize the audio channel]                                      */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_InitAudioChanal(void)
{
  int ret = -1;
  int fd = 0;

  static char cmdBuf[128] = {0};

  SndpCurrentStatus = SNDP_IDLE;
  SndpVolumLevel = 60;

  /*SNDP access file in EMMC may be blocked,
   * so copy Audio files from emmc to ram, */
  snprintf(cmdBuf,sizeof(cmdBuf),"mkdir %s ", SNDP_AUDIOPATHPREFIX);
  system(cmdBuf);

  snprintf(cmdBuf,sizeof(cmdBuf),"cp %s %s", SNDP_AUDIOROMPATHFILES,
            SNDP_AUDIOPATHPREFIX);
  system(cmdBuf);

  fd = open(SNDP_AUDIO_CONFIG_FILE, O_RDONLY);
  if(-1 != fd)
  {
    lseek(fd, 0L, SEEK_SET);
    SndpAudioXmlBufferSize = lseek(fd, 0L, SEEK_END);
    if(SndpAudioXmlBufferSize > 0)
    {
      Sndp_AudioXmlBuffer = mmap(0, SndpAudioXmlBufferSize, PROT_READ,
          MAP_PRIVATE, fd, 0);
      if(0
          <= snprintf(SndpFullAudioPath, sizeof(SndpFullAudioPath), "%s",
              SNDP_AUDIOPATHPREFIX))
      {
        ret = 0;
      }
    }
    close(fd);
  }
  return ret;
}
/******************************************************************************/
/*Name : Sndp_handle_mixer                                                   */
/*Role :handle mixer                               */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [play the audio by music ID]                                      */
/*OD                                                                          */
/******************************************************************************/
static void Sndp_handle_mixer(void)
{
  fd_set rfds = { 0 };
  int mixer_fd = snd_mixer_file_descriptor(SndpMixer_handle);

  FD_ZERO(&rfds);
  FD_SET(mixer_fd, &rfds);
  if(select(mixer_fd + 1, &rfds, NULL, NULL, NULL ) == -1)
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,"select fails \n"));
  }
  snd_mixer_callbacks_t callbacks = { 0, 0, 0, 0 };
  snd_mixer_read(SndpMixer_handle, &callbacks);
}

/******************************************************************************/
/*Name : Sndp_write_audio_data                                                   */
/*Role :write aduio data                              */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [play the audio by music ID]                                      */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_write_audio_data(Sndp_AudioInfo_t *AudioInfo)
{
  struct timespec current_time = { 0 };
  snd_pcm_channel_status_t status = { 0 };
  int written = 0;
  int rdSize = 0;
  int ret = 0;

  if(NULL != AudioInfo)
  {
    if((rdSize = fread(SndpmSampleBfr1, 1,
        min(AudioInfo->DataSize - AudioInfo->PlayedSize, AudioInfo->BlockSize), 
        AudioInfo->Fp)) > 0)
    {
      written = snd_pcm_plugin_write(SndpPcm_handle, SndpmSampleBfr1, rdSize);
      if(written < rdSize)
      {
        memset(&status, 0, sizeof(status));
        status.channel = SND_PCM_CHANNEL_PLAYBACK;
        if(snd_pcm_plugin_status(SndpPcm_handle, &status) >= 0)
        {
          switch (status.status)
          {
            case SND_PCM_STATUS_CHANGE:
            {
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_write_audio_data: Audio device change occurred from %s to %s\n", 
                audio_manager_get_device_name(status.status_data.change_data.old_device), 
                audio_manager_get_device_name(status.status_data.change_data.new_device)));

              if(snd_pcm_plugin_prepare(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0)
              {
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                  "Sndp_write_audio_data: underrun: playback channel prepare error\n"));
                ret = -1;
              }

            }
            break;
            case SND_PCM_STATUS_UNDERRUN:
            {
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_write_audio_data: Audio underrun occurred \n"));
              
              if(snd_pcm_plugin_prepare(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0)
              {
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                  "Sndp_write_audio_data: underrun: playback channel prepare error\n"));
                ret = -1;
              }

            }
            break;
            case SND_PCM_STATUS_READY:
            {
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_write_audio_data: Audio status ready \n"));

              if(snd_pcm_plugin_prepare(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0)
              {
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                  "Sndp_write_audio_data: playback channel prepare error\n"));
                ret = -1;
              }
            }
            break;
            case SND_PCM_STATUS_UNSECURE:
            {
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_write_audio_data: Channel unsecure\n"));
              
              if(snd_pcm_plugin_prepare(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0)
              {
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                  "Sndp_write_audio_data: unsecure, playback channel prepare error\n"));
                ret = -1;
              }
            }
            break;
            case SND_PCM_STATUS_ERROR:
            {

              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_write_audio_data: error, playback channel failure\n"));
              ret = -1;
            }
              break;
            case SND_PCM_STATUS_PREEMPTED:
            {
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_write_audio_data: error, playback channel preempted\n"));
              ret = -1;
            }
              break;
            default:
              break;
          }
          if(0 == ret)
          {
            if(written < 0)
            {
              written = 0;
            }
            written += snd_pcm_plugin_write(SndpPcm_handle,
                SndpmSampleBfr1 + written, rdSize - written);
          }
          else
          {
            ;/*error do nothing*/
          }
        }
        else
        {
          SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
            "Sndp_write_audio_data: snd_pcm_plugin_status failed.\n"));
          ret = -1;
        }
      }
      AudioInfo->PlayedSize += written;
    }
    else
    {
      ret = -1;
    }
  }
  else
  {
    ret = -1;
  }
  return ret;
}

/******************************************************************************/
/*Name : Sndp_AudioRelease                                                    */
/*Role : release handles                                                      */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [release handles]                                                      */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_AudioRelease(Sndp_AudioInfo_t *AudioInfo)
{
  snd_mixer_close(SndpMixer_handle);
  snd_pcm_close(SndpPcm_handle);
  fclose(AudioInfo->Fp);
  AudioInfo->Fp = NULL;

}
/******************************************************************************/
/*Name : Sndp_InitSndMixer                                                    */
/*Role : Init snd mixer                                                       */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [init snd  mixer]                                                      */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_InitSndMixer(Sndp_AudioInfo_t *AudioInfo)
{
  snd_pcm_channel_setup_t setup = { 0 };
  int volume = 0;
  int rtn = -1;
  int ret = -1;

  /*get block size and data size*/
  memset(&setup, 0, sizeof(setup));
  memset(&SndpGroup, 0, sizeof(SndpGroup));
  setup.channel = SND_PCM_CHANNEL_PLAYBACK;
  setup.mixer_gid = &SndpGroup.gid;
  rtn = snd_pcm_plugin_setup(SndpPcm_handle, &setup);
  if (EOK == rtn)
  {
    AudioInfo->BlockSize = setup.buf.block.frag_size;
    printf("@@@@@@@@ %s:%d AudioInfo->BlockSize = %d\n",__func__,__LINE__,AudioInfo->BlockSize);

    if ('0' == SndpGroup.gid.name[0])
    {
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
          "Sndp_InitSndMixer:Mixer Pcm Group [%s] Not Set \n", SndpGroup.gid.name));
    }
    else
    {
      rtn = snd_mixer_open(&SndpMixer_handle, Snd_PcmHandle.Card, setup.mixer_device);
      if (0 == rtn)
      {
        ret = 0;
        rtn = snd_mixer_file_descriptor(SndpMixer_handle);
        if(rtn > 0)
        {
          Snd_SndMixer.MixerFd = rtn;
          /*set volume*/
          if (Sndp_Status.Volume >= 0)
          {
             rtn = snd_mixer_group_read(SndpMixer_handle, &SndpGroup);
            if (0 == rtn)
            {
              volume = (float) (SndpGroup.max - SndpGroup.min)
                       * (SndpVolumLevel / 100);

              if(SndpGroup.channels & SND_MIXER_CHN_MASK_FRONT_LEFT)
              {
                SndpGroup.volume.names.front_left = (int) volume;
              }
              if(SndpGroup.channels & SND_MIXER_CHN_MASK_REAR_LEFT)
              {
                SndpGroup.volume.names.rear_left = (int) volume;
              }
              if(SndpGroup.channels & SND_MIXER_CHN_MASK_FRONT_CENTER)
              {
                SndpGroup.volume.names.front_center = (int) volume;
              }
              if(SndpGroup.channels & SND_MIXER_CHN_MASK_FRONT_RIGHT)
              {
                SndpGroup.volume.names.front_right = (int) volume;
              }
              if(SndpGroup.channels & SND_MIXER_CHN_MASK_REAR_RIGHT)
              {
                SndpGroup.volume.names.rear_right = (int) volume;
              }
              if(SndpGroup.channels & SND_MIXER_CHN_MASK_WOOFER)
              {
                SndpGroup.volume.names.woofer = (int) volume;
              }

              rtn = snd_mixer_group_write(SndpMixer_handle, &SndpGroup);
              if (0 != rtn)
              {
                ret = -1;
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                  "Sndp_InitSndMixer: snd_mixer_group_write failed: %s\n", snd_strerror (rtn)));
              }
            }
            else
            {
              ret = -1;
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Sndp_InitSndMixer: snd_mixer_group_read failed: %s\n", snd_strerror (rtn)));
            }
          }
        }
        else
        {
          SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
            "Sndp_InitSndMixer: snd_mixer_file_descriptor failed: %s\n", snd_strerror (rtn)));
        }
      }
      else
      {
        SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
          "Sndp_InitSndMixer: snd_mixer_open failed: %s\n", snd_strerror (rtn)));
      }
    }
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitSndMixer: snd_pcm_plugin_setup failed: %s\n", snd_strerror (rtn)));
  }
}
/******************************************************************************/
/*Name : Sndp_InitPcmHandle                                                   */
/*Role : InitPcm Handle                                                       */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [init pcm handle]                                                      */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_InitPcmHandle(Sndp_AudioInfo_t *AudioInfo)
{
  int mode = SND_PCM_OPEN_PLAYBACK;
  const char *type = "multimedia";
  unsigned int audioman_handle = 0;
  snd_pcm_channel_info_t pi = { 0 };
  snd_pcm_channel_params_t pp = { 0 };
  int fragsize = -1;
  int num_frags = -1;
  int num_buffered_frags = 0;
  int rate_method = 0;
  int rtn = 0;
  int ret = 0;


  if (AudioInfo != NULL)
  {
    /*get pmc handle*/
    rtn = snd_pcm_open_preferred(&SndpPcm_handle, &Snd_PcmHandle.Card, &Snd_PcmHandle.Dev, mode);
    if (0 == rtn)
    {
      rtn = audio_manager_get_handle(audio_manager_get_type_from_name(type), 0,
          true, &audioman_handle);
      if (EOK == rtn)
      {
        rtn = snd_pcm_set_audioman_handle(SndpPcm_handle, audioman_handle);
        if (EOK != rtn)
        {
          ret = -1;
          SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
            "Sndp_InitPcmHandle: snd_pcm_set_audioman_handle failed, %s\n", snd_strerror(rtn)));
        }
      }
      if(0 == ret)
      {
          ret = -1;
          /*set pcm handle parameters*/
          snd_pcm_plugin_set_disable(SndpPcm_handle, PLUGIN_DISABLE_MMAP);
          snd_pcm_plugin_set_enable(SndpPcm_handle, PLUGIN_ROUTING);
          memset(&pi, 0, sizeof(pi));
          pi.channel = SND_PCM_CHANNEL_PLAYBACK;

          rtn = snd_pcm_plugin_info(SndpPcm_handle, &pi);
          if (EOK == rtn)
          {
            memset(&pp, 0, sizeof(pp));
            pp.mode = SND_PCM_MODE_BLOCK;
            pp.channel = SND_PCM_CHANNEL_PLAYBACK;
            pp.start_mode = SND_PCM_START_FULL;
            pp.stop_mode = SND_PCM_STOP_STOP;

            if (fragsize != -1)
            {
              pp.buf.block.frag_size = fragsize;
            }
            else
            {
              pp.buf.block.frag_size = pi.max_fragment_size;
            }
            pp.buf.block.frags_max = num_frags;
            pp.buf.block.frags_buffered_max = num_buffered_frags;
            pp.buf.block.frags_min = 1;
            pp.format.interleave = 1;
            pp.format.rate = AudioInfo->SamplesPerSec;
            pp.format.voices = AudioInfo->Channels;
            pp.format.format = AudioInfo->Format;
            strcpy(pp.sw_mixer_subchn_name, "Wave playback channel");

            rtn = snd_pcm_plugin_set_src_method(SndpPcm_handle, rate_method);
            if (rtn != rate_method)
            {
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
                "Failed to apply rate_method %d, using %d\n", rate_method, rtn));
            }

            rtn = snd_pcm_plugin_params(SndpPcm_handle, &pp);
            if (EOK == rtn)
            {
              /*prepare pcm handle*/
              rtn = snd_pcm_plugin_prepare(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK);
              if (EOK == rtn)
              {
                rtn = snd_pcm_file_descriptor(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK);
                if(rtn > 0)
                {
                  /*initialization successful*/
                  Snd_PcmHandle.PcmFd = rtn;
                  ret = 0;
                }
                else
                {
                  SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
                     "snd_pcm_file_descriptor failed: %s\n", snd_strerror (rtn)));
                }
              }
              else
              {
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
                  "snd_pcm_plugin_prepare failed: %s\n", snd_strerror (rtn)));
              }

            }
            else
            {
              SYST_DEBUG(TRUE,  SYST_Printf(_SLOG_ERROR,
                "Sndp_InitPcmHandle: snd_pcm_plugin_params error, return %d.\n",rtn));
            }
          }
          else
          {
            SYST_DEBUG(TRUE,  SYST_Printf(_SLOG_ERROR,
              "Sndp_InitPcmHandle: snd_pcm_plugin_info error, return %d.\n",rtn));
          }
       
      }
    }
    else
    {
      SYST_DEBUG(TRUE,  SYST_Printf(_SLOG_ERROR,
        "Sndp_InitPcmHandle: snd_pcm_open_preferred error, return %d.\n",rtn));
    }
  }
  else
  {
    SYST_DEBUG(TRUE,
      SYST_Printf(_SLOG_WARNING,"Sndp_InitPcmHandle: invalid parameters.\n"));
  }

  return ret;
}
/******************************************************************************/
/*Name : Sndp_GetAudioParam                                                   */
/*Role :get audio parameters from file                                        */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [get audio parameters from file]                                       */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_GetAudioParam(Sndp_AudioInfo_t *AudioInfo)
{
  Sndp_WaveHdr_t wavHdr1 = { 0 };
  int BlockSize = 0;
  int ret = -1;

  printf("@@@@@@@@@@@ sizeof Riff header is %d\n",sizeof(Sndp_RiffHdr_t));
  
  printf("@@@@@@@@@@@ sizeof fmt header is %d\n",sizeof(Sndp_WaveHdr_t));

  if ((AudioInfo != NULL) &&(strlen(AudioInfo->FileName) > 0))
  {
    AudioInfo->Fp = fopen(AudioInfo->FileName, "r");
    if(AudioInfo->Fp != NULL )
    {
      /*3 get wave parameters*/
      if(SndpCheckHdr(AudioInfo->Fp) != -1)
      {
        BlockSize = SndpFindTag(AudioInfo->Fp, "fmt ");
        if(BlockSize > 0)
        {
          printf("@@@@@@@@ %s:%d BlockSize = %d\n",__func__,__LINE__,BlockSize);
          fread(&wavHdr1, sizeof(wavHdr1), 1, AudioInfo->Fp);
          fseek(AudioInfo->Fp, (BlockSize - sizeof(Sndp_WaveHdr_t)), SEEK_CUR);
          if(SndpmBigEndian)
          {
            AudioInfo->SamplesPerSec = ENDIAN_BE32(wavHdr1.SamplesPerSec);
            AudioInfo->Channels = ENDIAN_BE16(wavHdr1.Channels);
            AudioInfo->BitsPerSample = ENDIAN_BE16(wavHdr1.BitsPerSample);
            AudioInfo->FormatTag = ENDIAN_BE16(wavHdr1.FormatTag);
          }
          else
          {
            AudioInfo->SamplesPerSec = ENDIAN_LE32(wavHdr1.SamplesPerSec);
            AudioInfo->Channels = ENDIAN_LE16(wavHdr1.Channels);
            AudioInfo->BitsPerSample = ENDIAN_LE16(wavHdr1.BitsPerSample);
            AudioInfo->FormatTag = ENDIAN_LE16(wavHdr1.FormatTag);
          }

          /*4 get data block size*/
          BlockSize = SndpFindTag(AudioInfo->Fp, "data");
          if(BlockSize > 0)
          {
            ret = 0;
            AudioInfo->DataSize = BlockSize;

            /*5 get format*/
            if(6 == AudioInfo->FormatTag)
            {
              AudioInfo->Format = SND_PCM_SFMT_A_LAW;
            }
            else if(7 == AudioInfo->FormatTag)
            {
              AudioInfo->Format = SND_PCM_SFMT_MU_LAW;
            }
            else
            {
              switch (AudioInfo->BitsPerSample)
              {
                case 8:
                {
                  AudioInfo->Format = SND_PCM_SFMT_U8;
                }
                  break;
                case 16:
                {
                  if(SndpmBigEndian)
                  {
                    AudioInfo->Format = SND_PCM_SFMT_S16_BE;
                  }
                  else
                  {
                    AudioInfo->Format = SND_PCM_SFMT_S16_LE;
                  }
                }
                  break;
                case 24:
                {
                  if(SndpmBigEndian)
                  {
                    AudioInfo->Format = SND_PCM_SFMT_S24_BE;
                  }
                  else
                  {
                    AudioInfo->Format = SND_PCM_SFMT_S24_LE;
                  }
                }
                  break;
                case 32:
                {
                  if(SndpmBigEndian)
                  {
                    AudioInfo->Format = SND_PCM_SFMT_S32_BE;
                  }
                  else
                  {
                    AudioInfo->Format = SND_PCM_SFMT_S32_LE;
                  }
                }
                  break;
                default:
                {
                  SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
                    "Unsupported number of bits per sample %d", AudioInfo->BitsPerSample));
                  AudioInfo->Format = 0;
                  ret = -1;
                }
                  break;
              }
            }
          }
          else
          {
            SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
              "Sndp_GetAudioParam: Get data block size error, return is %d.\n", BlockSize));
          }
        }
        else
        {
          SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
            "Sndp_GetAudioParam: Get fmt block size error, return is %d.\n", BlockSize));
        }

        printf("@@@@@@@@ %s:%d AudioInfo->SamplesPerSec = %d\n",__func__,__LINE__,AudioInfo->SamplesPerSec);
        printf("@@@@@@@@ %s:%d AudioInfo->Channels = %d\n",__func__,__LINE__,AudioInfo->Channels);
        printf("@@@@@@@@ %s:%d AudioInfo->BitsPerSample = %d\n",__func__,__LINE__,AudioInfo->BitsPerSample);
        printf("@@@@@@@@ %s:%d AudioInfo->FormatTag = %d\n",__func__,__LINE__,AudioInfo->FormatTag);
        printf("@@@@@@@@ %s:%d AudioInfo->DataSize = %d\n",__func__,__LINE__,AudioInfo->DataSize);
      }
      else
      {
        SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
          "Sndp_GetAudioParam: file %s header check failed.\n", AudioInfo->FileName));
      }
    }
    else
    {
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
        "Sndp_GetAudioParam: open file %s failed.\n", AudioInfo->FileName));
    }
  }
  else
  {
    SYST_DEBUG(TRUE,
      SYST_Printf(_SLOG_WARNING,"Sndp_GetAudioParam: invalid parameters.\n"));
  }
  return ret;
}
/******************************************************************************/
/*Name : Sndp_AudioInit                                                       */
/*Role :initialization before playing                                         */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [initialization before playing]                                        */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_AudioInit(Sndp_AudioInfo_t *AudioInfo)
{
  int ret = -1;

  if (AudioInfo != NULL)
  {
    if (0 == Sndp_InitPcmHandle(AudioInfo))
    {
      if (0 == Sndp_InitSndMixer(AudioInfo))
      {
        ret = 0;
      }
      else
      {
        SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
            "Sndp_AudioInit: Sndp_InitSndMixer error.\n"));
      }
    }
    else
    {
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
          "Sndp_AudioInit: Sndp_InitPcmHandle error.\n"));
    }

    if(0 != ret)
    {
      Sndp_AudioRelease(AudioInfo);
    }
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
        "Sndp_AudioInit: invalid parameters.\n"));
  }

  return ret;
}

/******************************************************************************/
/*Name : Sndp_PlayAudio                                                   */
/*Role :play the audio by music ID                               */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [play the audio by music ID]                                      */
/*OD                                                                          */
/******************************************************************************/
static Sndp_RetValue_t Sndp_PlayAudio(char *file, int times)
{
  Sndp_AudioInfo_t AudioInfo;
  fd_set rfds = { 0 };
  fd_set wfds = { 0 };
  int currentTimes = 1;
  int maxFd = 0;
  int rtn = -1;
  Sndp_RetValue_t ret = Sndp_OK;

  if ((file != NULL) && (times > 0))
  {
    memset(&AudioInfo, 0, sizeof(AudioInfo));
    AudioInfo.Fp = NULL;
    strncpy(AudioInfo.FileName,file,sizeof(AudioInfo.FileName) - 1);
    /*get audio parameters*/
    rtn = Sndp_GetAudioParam(&AudioInfo);
    if(0 == rtn)
    {
      /*initialize audio handler*/
      rtn = Sndp_AudioInit(&AudioInfo);
      if (0 == rtn)
      {
        printf("############ %s:%d\n",__func__,__LINE__);
        while (SndpRunning && (AudioInfo.PlayedSize < AudioInfo.DataSize)
               && !AudioInfo.DoneFlag)
        {
          SNDP_FeedWatchDogReq();

          FD_ZERO(&rfds);
          FD_ZERO(&wfds);

          FD_SET(Snd_SndMixer.MixerFd, &rfds);
          FD_SET(Snd_PcmHandle.PcmFd, &wfds);
          maxFd = max(Snd_SndMixer.MixerFd, Snd_PcmHandle.PcmFd);
          rtn = select(maxFd + 1, &rfds, &wfds, NULL, NULL);
          if (-1 == rtn)
          {
            SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,"select fails\n"));
            ret = Sndp_ERROR_AUDIO;
            break; /* break loop to exit cleanly */
          }
          if (FD_ISSET(Snd_SndMixer.MixerFd, &rfds))
          {
            Sndp_handle_mixer();
          }
          if (FD_ISSET(Snd_PcmHandle.PcmFd, &wfds))
          {
            /*time to write data to pcm handle*/
            rtn = Sndp_write_audio_data(&AudioInfo);
            if(-1 == rtn)
            {
              ret = Sndp_ERROR_AUDIO;
              SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,"Sndp_write_audio_data failed\n"));
              break;
            }
          }
          if (AudioInfo.PlayedSize >= AudioInfo.DataSize)
          {
            /*audio played out*/
            if (currentTimes < times)
            {
              /*need to be played again*/
              AudioInfo.DoneFlag = 0;
              AudioInfo.PlayedSize = 0;
              currentTimes++;

              /*direct file pointer to data.*/
              fseek(AudioInfo.Fp, 0, SEEK_SET);
              if(SndpCheckHdr(AudioInfo.Fp) != -1)
              {
                rtn = SndpFindTag(AudioInfo.Fp, "data");
                if(rtn <= 0)
                {
                  ret = Sndp_ERROR_INPUT;
                  break;
                }
                printf("@@@@@@@@@@@ audio data is %d\n",rtn);
              }
              else
              {
                ret = Sndp_ERROR_INPUT;
                break;
              }
            }
            else
            {
              AudioInfo.DoneFlag = 1;
              break;
            }
          }
          /*received STOP command from other thread*/
          if(SNDP_STOP == SndpCurrentStatus)
          {
            ret = Sndp_OK;
            break;
          }
        }
        if(SndpRunning && (SNDP_STOP != SndpCurrentStatus))
        {
          /*this call would block until all data in driver queue is playe out the channel*/
          snd_pcm_plugin_flush(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK);
        }

        Sndp_AudioRelease(&AudioInfo);


      }
      else
      {
        ret = Sndp_ERROR_AUDIO;
        SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
          "Sndp_PlayAudio: Sndp_AudioInit failed.\n"));
      }
    }
    else
    {
      ret = Sndp_ERROR_INPUT;
      fclose(AudioInfo.Fp);
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
        "Sndp_PlayAudio: Sndp_GetAudioParam error.\n"));

    }
  }
  else
  {
    ret = Sndp_ERROR_INPUT;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_PlayAudio: invalid parameters.\n"));
  }

  return ret;
}

int main(int argc, const char *argv[])
{
  if(argc != 3)
  {
    printf("######### invalid parameter.\n");
    printf("######### usage: audioTest [filename] [playtime]\n");
    return -1;
  }

  Sndp_Status.Volume = 60;

  Sndp_PlayAudio(argv[1],atoi(argv[2]));
}

