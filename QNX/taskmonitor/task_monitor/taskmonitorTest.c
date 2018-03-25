/******************************************************************************/
/* @F_NAME:           main.c                                             */
/* @F_PURPOSE:                                                                */
/* @F_CREATED_BY:       jun.ma                                                  */
/* @F_CREATION_DATE: May 10, 2017                                               */
/* @F_LANGUAGE:      C                                                        */
/* @F_MPROC_TYPE:    processor independent                                    */
/************************************** (C) Copyright 2016 Magneti Marelli ****/

#ifdef __USAGE


#endif

/*______ I N C L U D E - F I L E S ___________________________________________*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <screen/screen.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <dirent.h>
#include <img/img.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <io/io.h>
#include "taskmonitor.h"
#include <semaphore.h>
#include <sys/procmgr.h>
#include <stdint.h>
#include <mqueue.h>


#define SSCS_Task_Id         0
#define WKSS_Task_Id         1
#define EEPC_Task_Id         2
#define EEPM_Task_Id         3
#define IMCM_PLA_Task_Id         4
#define IMCM_Task_Id         5
#define VERS_Task_Id         6
#define IMC2DIAG_Task_Id         7
#define APSM_Task_Id         8
#define IODC_Task_Id         9
#define HMIH_Task_Id         10
#define DIMM_Task_Id         11
#define SNDP_Task_Id         12
#define NAVI_Task_Id         13
#define NVGM_Task_Id         14

#define TASK_MONITOR_CYCLE   1000     /*unit is millisecond*/
#define TASK_MONITOR_TIME    3
#define TASK_MONITOR_DEVINFO_CYCLE   60 * 60  
#define TASK_NUMBER          15
#define TASK_MONITOR_LOG_DIR  "/appfs/taskmonitor/"
#define TASK_MONITOR_LOG_FILE "/appfs/taskmonitor/taskmonitorlog"
#define TASK_MONITOR_DEVINFO_FILE "/appfs/taskmonitor/devinfo"


/*----------------------------------------------------------------------------*/
/*Name : main                                                                 */
/*Role : driver entry                                                         */
/*Interface :                                                                 */
/*  - IN  : -                                                                 */
/*  - OUT : void                                                              */
/*Pre-condition : -                                                           */
/*Constraints   : -                                                           */
/*Behavior :                                                                  */
/*  DO                                                                        */
/*    [Return the work mode]                                                  */
/*  OD                                                                        */
/*----------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    int i = 0;
    int fd = 0;
    task_monitor_t taskMonitor;
    
	while (1)
	{
		sleep(1);
        fd = open(DISP_NODE,O_RDWR);
        if(fd <= 0)
        {
            printf("open %s failed.\n",DISP_NODE);
            return -1;
        }

        for(i = 0;i < TASK_NUMBER;i++)
        {
            taskMonitor.taskID = i;
            devctl(fd,DCMD_TASK_MONITOR_HB,&taskMonitor,sizeof(taskMonitor),NULL);
        }
        close(fd);
        
	}
	return 1;
}





