/******************************************************************************/
/*@F_NAME:           sndp.c                                                */
/*@F_PURPOSE:        source file for Audio application API                   */
/*@F_CREATED_BY:     Jun Ma                                    */
/*@F_CREATION_DATE:   2017-3-17                                          */
/*@F_MPROC_TYPE:     up independent                                           */
/************************************** (C) Copyright 2011 Magneti Marelli ****/
/*______ I N C L U D E - F I L E S ___________________________________________*/
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

#define SYST_DEBUG(Condition, Action) if(Condition) {Action;}
#define SYST_Printf(Level,fmt, ...) {slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), Level | _SLOG_TEXTBIT, fmt,  ##__VA_ARGS__);}


/*______ L O C A L - D E F I N E S ___________________________________________*/
/*config file nodes description*/
#define   SNDP_CFG_ROOT_NODE  			      "root"
#define   SNDP_CFG_VERSION_NODE  		      "version"
#define   SNDP_CFG_DESCRIPTION_NODE  	    "description"
#define   SNDP_CFG_AUDIONODES_NODE   	    "audioNodes"
#define   SNDP_CFG_AUDIONODE_NODE   	    "audioNode"
#define   SNDP_CFG_IDNODE_NODE   		      "id"
#define   SNDP_CFG_NAMENODE_NODE   		    "name"
#define   SNDP_AUDIOROMPATHPREFIX   			"/appfs/audio/"
#define   SNDP_AUDIO_CONFIG_ROMFILE  			"/appfs/audio/audioCfg.xml"
#define   SNDP_AUDIOROMPATHFILES          "/appfs/audio/*"
#define   SNDP_AUDIOPATHPREFIX            "/tmp/audio/"
#define   SNDP_AUDIO_CONFIG_FILE          "/tmp/audio/audioCfg.xml"
#define   SNDP_PCM_SAMPLE_BYTES     			(4096*4)

/*#define SNDP_DEBUG*/
/*______ L O C A L - T Y P E S _______________________________________________*/
typedef enum
{
  Sndp_CMD_NONE,
  Sndp_CMD_PLAY,
  Sndp_CMD_STOP
}Sndp_CmdType_t;

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
  int PlayedSize;
  int DoneFlag;
}Sndp_AudioInfo_t;

typedef struct
{
  SNDP_STATUS Status;
  unsigned int AudioId;
  float Volume;
  UINT8 CmdId;
}Sndp_StatusInfo_t;

typedef struct
{
  int CmdFlag;
  int Cmd;
  int PlayTime;
  int AudioId;
  UINT8 CmdId;
}Sndp_CmdInfo_t;

/*______ G L O B A L - D A T A _______________________________________________*/
/*______ P R I V A T E - D A T A _____________________________________________*/
/*______ L O C A L - D A T A _________________________________________________*/
static snd_mixer_t *SndpMixer_handle = NULL;
static snd_pcm_t *SndpPcm_handle = NULL;
static snd_mixer_group_t SndpGroup = { 0 };
static const char *SndpkRiffId = "RIFF";
static const char *SndpkRifxId = "RIFX";
static const char *SndpkWaveId = "WAVE";
static char *Sndp_AudioXmlBuffer = NULL;
static int SndpAudioXmlBufferSize = 0;
static int Sndp_BlockSize = 0;
static bool_t Sndp_TaskRunningFlag = FALSE;
static sem_t Sndp_Sem = { 0 };
static bool SndpmBigEndian = false;
static char SndpmSampleBfr1[SNDP_PCM_SAMPLE_BYTES] = { 0 };
static Sndp_StatusInfo_t Sndp_Status = { 0 };
static Sndp_CmdInfo_t Sndp_Cmd = { 0 };
static Sndp_PcmHandle_t Snd_PcmHandle;
static Sndp_SndMixer_t Snd_SndMixer;
static pthread_rwlock_t Sndp_RWLockStatus;
static pthread_rwlock_t Sndp_RWLockCmd;
/*______ L O C A L - M A C R O S _____________________________________________*/
/*______ I M P O R T - F U N C T I O N S - P R O T O T Y P E S _______________*/
/*______ L O C A L - F U N C T I O N S - P R O T O T Y P E S _________________*/
static int SndpErr(ubyte *msg);
static int SndpFindTag(FILE * fp, const ubyte *tag);
static int SndpCheckHdr(FILE * fp);
static void SndpDump_info(int card, int dev, const char* name, int channel);
static int Sndp_HandleXMLNode(xmlNodePtr itemNodePtr, int ID, char *fileName,
                              int maxSize);
