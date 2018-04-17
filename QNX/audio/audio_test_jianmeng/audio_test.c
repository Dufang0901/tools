/******************************************************************************/
/*@F_NAME:           sndp_slave.c                                             */
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
#include <audio/audio_manager_volume.h>
#include <audio/audio_manager_routing.h>
#include <audio/audio_manager_event.h>
#include <sys/asoundlib.h>
#include <semaphore.h>
#include "sndp.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/mman.h>

/************************LOCAL DEFINES***************************/
/*config file nodes description*/
#define   SNDP_CFG_ROOT_NODE        "root"
#define   SNDP_CFG_VERSION_NODE     "version"
#define   SNDP_CFG_DESCRIPTION_NODE   "description"
#define   SNDP_CFG_AUDIONODES_NODE    "audioNodes"
#define   SNDP_CFG_AUDIONODE_NODE     "audioNode"
#define   SNDP_CFG_IDNODE_NODE      "id"
#define   SNDP_CFG_NAMENODE_NODE      "name"
#define   SNDP_AUDIOPATHPREFIX        "/appfs/audio/"
#define   SNDP_AUDIO_CONFIG_FILE        "/appfs/audio/audioCfg.xml"
#define   SNDP_PCM_SAMPLE_BYTES           (4096*4)

typedef   unsigned           char     UINT8;
typedef     signed           char     SINT8;
typedef   unsigned short     int      UINT16;
typedef     signed short     int      SINT16;

/*#define SNDP_DEBUG*/

typedef struct
{
    ubyte    tag[4];
    int    length;
}
Sndp_RiffTag_t;

typedef struct
{
    ubyte    Riff[4];
    int    Size;
    ubyte    Wave[4];
}
Sndp_RiffHdr_t;

typedef struct
{
    FILE *file1;
    struct timespec start_time;
}
Sndp_WriterData;

typedef struct
{
    sshort   FormatTag;
    sshort   Channels;
    int    SamplesPerSec;
    int    AvgBytesPerSec;
    sshort   BlockAlign;
    sshort   BitsPerSample;
}
Sndp_WaveHdr_t;

/***************************************Module data***********************************/
static char *Sndp_AudioXmlBuffer = NULL;
static int SndpAudioXmlBufferSize = 0;
static float SndpVolumLevel = 40;
static int SndpCurrentMusicID = 0;
static int SndpCurrentTimes = 0;
static int SndpCurrentStatus = SNDP_IDLE;
static bool_t Sndp_TaskRunningFlag = FALSE;
static sem_t Sndp_Sem = {0};
static char SndpFullAudioPath[512] = {0};
static const char *SndpkRiffId = "RIFF";
static const char *SndpkRifxId = "RIFX";
static const char *SndpkWaveId = "WAVE";
static bool SndpRunning = true;
static int Sndp_n = 0;
static int Sndp_N=0;
static int SndpPrint_timing = 0;
static int SndpBsize = 0;
static snd_mixer_group_t SndpGroup = {0};
static snd_mixer_t *SndpMixer_handle = NULL;
static snd_pcm_t *SndpPcm_handle = NULL;
static unsigned int SndpMSamples = 0;
static bool SndpmBigEndian = false;
static char  SndpmSampleBfr1[SNDP_PCM_SAMPLE_BYTES] = {0};
static int SNDP_monitorIoAudio(void);

