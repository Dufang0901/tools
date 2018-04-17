/******************************************************************************/
/* @F_NAME:           sndp.h                                                  */
/* @F_PURPOSE:        Sound                                                   */
/* @F_CREATED_BY:     zhao.gao                                                */
/* @F_CREATION_DATE:  08/02/2017                                              */
/* @F_MODIFIED_BY:                                                            */
/* @F_MODIFY_DATE:                                                            */
/* @F_LANGUAGE :      C                                                       */
/* @F_MPROC_TYPE:     uC Independent                                          */
/************************************** (C) Copyright 2017 Magneti Marelli ****/

#ifndef SNDP_H
#define SNDP_H

#include "type.h"
typedef unsigned char UINT8;

typedef enum{
	SNDP_IDLE,
	SNDP_STOP,
	SNDP_BUSY,
	SNDP_ERROR
}SNDP_STATUS;

void *SNDP_Task_ts(void *);
int SNDP_PlayAudioByID(unsigned int musicID,unsigned int times, UINT8 cmdId);
void SNDP_WakeUp(void);
void SNDP_Sleep(void);
void SNDP_Init(void);
void SNDP_SetAudioVolum(float level);
int SNDP_GetPlayStatus(SNDP_STATUS *status, int *audioId, UINT8 *cmdId);
int SNDP_StopAudio(UINT8 cmdId);

#endif /* SNDP_H */

/* _____ E N D _____ (sndp.h) ________________________________________________*/