static int Sndp_GetAudioFileNameByIDInXMLCFG(char *xmlFile, int ID,
                                             char *fileName, int maxSize);
static int Sndp_GetAudioByCmd(char *fileName, int length, int *playTime);
static int Sndp_CpAudioToRam(void);
static int Sndp_InitAudioChannel(void);
static void Sndp_SetStatus(SNDP_STATUS status);
static void Sndp_ClearSem(void);
static void Sndp_ClearCmd(void);
static int Sndp_CheckNewCmd(void);
static void Sndp_handle_mixer(void);
static int Sndp_write_audio_data(Sndp_AudioInfo_t *AudioInfo);
static int Sndp_AudioRelease(Sndp_AudioInfo_t *AudioInfo);
static int Sndp_InitSndMixer(void);
static int Sndp_InitPcmHandle(Sndp_AudioInfo_t *AudioInfo);
static int Sndp_GetAudioParam(Sndp_AudioInfo_t *AudioInfo);
static int Sndp_AudioInit(Sndp_AudioInfo_t *AudioInfo);
static Sndp_RetValue_t Sndp_PlayAudio(char *file, int times);
static int Sndp_MonitorIoAudio(void);
/*______ G L O B A L - F U N C T I O N S _____________________________________*/
void SNDP_FeedWatchDogReq()
{

  
}
void print_time(const char *info)
{
#if 0
  time_t t;
  
  time(&t);
  printf("**************%s: %s\n",info,ctime(&t));
  
#endif

  struct timespec tm;

  clock_gettime(CLOCK_REALTIME, &tm);
  //printf("**************%s: sec:%lld  nsec:%ld\n",info,tm.tv_sec,tm.tv_nsec);
  printf("**************%s: nsec:%ld\n",info,tm.tv_nsec);

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
void *SNDP_Task_ts(void *arg)
{
  struct timespec tm;
  char AudioName[128];
  int PlayTime;
  int ret = -1;

  Sndp_TaskRunningFlag = TRUE;

  Sndp_CpAudioToRam();

  do
  {
    SNDP_FeedWatchDogReq();


    /*wait for upgrade requesting*/
    //tm = QUOS_TimeAfterNow(DELAY_100MS);
    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_nsec += 100 * 100 * 1000;

    if(tm.tv_nsec >= 1000 * 1000 * 1000)
    {
      tm.tv_nsec -= 1000 * 1000 * 1000;
      tm.tv_sec += 1;
    }
    if (EOK == sem_timedwait(&Sndp_Sem, &tm))
    {
      
      Sndp_SetStatus(SNDP_BUSY);
      Sndp_ClearCmd();
      print_time("before:");
      Sndp_ClearSem();
      print_time("after :");
      ret = Sndp_GetAudioByCmd(AudioName, sizeof(AudioName), &PlayTime);
      if (0 == ret)
      {
        printf("########## audioId is %d\n",Sndp_Status.AudioId);
        if (Sndp_PlayAudio(AudioName, PlayTime) != Sndp_ERROR_AUDIO)
        {
          Sndp_SetStatus(SNDP_IDLE);
        }
        else
        {
          Sndp_SetStatus(SNDP_ERROR);
          SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,"Sndp_PlayAudio failed\n"));
        }
      }
      else
      {
        Sndp_SetStatus(SNDP_IDLE);
        SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
          "Sndp_GetAudioFileNameByIDInXMLCFG failed\n"));
      }

    }
    else if (ETIMEDOUT == errno)
    {
      /*timeout, check audio driver status.*/
      if (Sndp_MonitorIoAudio())
      {
        Sndp_SetStatus(SNDP_ERROR);
      }
      else
      {
        Sndp_SetStatus(SNDP_IDLE);
      }
    }
    else
    {
      Sndp_SetStatus(SNDP_ERROR);
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,"sem_timedwait error.\n"));
    }

    /*update status to WKSS*/
    //WKSS_TaskState(SNDP_Task_al,WKSS_INACTIVE_TASK);


