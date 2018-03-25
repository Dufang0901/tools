/*
 * imagesDisplay.h
 *
 *  Created on: 2017-5-10
 *      Author: f58962b
 */

#ifndef IMAGESDISPLAY_H_
#define IMAGESDISPLAY_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/resmgr.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <hw/inout.h>
#include <errno.h>
#include <devctl.h>

typedef enum
{
	IMGDISP_DECODER_TYPE_NONE,
	IMGDISP_DECODER_TYPE_HW,
	IMGDISP_DECODER_TYPE_SW
}imgdisp_DecoderType_t;

typedef struct {
	int  fps;
	int  posX;
	int  posY;
	char source[64];
	int  decoderType;
}img_display_t;

#define DISP_NODE   "/dev/imgdisp"
#define START_UP_FILE_PATH   "/appfs/StartUpFile/"
#define _DCMD_IMG_DISPLAY				_DCMD_MISC

#define DCMD_IMG_DISPLAY_START_FILE					__DIOT (_DCMD_IMG_DISPLAY, 0x1, img_display_t)
#define	DCMD_IMG_DISPLAY_START_BUFFER				__DIOT (_DCMD_IMG_DISPLAY, 0x2, img_display_t)
#define	DCMD_IMG_DISPLAY_STOP						__DION (_DCMD_IMG_DISPLAY, 0x3)
#define	DCMD_IMG_DISPLAY_STATUS						__DIOTF (_DCMD_IMG_DISPLAY, 0x4, int)

#define IMGDISP_STATUS_INIT                        0    /*program just launched*/
#define IMGDISP_STATUS_RUNNING                     1    /*animation playing*/
#define IMGDISP_STATUS_DONE                        2    /*animation played out, waiting.*/
#define IMGDISP_STATUS_STOP                        3    /*animation stopped*/


#endif /* IMAGESDISPLAY_H_ */
