#include <stdio.h>
#include <pthread.h>
#include "sndp.h"


#define PLAY_TIME         1
#define PLAY_INTERVAL_MS  0
#define PLAY_INTERVAL_S   1

typedef enum
{
  Sndp_OK,
  Sndp_ERROR_INPUT,
  Sndp_ERROR_AUDIO
}Sndp_RetValue_t;

static int audioIdSet[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,60};

int main(int argc, const char *argv)
{
  pthread_t pid;
  int audioCnt = 0;
  int i = 0;
  unsigned char cmdId = 0;
  float value = 20;
  
  SNDP_Init();
  pthread_create(&pid,NULL,SNDP_Task_ts,NULL);

  audioCnt = sizeof(audioIdSet) / sizeof(audioIdSet[0]);
  printf("############ audio Cnt is %d\n",audioCnt);

  for(i = 0;i < audioCnt;i++)
  {
    printf("======== audioId = %d\n",audioIdSet[i]);
    SNDP_PlayAudioByID(audioIdSet[i],PLAY_TIME,cmdId++);
    SNDP_SetAudioVolum(value);
    value += 5;
    if(PLAY_INTERVAL_MS)
    {
      usleep(PLAY_INTERVAL_MS * 1000);
    }
    else if(PLAY_INTERVAL_S)
    {
      sleep(PLAY_INTERVAL_S);
    }
    else
    {
      sleep(1);
    }
  }

  sleep(10);

  return 1;
}