#ifdef __POLYSPACE__
    break;
#endif
    usleep(100);  /*short delay to make sure system doesn't stuck*/
  } while (TRUE == Sndp_TaskRunningFlag);

  //WKSS_TaskState(SNDP_Task_al, WKSS_INACTIVE_TASK );
  //TerminateTask();
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
  Sndp_InitAudioChannel();
}

/******************************************************************************/
/* Name: SNDP_WakeUp                                                          */
/* Role: Start SNDP task                                                      */
/* Interface:     none                                                        */
/* Pre-condition: none                                                        */
/* Constraints:   none                                                        */
/* Behavior:                                                                  */
/*   DO                                                                       */
/*    [ start SNDP task ]                                                     */
/*   OD                                                                       */
/******************************************************************************/
void SNDP_WakeUp(void)
{

  //SetRelAlarm(SNDP_Task_al, SYST_SNDP_TASK_DELAY, SYST_SNDP_TASK_PERIOD);
  //WKSS_TaskState(SNDP_Task_al, WKSS_ACTIVE_TASK );
}

/******************************************************************************/
/* Name: SNDP_Sleep                                                          */
/* Role: Start SNDP task                                                      */
/* Interface:     none                                                        */
/* Pre-condition: none                                                        */
/* Constraints:   none                                                        */
/* Behavior:                                                                  */
/*   DO                                                                       */
/*    [ sleep SNDP task ]                                                     */
/*   OD                                                                       */
/******************************************************************************/
void SNDP_Sleep(void)
{
#if 0
  CancelAlarm(SNDP_Task_al);
  if(NULL != Sndp_AudioXmlBuffer)
  {
    munmap(Sndp_AudioXmlBuffer, SndpAudioXmlBufferSize);
  }
#endif
}

/******************************************************************************/
/* Name: SNDP_PlayAudioByID                                                   */
/* Role: SNDP_PlayAudioByID                                                   */
/* Interface:     none                                                        */
/* Pre-condition: none                                                        */
/* Constraints:   none                                                        */
/* Behavior:                                                                  */
/*   DO                                                                       */
/*    [SNDP_PlayAudioByID ]                                                   */
/*   OD                                                                       */
/******************************************************************************/
int SNDP_PlayAudioByID(unsigned int musicID, unsigned int times, UINT8 cmdId)
{
  int ret = 0;
  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockCmd))
  {
    Sndp_Cmd.CmdFlag = 1;
    Sndp_Cmd.AudioId = musicID;
    Sndp_Cmd.PlayTime = times;
    Sndp_Cmd.Cmd = Sndp_CMD_STOP;
    Sndp_Cmd.CmdId = cmdId;
    sem_post(&Sndp_Sem);
    pthread_rwlock_unlock(&Sndp_RWLockCmd);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "SNDP_PlayAudioByID: get WR lock failed.\n"));
  }

  return ret;
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
int SNDP_GetPlayStatus(SNDP_STATUS *status, int *audioId, UINT8 *cmdId)
{
  int ret = 0;

  if ((status != NULL) && (audioId != NULL) && (cmdId != NULL))
  {
    if (EOK == pthread_rwlock_rdlock(&Sndp_RWLockStatus))
    {
      *status = Sndp_Status.Status;
      *audioId = Sndp_Status.AudioId;
      *cmdId = Sndp_Status.CmdId;
      pthread_rwlock_unlock(&Sndp_RWLockStatus);
    }
    else
    {
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
          "SNDP_GetPlayStatus: get RD lock failed.\n"));
    }
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "SNDP_GetPlayStatus: invalid parameters.\n"));
  }
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
int SNDP_StopAudio(UINT8 cmdId)
{
#if 0
  SndpCurrentStatus = SNDP_STOP;
#endif
  int ret = 0;
  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockCmd))
  {
    Sndp_Cmd.CmdFlag = 1;
    Sndp_Cmd.AudioId = 0;
    Sndp_Cmd.PlayTime = 0;
    Sndp_Cmd.Cmd = Sndp_CMD_STOP;
    Sndp_Cmd.CmdId = cmdId;
    pthread_rwlock_unlock(&Sndp_RWLockCmd);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "SNDP_StopAudio: get WR lock failed.\n"));
  }
  return ret;
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
void SNDP_SetAudioVolum(float level)
{
  if((level >= 0) && (level <= 100))
  {
    if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockStatus))
    {
      Sndp_Status.Volume = level;
      pthread_rwlock_unlock(&Sndp_RWLockStatus);
    }
    else
    {
      SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
        "SNDP_StopAudio: get WR lock failed.\n"));
    }
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "SNDP_SetAudioVolum: invalid parameters.\n"));
  }
}


