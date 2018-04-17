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
typedef enum{
	SNDP_IDLE,
	SNDP_STOP,
	SNDP_BUSY,
	SNDP_ERROR
}SNDP_STATUS;

int SNDP_PlayAudioByID(unsigned int musicID,unsigned int times);
void SNDP_WakeUp(void);
void SNDP_Sleep(void);
void SNDP_Init(void);
void SNDP_SetAudioVolum(int level);
SNDP_STATUS SNDP_GetPlayStatus(void);
int SNDP_StopAudio(void);



#endif /* SNDP_H */

/* _____ E N D _____ (sndp.h) ________________________________________________*/