#ifdef __CLIENT_LINK__
static PA_Alrm_SoundInfo_t Alrm_SoundInfo = {0};
static CB_AlrmSound_t CB_AlrmSound        = {0};
#endif
/**********************************APIS**********************************************/
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
static int SndpErr (ubyte *msg)
{
  if (NULL != msg)
  {
    perror (msg);
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
static int SndpFindTag (FILE * fp, const ubyte *tag)
{
    int     retVal = 0;
    Sndp_RiffTag_t tagBfr = { "", 0 };

    if ((NULL != fp) && (NULL != tag))
    {
    /* Keep reading until we find the tag or hit the EOF.*/
    while (0 < fread ((ubyte *) &tagBfr, sizeof (tagBfr), 1,fp))
    {
      /* If this is our tag, set the length and break.*/
      if (0 == strncmp (tag, tagBfr.tag, sizeof tagBfr.tag))
      {
        retVal = ENDIAN_LE32 (tagBfr.length);
        break;
      }
      /* Skip ahead the specified number of bytes in the stream*/
      fseek (fp, tagBfr.length, SEEK_CUR);
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
static int SndpCheckHdr (FILE * fp)
{
    Sndp_RiffHdr_t riffHdr = { "", 0 };
    int retVal = -1;

    if (NULL != fp)
    {
    /* Read the header and, if successful, play the file
     file or WAVE file.*/
    if (fread (( ubyte *) &riffHdr, sizeof (Sndp_RiffHdr_t), 1, fp) == 0)
    {
      retVal = 0;
    }
    else
    {
    if (strncmp (riffHdr.Riff, SndpkRiffId, strlen (SndpkRiffId)) ||
      strncmp (riffHdr.Wave, SndpkWaveId, strlen (SndpkWaveId)))
      {
        retVal = -1;
      }
    }
    }
    else
    {
      ;/*error do nothing */
    }
    return 0;
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
static void SndpDump_info( int card, int dev, const char* name, int channel )
{
    int rtn = 0;
    snd_pcm_t * SndpPcm_handle = 0;
    snd_pcm_channel_info_t pi = {0};
    int open_mode = (channel == SND_PCM_CHANNEL_PLAYBACK)?SND_PCM_OPEN_PLAYBACK:SND_PCM_OPEN_CAPTURE;

    if (NULL != name)
    {
    if (name[0] != '\0')
    {
      rtn = snd_pcm_open_name(&SndpPcm_handle, name, open_mode);
    }
    else if (card == -1)
    {
      rtn = snd_pcm_open_preferred(&SndpPcm_handle, NULL, NULL, open_mode);
    }
    else
    {
      rtn = snd_pcm_open (&SndpPcm_handle, card, dev, open_mode);
    }
    if (rtn < 0)
    {
      printf("Cannot open %s device \n");
    }
    else
    {
      memset(&pi, 0, sizeof (pi));
      pi.channel = channel;
      if ((rtn = snd_pcm_channel_info( SndpPcm_handle, &pi )) == 0)
      {

      }
      else
      {
        printf("Error retrieving %s info\n");
      }
      snd_pcm_close( SndpPcm_handle );
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
static  int Sndp_HandleXMLNode(xmlNodePtr itemNodePtr,int ID, char *fileName,int maxSize)
{
  int ret = -1;
  xmlChar *key = NULL;
  xmlNodePtr itemNodePtr2 = NULL;
  char audioID = -1;

  if (fileName != NULL)
  {
    while (itemNodePtr != NULL)
    {
      if ((!xmlStrcmp(itemNodePtr->name,  (const xmlChar *)SNDP_CFG_AUDIONODE_NODE)))
      {
      itemNodePtr2 = itemNodePtr->xmlChildrenNode;
      audioID = -1;
      while (itemNodePtr2 != NULL)/*search the file nodes*/
      {
        if ((!xmlStrcmp(itemNodePtr2->name,  (const xmlChar *)SNDP_CFG_IDNODE_NODE)))
        {
          key = xmlNodeGetContent(itemNodePtr2);
          if (NULL != key)
          {
            audioID = atoi(key);
            xmlFree(key);
            if (audioID != ID)
            {
              break; /*Not match ,next node*/
            }
          }
          else
          {
            break;/*Not match ,next node*/
          }
         }
        if ((!xmlStrcmp(itemNodePtr2->name,  (const xmlChar *)SNDP_CFG_NAMENODE_NODE)))
        {
          key = xmlNodeGetContent(itemNodePtr2);
          if (NULL != key)
          {
            if (strlen(key) < maxSize)
            {
              strncpy(fileName,key,strlen(key) + 1);
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
    if (-1 == ret)
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
static int  Sndp_GetAudioFileNameByIDInXMLCFG(char *xmlFile,int ID, char *fileName,int maxSize)
{
  int ret = -1;
  xmlDocPtr  doc = NULL;
  xmlNodePtr cur = NULL;
  xmlChar *key = NULL;
  xmlNodePtr propNodePtr = cur;
  xmlNodePtr itemNodePtr = NULL;

  if ((NULL != xmlFile) && (NULL != fileName))
  {
    /*parse the xml*/
    doc = xmlParseMemory(xmlFile,strlen(xmlFile));
    if ( NULL != doc)
    {
      cur = xmlDocGetRootElement(doc); /*get the root node*/
      if (cur != NULL)
      {
        if (0 == xmlStrcmp(cur->name, (const xmlChar *) SNDP_CFG_ROOT_NODE))
        {
          cur = cur->xmlChildrenNode;
          while (cur != NULL)
          {
            if ((!xmlStrcmp(cur->name,  (const xmlChar *)SNDP_CFG_VERSION_NODE)))
            {
              key = xmlNodeGetContent(cur);
              xmlFree(key);
            }
            if ((!xmlStrcmp(cur->name,  (const xmlChar *)SNDP_CFG_DESCRIPTION_NODE)))
            {
              key = xmlNodeGetContent(cur);
              xmlFree(key);
            }
            if ((!xmlStrcmp(cur->name,  (const xmlChar *)SNDP_CFG_AUDIONODES_NODE)))
            {
              itemNodePtr = cur->xmlChildrenNode;
              ret = Sndp_HandleXMLNode(itemNodePtr,ID,fileName,maxSize);
              if (0 == ret)
              {
                break;
              }
            }
            cur = cur->next;
           }
        }
        else
        {
          printf("document of the wrong type, root node != root");
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

   SndpCurrentStatus = SNDP_IDLE;
   SndpVolumLevel = 60;
   fd = open(SNDP_AUDIO_CONFIG_FILE,O_RDONLY);
   if (-1 != fd)
   {
    lseek(fd, 0L, SEEK_SET);
    SndpAudioXmlBufferSize = lseek(fd, 0L, SEEK_END);
    if (SndpAudioXmlBufferSize > 0)
    {
      Sndp_AudioXmlBuffer = mmap(0, SndpAudioXmlBufferSize, PROT_READ, MAP_PRIVATE, fd, 0);
       if (0 <= snprintf(SndpFullAudioPath,sizeof(SndpFullAudioPath),"%s",SNDP_AUDIOPATHPREFIX))
       {
         ret =0;
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
    fd_set  rfds = {0};
    int mixer_fd = snd_mixer_file_descriptor (SndpMixer_handle);

    FD_ZERO(&rfds);
    FD_SET ( mixer_fd, &rfds);
    if (select (mixer_fd + 1, &rfds, NULL, NULL, NULL) == -1)
    {
      printf("select fails \n");
    }
    snd_mixer_callbacks_t callbacks = { 0, 0, 0, 0 };
    snd_mixer_read (SndpMixer_handle, &callbacks);
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
static int Sndp_write_audio_data(Sndp_WriterData *wd)
{
    struct  timespec current_time = {0};
    snd_pcm_channel_status_t status = {0};
    int     written = 0;
    int ret = 0;

    if (NULL != wd)
    {
        if ((Sndp_n = fread (SndpmSampleBfr1, 1, min (SndpMSamples - Sndp_N, SndpBsize), wd->file1)) > 0)
        {
            written = snd_pcm_plugin_write (SndpPcm_handle, SndpmSampleBfr1, Sndp_n);
            if ( SndpPrint_timing )
            {
                clock_gettime( CLOCK_REALTIME, &current_time );
                printf("Sent frag at %llu\n");
            }
            if (written < Sndp_n)
      {
        memset(&status, 0, sizeof (status));
        status.channel = SND_PCM_CHANNEL_PLAYBACK;
        if (snd_pcm_plugin_status (SndpPcm_handle, &status) >= 0)
        {
          switch(status.status)
          {
          case SND_PCM_STATUS_READY:
            /*break;*/
          case SND_PCM_STATUS_UNDERRUN:
            /*break;*/
          case SND_PCM_STATUS_CHANGE:
          {
            if ( status.status == SND_PCM_STATUS_UNDERRUN )
            {
              printf("Audio underrun occurred \n");
            }
            else if ( status.status == SND_PCM_STATUS_CHANGE )
            {
              printf("Audio device change occurred from %s to %s\n");
            }
            else
            {
              ;/*do nothing*/
            }
            if (snd_pcm_plugin_prepare (SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0)
            {
              printf("underrun: playback channel prepare error\n");
              ret = -1;
            }
          }
            break;
          case SND_PCM_STATUS_UNSECURE:
          {
            printf("Channel unsecure\n");
            if (snd_pcm_plugin_prepare (SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0)
            {
              printf("unsecure: playback channel prepare error\n");
              ret = -1;
            }
          }
            break;
          case SND_PCM_STATUS_ERROR:
          {

            printf("error: playback channel failure\n");
            ret = -1;
          }
            break;
          case SND_PCM_STATUS_PREEMPTED:
          {
            printf("error: playback channel preempted\n");
            ret = -1;
          }
            break;
          default:
            break;
          }
          if(0 == ret)
          {
            if (written < 0)
            {
              written = 0;
            }
            written += snd_pcm_plugin_write (SndpPcm_handle, SndpmSampleBfr1 + written, Sndp_n - written);
          }
          else
          {
            ;/*error do nothing*/
          }
        }
        else
        {
          printf("underrun: playback channel status error\n");
          ret = -1;
        }
      }
      Sndp_N += written;
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
static int Sndp_PlayAudio(char *file,int times)
{
int     card = -1;
int     dev = 0;
Sndp_WriterData wd = {0};
Sndp_WaveHdr_t wavHdr1 = {0};
int     mSampleRate = 0;
int     mSampleChannels = 0;
int     mSampleBits = 0;
int     fragsize = -1;
int     mode = SND_PCM_OPEN_PLAYBACK;
int     rtn = 0;
snd_pcm_channel_info_t pi = {0};
snd_pcm_channel_params_t pp = {0};
snd_pcm_channel_setup_t setup = {0};
fd_set  rfds = {0}, wfds = {0};
int     num_frags = -1;
int     num_buffered_frags = 0;
char   *sub_opts = NULL, *sub_opts_copy = NULL, *value = NULL;
char    name[_POSIX_PATH_MAX] = { 0 };
int     volume = 0;
char    mixer_name[32] = {0};
int     mix_name_enable = -1;
int     uses_audioman_handle = 1;
unsigned int audioman_handle = 0;
char   *type = "multimedia";
int    rate_method = 0;
int    use_mmap = 0;
int    pcm_fd, mixer_fd = 0;
int    currentTimes = 1;
int    ret = 0;

if ((NULL != file) &&  (times > 0))
{
   /*open pcm handle */
  if ((rtn = snd_pcm_open_preferred (&SndpPcm_handle, &card, &dev, mode)) >= 0)
  {
    if (0 == audio_manager_get_handle (audio_manager_get_type_from_name(type), 0, true, &audioman_handle)  )
    {
       if ((rtn = snd_pcm_set_audioman_handle (SndpPcm_handle, audioman_handle)) < 0)
        {
         printf("set audioman handle fails \n");
          ret = err ("set audioman handle");
        }
    }
    if(0 == ret)
    {
      if ((wd.file1 = fopen (file, "r")) != NULL)
      {
        if (SndpCheckHdr (wd.file1) != -1)
        {
          SndpMSamples = SndpFindTag (wd.file1, "fmt ");
          fread (&wavHdr1, sizeof (wavHdr1), 1, wd.file1);
          fseek (wd.file1, (SndpMSamples - sizeof (Sndp_WaveHdr_t)), SEEK_CUR);
          if ( SndpmBigEndian )
          {
            mSampleRate = ENDIAN_BE32 (wavHdr1.SamplesPerSec);
            mSampleChannels = ENDIAN_BE16 (wavHdr1.Channels);
            mSampleBits = ENDIAN_BE16 (wavHdr1.BitsPerSample);
            wavHdr1.FormatTag = ENDIAN_BE16 (wavHdr1.FormatTag);
          }
          else
          {
            mSampleRate = ENDIAN_LE32 (wavHdr1.SamplesPerSec);
            mSampleChannels = ENDIAN_LE16 (wavHdr1.Channels);
            mSampleBits = ENDIAN_LE16 (wavHdr1.BitsPerSample);
            wavHdr1.FormatTag = ENDIAN_LE16 (wavHdr1.FormatTag);
          }
          snd_pcm_plugin_set_disable (SndpPcm_handle, PLUGIN_DISABLE_MMAP);
          snd_pcm_plugin_set_enable (SndpPcm_handle, PLUGIN_ROUTING);
          memset (&pi, 0, sizeof (pi));
          pi.channel = SND_PCM_CHANNEL_PLAYBACK;
          if ((rtn = snd_pcm_plugin_info (SndpPcm_handle, &pi)) >= 0)
          {
            memset (&pp, 0, sizeof (pp));
            pp.mode = SND_PCM_MODE_BLOCK;
            pp.channel = SND_PCM_CHANNEL_PLAYBACK;
            pp.start_mode = SND_PCM_START_FULL;
            pp.stop_mode = SND_PCM_STOP_STOP;
            pp.buf.block.frag_size = pi.max_fragment_size;
            if (fragsize != -1)
            {
              pp.buf.block.frag_size = fragsize;
            }
            pp.buf.block.frags_max = num_frags;
            pp.buf.block.frags_buffered_max = num_buffered_frags;
            pp.buf.block.frags_min = 1;
            pp.format.interleave = 1;
            pp.format.rate = mSampleRate;
            pp.format.voices = mSampleChannels;
            if(6 == wavHdr1.FormatTag)
            {
              pp.format.format = SND_PCM_SFMT_A_LAW;
            }
            else if(7 == wavHdr1.FormatTag)
            {
              pp.format.format = SND_PCM_SFMT_MU_LAW;
            }
            else
            {
            switch (mSampleBits)
            {
              case 8:
              {
                pp.format.format = SND_PCM_SFMT_U8;
              }
              break;
              case 16:
              {
                if (SndpmBigEndian)
                {
                  pp.format.format = SND_PCM_SFMT_S16_BE;
                }
                else
                {
                  pp.format.format = SND_PCM_SFMT_S16_LE;
                }
              }
              break;
              case 24:
              {
                if (SndpmBigEndian)
                {
                  pp.format.format = SND_PCM_SFMT_S24_BE;
                }
                else
                {
                  pp.format.format = SND_PCM_SFMT_S24_LE;
                }
              }
              break;
              case 32:
              {
                if (SndpmBigEndian)
                {
                  pp.format.format = SND_PCM_SFMT_S32_BE;
                }
                else
                {
                  pp.format.format = SND_PCM_SFMT_S32_LE;
                }
              }
              break;
              default:
              {
                printf("Unsupported number of bits per sample %d");
                ret = SndpErr("wavHdr1.FormatTag not support ");
              }
              break;
            }
            }
            if(0 == ret )
            {
              strcpy (pp.sw_mixer_subchn_name, "Wave playback channel");
              if ((rtn = snd_pcm_plugin_set_src_method(SndpPcm_handle, rate_method)) != rate_method)
              {
                printf("Failed to apply rate_method %d, using %d\n");
              }
                if ((rtn = snd_pcm_plugin_params (SndpPcm_handle, &pp)) >= 0)
                {
                  if ((rtn = snd_pcm_plugin_prepare (SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
                  {
                    printf("snd_pcm_plugin_prepare failed: %s\n");
                  }
                  memset (&setup, 0, sizeof (setup));
                  memset (&SndpGroup, 0, sizeof (SndpGroup));
                  setup.channel = SND_PCM_CHANNEL_PLAYBACK;
                  setup.mixer_gid = &SndpGroup.gid;
                  if ((rtn = snd_pcm_plugin_setup (SndpPcm_handle, &setup)) >= 0)
                  {
                    SndpBsize = setup.buf.block.frag_size;
                    SndpMSamples = SndpFindTag (wd.file1, "data");
                    FD_ZERO (&rfds);
                    FD_ZERO (&wfds);
                    Sndp_n = 1;
                    if (SndpGroup.gid.name[0] == 0)
                    {
                      printf("Mixer Pcm Group [%s] Not Set \n");
                    }
                    else
                    {
                      if ((rtn = snd_mixer_open (&SndpMixer_handle, card, setup.mixer_device)) >= 0)
                      {
                        if (SndpVolumLevel >=0)
                        {
                          if ((rtn = snd_mixer_group_read (SndpMixer_handle, &SndpGroup)) < 0)
                          {
                            printf("snd_mixer_group_read failed: %s\n");
                          }
                          volume = (float)(SndpGroup.max - SndpGroup.min) * ( SndpVolumLevel / 100);
                          if (SndpGroup.channels & SND_MIXER_CHN_MASK_FRONT_LEFT)
                            SndpGroup.volume.names.front_left = (int)volume;
                          if (SndpGroup.channels & SND_MIXER_CHN_MASK_REAR_LEFT)
                            SndpGroup.volume.names.rear_left = (int)volume;
                          if (SndpGroup.channels & SND_MIXER_CHN_MASK_FRONT_CENTER)
                            SndpGroup.volume.names.front_center = (int)volume;
                          if (SndpGroup.channels & SND_MIXER_CHN_MASK_FRONT_RIGHT)
                            SndpGroup.volume.names.front_right = (int)volume;
                          if (SndpGroup.channels & SND_MIXER_CHN_MASK_REAR_RIGHT)
                            SndpGroup.volume.names.rear_right = (int)volume;
                          if (SndpGroup.channels & SND_MIXER_CHN_MASK_WOOFER)
                            SndpGroup.volume.names.woofer = (int)volume;
                          if ((rtn = snd_mixer_group_write (SndpMixer_handle, &SndpGroup)) < 0)
                          {
                            printf("snd_mixer_group_write failed: %s\n");
                          }
                            while (SndpRunning && (Sndp_N < SndpMSamples) && (Sndp_n > 0))
                            {
                              FD_ZERO(&rfds);
                              FD_ZERO(&wfds);
                              if (SndpMixer_handle)
                              {
                                mixer_fd = snd_mixer_file_descriptor (SndpMixer_handle);
                                FD_SET (mixer_fd, &rfds);
                              }
                              pcm_fd = snd_pcm_file_descriptor (SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK);
                              FD_SET (pcm_fd, &wfds);
                              rtn = max (mixer_fd, pcm_fd);
                              if (select (rtn + 1, &rfds, &wfds, NULL, NULL) == -1)
                              {
                                printf("select fails\n");
                                ret= -1;
                                break; /* break loop to exit cleanly */
                              }
                              if (SndpMixer_handle && FD_ISSET (mixer_fd, &rfds))
                              {
                                Sndp_handle_mixer();
                              }
                              if (FD_ISSET (snd_pcm_file_descriptor (SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK), &wfds))
                              {
                                if (-1 == Sndp_write_audio_data(&wd) && (SNDP_STOP != SndpCurrentStatus))
                                {
                                  ret =-1;
                                  printf("Sndp_write_audio_data failed\n");
                                  break;
                                }
                              }
                              if ((Sndp_N >= SndpMSamples) && (currentTimes < times))
                              {
                                currentTimes ++;
                                fseek(wd.file1,0,SEEK_SET);
                                Sndp_N=0;
                                if (SndpCheckHdr (wd.file1) != -1)
                                {
                                  SndpMSamples = SndpFindTag (wd.file1, "fmt ");
                                  fread (&wavHdr1, sizeof (wavHdr1), 1, wd.file1);
                                  fseek (wd.file1, (SndpMSamples - sizeof (Sndp_WaveHdr_t)), SEEK_CUR);
                                  SndpMSamples = SndpFindTag (wd.file1, "data");
                                }
                                else
                                {
                                  ret = -1;
                                  break;
                                }
                               }
                               if (SNDP_STOP == SndpCurrentStatus)
                               {
                              ret = 0;
                              break;
                               }
                            }
                            if (SndpRunning  && (SNDP_STOP != SndpCurrentStatus))
                            {
                              snd_pcm_plugin_flush (SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK);
                            }
                          }
                          snd_mixer_close (SndpMixer_handle);
                        }
                        else
                        {
                          printf("snd_mixer_open failed: %s\n");
                          ret = SndpErr("snd_mixer_open failed");
                        }
                      }
                    }
                    else
                    {
                      printf("snd_pcm_plugin_setup failed: %s\n");
                      ret = SndpErr("snd_pcm_plugin_setup failed");
                    }
                }
                else
                {
                  printf("snd_pcm_plugin_params failed: %s, why_failed = %d\n");
                  ret = SndpErr("snd_pcm_plugin_params failed");
                }
            }
          }
          else
          {
            printf("snd_pcm_plugin_info failed: %s\n #1 \n");
            ret = SndpErr ("snd_pcm_plugin_info ");
          }
        }
        else
        {
          printf("SndpCheckHdr #1 \n");
          ret = SndpErr ("SndpCheckHdr #1");
        }
        fclose(wd.file1);
      }
      else
      {
        printf("file open #1 fails \n");
        ret =  SndpErr ("file open #1");
      }
    }
    if(audioman_handle)
    {
      audio_manager_free_handle (audioman_handle);
    }
    snd_pcm_close (SndpPcm_handle);
  }
  else
  {
    printf("device open fails \n");
    ret = SndpErr ("device open");
  }
}
else
{
  printf("parameter error\n");
  ret =  SndpErr ("parameter error");
}
Sndp_N = 0;
Sndp_n = 0;
return ret;
}
/******************************************************************************/
/*Name : SNDP_StopAudio                                                       */
/*Role : stop current music                                  */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [stop current music]                                      */
/*OD                                                                          */
/******************************************************************************/
int SNDP_StopAudio(void)
{
  SndpCurrentStatus = SNDP_STOP;
  return 0;
}
/******************************************************************************/
/*Name : SNDP_GetPlayStatus                                                       */
/*Role : get play status                                     */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [get play status]                                      */
/*OD                                                                          */
/******************************************************************************/
SNDP_STATUS SNDP_GetPlayStatus(void)
{
  return SndpCurrentStatus;
}

/******************************************************************************/
/*Name : SNDP_SetAudioVolum                                                       */
/*Role : set the music volum level                                        */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [ set the music volum level ]                                      */
/*OD                                                                          */
/******************************************************************************/
void SNDP_SetAudioVolum(int level)
{
  if ((level >= 0) && (level <= 100))
  {
    SndpVolumLevel = level;
  }
}

/******************************************************************************/
/*Name : SNDP_Init                                                            */
/*Role : Initialise module data                                               */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [ Init all internal module data ]                                      */
/*OD                                                                          */
/******************************************************************************/
void SNDP_Init(void)
{
  sem_init(&Sndp_Sem,0,0);
}

/******************************************************************************/
/* Name: SNDP_PlayAudioByID                                                          */
/* Role: SNDP_PlayAudioByID                                                    */
/* Interface:     none                                                        */
/* Pre-condition: none                                                        */
/* Constraints:   none                                                        */
/* Behavior:                                                                  */
/*   DO                                                                       */
/*    [SNDP_PlayAudioByID ]                                                     */
/*   OD                                                                       */
int SNDP_PlayAudioByID(unsigned int musicID,unsigned int times)
{
  int ret = -1;
  if (SNDP_BUSY != SndpCurrentStatus)
  {
    SndpCurrentStatus = SNDP_BUSY;
    SndpCurrentMusicID = musicID;
    SndpCurrentTimes = times;
    sem_post(&Sndp_Sem);
    ret = 0;
  }
  return ret;
}

/*This function only for test. and will be delete later*/
void * SNDP_TestThread(void *arg)
{

  /*this just for test*/

  static UINT8 lastID = 0;
  static UINT8 lastCmdID = 0;
  static UINT8 Sndp_SequenceNumber = 0;
  static UINT8 Sndp_SoundId = 0;
  static int Sndp_TestDelay = 0;

  while(1)
  {
    if (Sndp_TestDelay%10 == 0)
    {
      Sndp_SequenceNumber++;
      Sndp_SoundId++;
      if (Sndp_SoundId == 3)
      {
        Sndp_SoundId = 1;
      }
    }
    Sndp_TestDelay++;
    if (lastCmdID != Sndp_SequenceNumber)
    {
      if (((Sndp_SoundId == 0) || (lastID != Sndp_SoundId)) && (SNDP_BUSY == SndpCurrentStatus))
      {
        SndpCurrentStatus = SNDP_STOP;
        lastID = Sndp_SoundId;
        while((SNDP_IDLE != SndpCurrentStatus) && (SNDP_ERROR != SndpCurrentStatus))
        {
          usleep(1000);
        };
      }
      if (Sndp_SoundId > 0)
      {
        if (0 == SNDP_PlayAudioByID(Sndp_SoundId,1))
        {
          lastCmdID = Sndp_SequenceNumber;
        }
      }
      else
      {
        lastCmdID = Sndp_SequenceNumber;
      }
    }


    usleep(1000*10);
  }
  return NULL;
}
/******************************************************************************/
/* Name: SNDP_Task_ts                                                          */
/* Role: Start SNDP task                                                      */
/* Interface:     none                                                        */
/* Pre-condition: none                                                        */
/* Constraints:   none                                                        */
/* Behavior:                                                                  */
/*   DO                                                                       */
/*    [ start SNDP task ]                                                     */
/*   OD                                                                       */
/******************************************************************************/

int main(void)
{
  struct timespec tm;

  Sndp_TaskRunningFlag =  TRUE;
  /*this just for test code*/
  pthread_attr_t attr = {0};
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  /*create thread to handle events*/
  pthread_create( NULL, &attr, &SNDP_TestThread, NULL );

  do
  {

    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_nsec += 100*1000*1000;

    if(tm.tv_nsec >= 1000000000L)
    {
      tm.tv_nsec -= 1000000000L;
      tm.tv_sec += 1;
    }

    /*check io audio driver periodlly*/
    if (0 == SNDP_monitorIoAudio())
    {
    }
    else
    {
      SndpCurrentStatus = SNDP_ERROR;
    }

    /*wait for upgrade requesting*/
    if (EOK == sem_timedwait( &Sndp_Sem, &tm ))
    {
      if (0 == Sndp_GetAudioFileNameByIDInXMLCFG(Sndp_AudioXmlBuffer,SndpCurrentMusicID,(SndpFullAudioPath + strlen(SNDP_AUDIOPATHPREFIX)),sizeof(SndpFullAudioPath) -  strlen(SNDP_AUDIOPATHPREFIX)))
      {
        if (0 == Sndp_PlayAudio(SndpFullAudioPath,SndpCurrentTimes))
        {
          SndpCurrentStatus = SNDP_IDLE;
        }
        else
        {
          SndpCurrentStatus = SNDP_ERROR;
          printf("Sndp_PlayAudio failed\n");
        }
      }
      else
      {
        SndpCurrentStatus = SNDP_ERROR;
        printf("Sndp_GetAudioFileNameByIDInXMLCFG failed\n");
      }
    }
    else if(ETIMEDOUT == errno)
    {
      /* just timeout */
    }
    else
    {
      SndpCurrentStatus = SNDP_ERROR;
      printf("sem_wait failed\n");
      usleep(10000);/*sshort delay to make sure system doesn't stuck*/
    }

  }
#ifndef __POLYSPACE__
  while(TRUE == Sndp_TaskRunningFlag);
#else
  while(0);
#endif

}

int SNDP_monitorIoAudio(void)
{
  int card = -1;
  int dev = 0;
  int mode = SND_PCM_OPEN_PLAYBACK;
  int rtn = 0;
  int ret = 0;
  /*check io audio driver is OK or not*/
  if((rtn = snd_pcm_open_preferred(&SndpPcm_handle, &card, &dev, mode)) >= 0)
  {
    snd_pcm_close(SndpPcm_handle);
  }
  else
  {
    printf("device open fails \n");
    ret = SndpErr("device open");
  }
  return ret;
}