/*______ P R I V A T E - F U N C T I O N S ___________________________________*/
/*______ L O C A L - F U N C T I O N S _______________________________________*/
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
    int maxSize)
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
    char *fileName, int maxSize)
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
          SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,
            "document of the wrong type, root node != root"));
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
/*Name : Sndp_GetAudioByCmd                                                   */
/*Role : get audio name and play time by command                              */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                                                     */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_GetAudioByCmd(char *fileName, int length, int *playTime)
{
  int ret = 0;
  char name[128];
  int audioId = 0;
  UINT8 cmdId = 0;

  bzero(name,sizeof(name));
  bzero(fileName,length);

  if (EOK == pthread_rwlock_rdlock(&Sndp_RWLockCmd))
  {
    *playTime = Sndp_Cmd.PlayTime;
    audioId = Sndp_Cmd.AudioId;
    cmdId = Sndp_Cmd.CmdId;
    pthread_rwlock_unlock(&Sndp_RWLockCmd);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_GetAudioByCmd: get RD lock failed.\n"));
  }

  ret = Sndp_GetAudioFileNameByIDInXMLCFG(Sndp_AudioXmlBuffer, audioId,
                                          name, sizeof(name));
  if (0 == ret)
  {
    snprintf(fileName, length - 1, "%s%s",SNDP_AUDIOPATHPREFIX,name);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_GetAudioByCmd: Sndp_GetAudioFileNameByIDInXMLCFG() failed.\n"));
  }

  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockStatus))
  {
    Sndp_Status.AudioId = audioId;
    Sndp_Status.CmdId = cmdId;
    pthread_rwlock_unlock(&Sndp_RWLockStatus);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_GetAudioByCmd: get WR lock failed.\n"));
  }
  return ret;
}
/******************************************************************************/
/*Name : Sndp_CpAudioToRam                                                    */
/*Role :copy audio files from eMMC to RAM                                     */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                                                     */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_CpAudioToRam(void)
{
  char cmdBuf[128] = {0};

  /*SNDP access file in EMMC may be blocked,
   * so copy Audio files from emmc to ram, */
  if(access(SNDP_AUDIOPATHPREFIX,F_OK))
  {
    snprintf(cmdBuf,sizeof(cmdBuf),"mkdir %s ", SNDP_AUDIOPATHPREFIX);
    system(cmdBuf);
  }
  snprintf(cmdBuf,sizeof(cmdBuf),"cp %s %s", SNDP_AUDIOROMPATHFILES,
            SNDP_AUDIOPATHPREFIX);
  system(cmdBuf);

  return 0;

}


