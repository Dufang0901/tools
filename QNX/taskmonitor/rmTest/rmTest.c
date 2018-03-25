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

static void print_help()
{
	printf("*********************************************\n");
	printf("** USAGE: rmTest cmd \n");
	printf("**        cmd: 1(session)  - set boot session as client.\n");
	printf("**        cmd: 2(notify)   - shutdown notify.\n");
	printf("**        cmd: 3(status)   - get shutdown status.\n");
	printf("**        cmd: 4(off)      - power off.\n");
	printf("**        cmd: 5(shutdown) - shutdown.\n");
	printf("*********************************************\n");
}

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
int main(int argc, char *argv[])
{
    int i = 0;
    int fd = 0;
    int ret = -1;
    Monitor_BootSession_t sesstion = Monitor_BOOT_SESSION_CLIENT;
	int status = -1;
	
	if(argc != 2)
	{
		print_help();
		return -1;
	}
	else
	{
		fd = open(T_MONITOR_NODE,O_RDWR);
        if(fd > 0)
		{
			if((0 == strcmp(argv[1],"1")) || (0 == strcmp(argv[1],"session")))
			{
				/*set session*/
				ret = devctl(fd, DCMD_RESOURCE_MONITOR_SESSION, &sesstion, sizeof(sesstion), NULL);
				if(ret)
				{
				    printf("rmTest: set session to CLIENT failed, returns %d\n",ret);
				}
				else
				{
					printf("rmTest: set session to CLIENT successfully.\n");
				}
				
				
				
			}
			else if((0 == strcmp(argv[1],"2")) || (0 == strcmp(argv[1],"notify")))
			{
				/*notify shutdown*/
				int value = 0;
				ret = devctl(fd, DCMD_RESOURCE_MONITOR_SHUTDOWN_NOTIFY, &value, sizeof(value), NULL);
				if(ret)
				{
				    printf("rmTest: notify shutdown failed, returns %d\n",ret);
				}
				else
				{
					printf("rmTest: notify shutdown successfully.\n");
				}
				
			}
			else if((0 == strcmp(argv[1],"3")) || (0 == strcmp(argv[1],"status")))
			{
				/*get status*/
				ret = devctl(fd, DCMD_RESOURCE_MONITOR_STATUS, &status, sizeof(status), NULL);
				if(ret)
				{
				    printf("rmTest: get status failed, returns %d\n",ret);
				}
				else
				{
					printf("rmTest: get status successfully, status is %d.\n",status);
				}
			}
			else if((0 == strcmp(argv[1],"4")) || (0 == strcmp(argv[1],"off")))
			{
				/*power off*/
				Monitor_PowerOff_t pwOff;
				
				pwOff.taskID = Monitor_ImagesDisplay_Task_Id;
				pwOff.delaytime = 100;
				
				ret = devctl(fd, DCMD_RESOURCE_MONITOR_POWEROFF, &pwOff, sizeof(pwOff), NULL);
				if(ret)
				{
				    printf("rmTest: power off failed, returns %d\n",ret);
				}
				else
				{
					printf("rmTest: power off successfully.\n");
				}
				
			}
			else if((0 == strcmp(argv[1],"5")) || (0 == strcmp(argv[1],"shutdown")))
			{
				/*shutdown*/
				int value = 0;
				ret = devctl(fd, DCMD_RESOURCE_MONITOR_SHUTDOWN, &value, sizeof(value), NULL);
				if(ret)
				{
				    printf("rmTest: shutdown failed, returns %d\n",ret);
				}
				else
				{
					printf("rmTest: shutdown successfully.\n");
				}
			}
			
			close(fd);
		}
		else
		{
			printf("xxxxxxxxxx open %s failed.\n",T_MONITOR_NODE);
		}
	
	}
	
	return 1;
}





