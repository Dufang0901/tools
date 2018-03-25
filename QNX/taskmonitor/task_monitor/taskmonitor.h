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


typedef struct {
    int taskID;
}task_monitor_t;

#define DISP_NODE   "/dev/taskMonitor"
#define _DCMD_TASK_MONITOR				_DCMD_MISC

#define DCMD_TASK_MONITOR_HB   					    __DIOT (_DCMD_TASK_MONITOR, 0x1, task_monitor_t)
#define	DCMD_IMG_DISPLAY_START_BUFFER				__DIOT (_DCMD_TASK_MONITOR, 0x2, task_monitor_t)
#define	DCMD_IMG_DISPLAY_STOP						__DION (_DCMD_TASK_MONITOR, 0x3)


#endif /* IMAGESDISPLAY_H_ */