/******************************************************************************/
/*Name : Sndp_InitAudioChannel                                                   */
/*Role :initialize the audio channel                            */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [initialize the audio channel]                                      */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_InitAudioChannel(void)
{
  int ret = 0;
  int fd = 0;

  /*initialize semphora*/
  sem_init(&Sndp_Sem, 0, 0);

  /*initialize RW locks*/
  if (pthread_rwlock_init(&Sndp_RWLockStatus,NULL) != EOK)
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitAudioChannel: initialize Snd_RWLockStatus failed. \n"));
  }
  if (pthread_rwlock_init(&Sndp_RWLockCmd,NULL) != EOK)
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitAudioChannel: initialize Snd_RWLockCmd failed. \n"));
  }
  /*1. initialize private variables */
  Sndp_SetStatus(SNDP_IDLE);

  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockCmd))
  {
    memset(&Sndp_Cmd, 0, sizeof(Sndp_Cmd));
    Sndp_Cmd.Cmd = Sndp_CMD_NONE;
    Sndp_Cmd.AudioId = 0;
    pthread_rwlock_unlock(&Sndp_RWLockCmd);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitAudioChannel: get Snd_RWLockCmd WR lock failed. \n"));
  }

  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockStatus))
  {
    memset(&Sndp_Status, 0, sizeof(Sndp_Status));
    Sndp_Status.Status = SNDP_IDLE;
    Sndp_Status.Volume = 60;
    pthread_rwlock_unlock(&Sndp_RWLockStatus);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitAudioChannel: get Snd_RWLockStatus WR lock failed. \n"));
  }

  /*open audio configuration file.*/
  fd = open(SNDP_AUDIO_CONFIG_FILE, O_RDONLY);
  if(-1 != fd)
  {
    lseek(fd, 0L, SEEK_SET);
    SndpAudioXmlBufferSize = lseek(fd, 0L, SEEK_END);
    if (SndpAudioXmlBufferSize > 0)
    {
      Sndp_AudioXmlBuffer = mmap(0, SndpAudioXmlBufferSize, PROT_READ,
          MAP_PRIVATE, fd, 0);
      if (MAP_FAILED == Sndp_AudioXmlBuffer)
      {
        ret = -1;
        SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
          "Sndp_InitAudioChannel: mmap failed.\n"));
      }
    }
    close(fd);
  }
  else
  {
    ret = -1;
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitAudioChannel: open %s failed.\n",SNDP_AUDIO_CONFIG_FILE));
  }
  return ret;
}
/******************************************************************************/
/*Name : Sndp_SetStatus                                                       */
/*Role : set task status                                                      */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                                                     */
/*OD                                                                          */
/******************************************************************************/
static void Sndp_SetStatus(SNDP_STATUS status)
{
  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockStatus))
  {
    Sndp_Status.Status = status;
    pthread_rwlock_unlock(&Sndp_RWLockStatus);
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_SetStatus: get WR lock failed.\n"));
  }

}
/******************************************************************************/
/*Name : Sndp_ClearSem                                                        */
/*Role :clear semphora                                                        */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                                                     */
/*OD                                                                          */
/******************************************************************************/
static void Sndp_ClearSem(void)
{
  int value = 0;
  int i = 0;

  if(0 == sem_getvalue(&Sndp_Sem, &value))
  {
    for(i = 0;i < value;i++)
    {
      sem_wait(&Sndp_Sem);
    }
    sem_getvalue(&Sndp_Sem, &value);
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_ClearSem: sem_getvalue failed.\n"));
  }

}
/******************************************************************************/
/*Name : Sndp_ClearCmd                                                        */
/*Role :clear command flag                                                    */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                                                     */
/*OD                                                                          */
/******************************************************************************/
static void Sndp_ClearCmd(void)
{
  UINT8 newCmdId = 0;

  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockCmd))
  {
    Sndp_Cmd.CmdFlag = 0;
    newCmdId = Sndp_Cmd.CmdId;
    pthread_rwlock_unlock(&Sndp_RWLockCmd);
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_ClearCmd: get WR lock failed.\n"));
  }

  if (EOK == pthread_rwlock_wrlock(&Sndp_RWLockStatus))
  {
    Sndp_Status.CmdId = newCmdId;
    pthread_rwlock_unlock(&Sndp_RWLockStatus);
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_ClearCmd: get WR lock failed.\n"));
  }

}

/******************************************************************************/
/*Name : Sndp_ReceiveNewCmd                                                   */
/*Role :check if received new command                                         */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     []                                                                     */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_CheckNewCmd(void)
{
  int ret = -1;
  int cmdFlag = 0;

  if (EOK == pthread_rwlock_rdlock(&Sndp_RWLockCmd))
  {
    cmdFlag = Sndp_Cmd.CmdFlag;
    pthread_rwlock_unlock(&Sndp_RWLockCmd);
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_ClearCmd: get cmd RD lock failed.\n"));
  }

  return cmdFlag;
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
        min(AudioInfo->DataSize - AudioInfo->PlayedSize, Sndp_BlockSize),
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

  return 0;
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
static int Sndp_InitSndMixer(void)
{
  snd_pcm_channel_setup_t setup = { 0 };
  int volume = 0;
  float volumeLevel = 0;
  int rtn = -1;
  int ret = -1;

  /*get volume*/
  if (EOK == pthread_rwlock_rdlock(&Sndp_RWLockStatus))
  {
    volumeLevel = Sndp_Status.Volume;
    pthread_rwlock_unlock(&Sndp_RWLockStatus);
  }
  else
  {
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
      "Sndp_InitSndMixer:Get RD lock failed.\n"));
  }

  /*get block size and data size*/
  memset(&setup, 0, sizeof(setup));
  memset(&SndpGroup, 0, sizeof(SndpGroup));
  setup.channel = SND_PCM_CHANNEL_PLAYBACK;
  setup.mixer_gid = &SndpGroup.gid;
  rtn = snd_pcm_plugin_setup(SndpPcm_handle, &setup);
  if (EOK == rtn)
  {
    Sndp_BlockSize = setup.buf.block.frag_size;

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

        rtn = snd_mixer_file_descriptor(SndpMixer_handle);
        if(rtn > 0)
        {
          ret = 0;
          Snd_SndMixer.MixerFd = rtn;
          /*set volume*/
          if (volumeLevel >= 0)
          {
             rtn = snd_mixer_group_read(SndpMixer_handle, &SndpGroup);
            if (0 == rtn)
            {
              volume = (float) (SndpGroup.max - SndpGroup.min)
                       * (volumeLevel / 100);

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

  return ret;
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
        {          fread(&wavHdr1, sizeof(wavHdr1), 1, AudioInfo->Fp);
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
      if (0 == Sndp_InitSndMixer())
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
      //system("timestamp before");
      //system("timestamp before");
      //print_time("before_init");
      rtn = Sndp_AudioInit(&AudioInfo);
      //print_time("after_init ");
      if (0 == rtn)
      {
        while (Sndp_TaskRunningFlag && (AudioInfo.PlayedSize < AudioInfo.DataSize)
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
                  SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
                    "Sndp_PlayAudio: find data tag failed.\n"));
                  break;
                }
              }
              else
              {
                ret = Sndp_ERROR_INPUT;
                SYST_DEBUG(TRUE, SYST_Printf(_SLOG_ERROR,
                  "Sndp_PlayAudio: check file header failed.\n"));
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
          if (Sndp_CheckNewCmd())
          {
            printf("###### breaking out.\n");
            ret = Sndp_OK;
            break;
          }
        }

        if (Sndp_CheckNewCmd())
        {
          Sndp_ClearCmd();
        }
        else
        {
          /*this call would block until all data in driver queue is playe out the channel*/
          printf("###### snd_pcm_plugin_flush.\n");
          snd_pcm_plugin_flush(SndpPcm_handle, SND_PCM_CHANNEL_PLAYBACK);
        }
        Sndp_AudioRelease(&AudioInfo);
      }
      else
      {
        ret = Sndp_ERROR_AUDIO;
        Sndp_AudioRelease(&AudioInfo);
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

/******************************************************************************/
/*Name : Sndp_MonitorIoAudio                                                  */
/*Role :check io-audio driver status                                          */
/*Interface : none                                                            */
/*Pre-condition : none                                                        */
/*Constraints :                                                               */
/*Behaviour :                                                                 */
/*DO                                                                          */
/*     [play the audio by music ID]                                           */
/*OD                                                                          */
/******************************************************************************/
static int Sndp_MonitorIoAudio(void)
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
    SYST_DEBUG(TRUE, SYST_Printf(_SLOG_WARNING,"device open fails \n"));
    ret = SndpErr("device open");
  }
  return ret;
}
/*______ E N D _____ (Sndp.c) ________________________________________________*/
