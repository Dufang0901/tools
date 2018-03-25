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


static int TaskStatus[TASK_NUMBER];
static resmgr_connect_funcs_t Imgdisp_ConnectFuncs;
static resmgr_io_funcs_t Imgdisp_IoFuncs;
static iofunc_attr_t Imgdisp_IoFuncAttr;


/*----------------------------------------------------------------------------*/
/*Name : Imgdisp_TimeStamp                                                    */
/*Role : print time, used for debug and test                                  */
/*Interface :                                                                 */
/*   IN :                                                                     */
/*  OUT : -                                                                   */
/*Pre-condition : -                                                           */
/*Constraints : -                                                             */
/*Behavior :                                                                  */
/*  DO                                                                        */
/*    [ ]                                                                     */
/*  OD                                                                        */
/*----------------------------------------------------------------------------*/
static void Imgdisp_TimeStamp(const char *info)
{
	struct timespec nowTime;
	struct tm nowLocalTime;
	int ms;

	if( clock_gettime( CLOCK_REALTIME, &nowTime) == -1 ) {
		perror( "clock gettime" );
		return;
	}

	localtime_r(&nowTime.tv_sec,&nowLocalTime);
	ms = nowTime.tv_nsec / 1000000;

#if 0
	 printf("################# %s :%02d:%02d:%02d,%d ###################\n",info
			 ,nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec,ms);
#endif

}
static void TaskMonitor_ErrorRecord(int taskId)
{
    FILE *fp = NULL;
    struct timespec nowTime;
	struct tm nowLocalTime;
    
    fp = fopen(TASK_MONITOR_LOG_FILE,"a");
    if(NULL == fp)
    {
        printf("TaskMonitor_ErrorRecord: open %s failed.\n",TASK_MONITOR_LOG_FILE);
    }
    else
    {
    	if( clock_gettime( CLOCK_REALTIME, &nowTime) == -1 ) {
    		perror( "clock gettime" );
    	}

    	localtime_r(&nowTime.tv_sec,&nowLocalTime);
        switch (taskId)
        {
            case SSCS_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: SSCS_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);

            }
             break;
            case WKSS_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: WKSS_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);

            }
             break;
            case EEPC_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: EEPC_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);

            }
             break;
            case EEPM_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: EEPM_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);

            }
             break;
            case IMCM_PLA_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: IMCM_PLA_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);


            }
             break;
            case IMCM_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: IMCM_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);

            }
             break;
            case VERS_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: VERS_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case IMC2DIAG_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: IMC2DIAG_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case APSM_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: APSM_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case IODC_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: IODC_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case HMIH_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: HMIH_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case DIMM_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: DIMM_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case SNDP_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: SNDP_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case NAVI_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: NAVI_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            case NVGM_Task_Id:
            {
                fprintf(fp,"time %02d:%02d:%02d: NVGM_Task is dead\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);
            }
             break;
            default:
                break;

        }
        fclose(fp);
                
    }

}
static void TaskMonitor_DevinfoRecord()
{
    static cnt = 0;
    FILE *fp_log = NULL;
    FILE *fp_info = NULL;
    char lineStr[128];
    struct timespec nowTime;
	struct tm nowLocalTime;
    int ret;
    

    if( clock_gettime( CLOCK_REALTIME, &nowTime) == -1 ) 
    {
		perror( "clock gettime" );
	}

    localtime_r(&nowTime.tv_sec,&nowLocalTime);
    
    cnt++;
    if(cnt > TASK_MONITOR_DEVINFO_CYCLE)
    {
        cnt = 0;
        fp_info = fopen("/dev/ti/devmon","r");
        if(NULL == fp_info)
        {
            printf("TaskMonitor_DevinfoRecord: open /dev/ti/devmon failed.\n");
        }
        else
        {
            fp_log = fopen(TASK_MONITOR_DEVINFO_FILE,"a");
            if(NULL == fp_log)
            {
                printf("TaskMonitor_DevinfoRecord: open %s failed.\n",TASK_MONITOR_DEVINFO_FILE);
            }
            else
            {
                fprintf(fp_log,"time %02d:%02d:%02d:\r\n",nowLocalTime.tm_hour,nowLocalTime.tm_min,nowLocalTime.tm_sec);   
                while(!feof(fp_info))
                {
                    bzero(lineStr,sizeof(lineStr));
                    if(fgets(lineStr,sizeof(lineStr) - 1, fp_info) != NULL)
                    {
                        fprintf(fp_log,"  %s",lineStr);

                    }
                    else
                    {
                        printf("lineStr is null.\n");
                    }
                }
                fclose(fp_log);
            }
            
            fclose(fp_info);
        }
        
        
    }
    system("sync");
    system("sync");
    

}
static long  TaskMonitor_GetPidByName(char* task_name)
{
    #define BUF_SIZE  50
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	FILE *fp = NULL;
	char filepath[BUF_SIZE] = {0};
	char buf[BUF_SIZE] = {0};
	unsigned long  pid = -1;
	size_t len = 0;

	if(NULL != task_name)
	{
		dir = opendir("/proc");
		if (NULL != dir)
		{
			/*search the directorys*/
			while((ptr=readdir(dir))!=NULL)
			{
				/*skip the . and ..*/
				if((strcmp(ptr->d_name,".")==0)||(strcmp(ptr->d_name,"..")==0))
				continue;
				/*get the file path*/
				sprintf(filepath,"/proc/%s/cmdline",ptr->d_name);
				fp = fopen(filepath, "r");
				if (NULL != fp)
				{
					len = fread(buf,1, BUF_SIZE-1, fp);
					/*if (0 != memcmp(task_name, buf,len - 1))*/
					if (NULL == strstr(buf,task_name))
					{
						/*Not find ,close file*/
						fclose(fp);
						continue;
					}
					else
					{
						pid = atoi(ptr->d_name);
					}
						/*close file*/
					fclose(fp);
				}
			}
			/*close directory*/
			closedir(dir);
		}
	}
	return pid;
}

static void TaskMonitor_StartDevmon()
{
    long pid = -1;
    pid = TaskMonitor_GetPidByName("devmon");
    if(pid <= 0)
    {
        /*"devmon" is not running, needs to be started.*/
        system("devmon");
    }

}
static void TaskMonitor_init()
{
    int i = 0;
    FILE *fp = NULL;

    /*reset all task counter*/
    for(i = 0;i < TASK_NUMBER;i++)
    {
        TaskStatus[i] = 0;
    }

    /*create task monitor log file*/
    if(access(TASK_MONITOR_LOG_DIR,F_OK) != 0)
    {
        if(0 == mkdir(TASK_MONITOR_LOG_DIR,S_IRUSR | S_IWUSR))
        {
            fp = fopen(TASK_MONITOR_LOG_FILE,"w");
            if(NULL == fp)
            {
                printf("TaskMonitor_init: create %s failed.\n",TASK_MONITOR_LOG_FILE);
            }
            else
            {
                fclose(fp);
            }

            fp = fopen(TASK_MONITOR_DEVINFO_FILE,"w");
            if(NULL == fp)
            {
                printf("TaskMonitor_init: create %s failed.\n",TASK_MONITOR_DEVINFO_FILE);
            }
            else
            {
                fclose(fp);
            }

        }
        
    }

}
/*----------------------------------------------------------------------------*/
/*Name : Imgdisp_DriverThread                                                 */
/*Role : image display loop thread                                            */
/*Interface :                                                                 */
/*   IN :                                                                     */
/*  OUT : -                                                                   */
/*Pre-condition : -                                                           */
/*Constraints : -                                                             */
/*Behavior :                                                                  */
/*  DO                                                                        */
/*    [ ]                                                                     */
/*  OD                                                                        */
/*----------------------------------------------------------------------------*/
static void* TaskMonitor_DriverThread(void *data)
{
    int i = 0;
    int shutdownFlag = 0;

    TaskMonitor_init();
    TaskMonitor_StartDevmon();
    while(1)
    {
        usleep(TASK_MONITOR_CYCLE * 1000);
        TaskMonitor_DevinfoRecord();
        for(i = 0;i < TASK_NUMBER;i++)
        {
            TaskStatus[i]++;
            if(TaskStatus[i] > TASK_MONITOR_TIME)
            {
                shutdownFlag = 1;
                TaskMonitor_ErrorRecord(i);
            }
        }
        if(shutdownFlag)
        {
            system("sync");
            system("sync");
            system("shutdown");

        }
    }
}
/*----------------------------------------------------------------------------*/
/*Name : Imgdisp_Devctl                                                 */
/*Role : dev control cmds handle funtion                   */
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
static int TaskMonitor_Devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
	int			status;
    int         taskId;

	status = iofunc_devctl_default(ctp, msg, ocb);
	if (status != _RESMGR_DEFAULT)
		return status;
	switch (msg->i.dcmd)
	{
		case DCMD_TASK_MONITOR_HB:
		{
			task_monitor_t	*taskMonitor;
			if (msg->i.nbytes < sizeof(task_monitor_t))
				return EINVAL;
			taskMonitor    = (task_monitor_t *)((uint8_t *)_DEVCTL_DATA(msg->i));

			taskId = taskMonitor->taskID;

            if(taskId >= 0 && taskId < TASK_NUMBER)
            {
                printf("task id is %d\n",taskId);
                TaskStatus[taskId] = 0;
            }
#if 0
            switch(taskId)
            {
                case SSCS_Task_Id:
                {
                    taskStatus.SSCS_Task_ts = 0;
                }
                 break;
                case WKSS_Task_Id:
                {
                    taskStatus.WKSS_Task_ts = 0;
                }
                 break;
                case EEPC_Task_Id:
                {
                    taskStatus.EEPC_Task_ts = 0;
                }
                 break;
                case EEPM_Task_Id:
                {
                    taskStatus.EEPM_Task_ts = 0;
                }
                 break;
                case IMCM_PLA_Task_Id:
                {
                    taskStatus.IMCM_PLA_Task_ts = 0;

                }
                 break;
                case IMCM_Task_Id:
                {
                    taskStatus.IMCM_Task_ts = 0;

                }
                 break;
                case VERS_Task_Id:
                {
                    taskStatus.VERS_Task_ts = 0;

                }
                 break;
                case IMC2DIAG_Task_Id:
                {
                    taskStatus.IMC2DIAG_Task_ts = 0;

                }
                 break;
                case APSM_Task_Id:
                {
                    taskStatus.APSM_Task_ts = 0;

                }
                 break;
                case IODC_Task_Id:
                {
                    taskStatus.IODC_Task_ts = 0;

                }
                 break;
                case HMIH_Task_Id:
                {
                    taskStatus.HMIH_Task_ts = 0;

                }
                 break;
                case DIMM_Task_Id:
                {
                    taskStatus.DIMM_Task_ts = 0;

                }
                 break;
                case SNDP_Task_Id:
                {
                    taskStatus.SNDP_Task_ts = 0;

                }
                 break;
                case NAVI_Task_Id:
                {
                    taskStatus.NAVI_Task_ts = 0;

                }
                 break;
                case NVGM_Task_Id:
                {
                    taskStatus.NVGM_Task_ts = 0;

                }
                 break;
                default:
                    break;

            }
#endif  
            
			memset(&msg->o, 0, sizeof(msg->o));
			msg->o.ret_val = 0;
			msg->o.nbytes = 0;
			return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o));
		}
		break;
		case DCMD_IMG_DISPLAY_START_BUFFER:
		{
			memset(&msg->o, 0, sizeof(msg->o));
			msg->o.ret_val = 0;
			msg->o.nbytes = 0;
			return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o));
		}
		break;
		case DCMD_IMG_DISPLAY_STOP:
		{
			memset(&msg->o, 0, sizeof(msg->o));
			msg->o.ret_val = 0;
			msg->o.nbytes = 0;
			return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o));
		}
		break;
		default:
			break;
	}

	return ENOSYS;
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
int main(int argc, char **argv)
{
	dispatch_t *dpp;
	resmgr_attr_t resmgr_attr;
	resmgr_context_t *ctp;
	int id;
    pthread_attr_t      pattr;
    struct sched_param  param;
    pthread_t pid;


	 /*Enable IO capability.*/
	if (-1 == ThreadCtl( _NTO_TCTL_IO, 0 )) {
		return EXIT_FAILURE;
	}
	procmgr_ability(0,PROCMGR_AID_EOL | PROCMGR_AID_INTERRUPT |PROCMGR_AID_IO );

	/*run in the background as daemon thread*/
	if ( procmgr_daemon( EXIT_SUCCESS, PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL ) == -1 ) {
		return EXIT_FAILURE;
	}
    
	/* initialize dispatch interface */
	if ( (dpp = dispatch_create()) == NULL )
	{
		fprintf( stderr, "%s: Unable to allocate dispatch handle.\n", argv[0] );
		return EXIT_FAILURE;
	}
    
	/* initialize resource manager attributes */
	memset( &resmgr_attr, 0, sizeof resmgr_attr );
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;
    
	/* initialize functions for handling messages */
	iofunc_func_init( _RESMGR_CONNECT_NFUNCS, &Imgdisp_ConnectFuncs, _RESMGR_IO_NFUNCS, &Imgdisp_IoFuncs );
	Imgdisp_IoFuncs.devctl = TaskMonitor_Devctl;
    
	/* initialize attribute structure */
	iofunc_attr_init( &Imgdisp_IoFuncAttr, S_IFNAM | 0666, 0, 0 );
    
	/* register device name */
	/* attach our device name (passing in the POSIX defaults
	from the iofunc_func_init and iofunc_attr_init functions)
	*/
	if ( (id = resmgr_attach( dpp, &resmgr_attr, DISP_NODE, _FTYPE_ANY, 0, &Imgdisp_ConnectFuncs, &Imgdisp_IoFuncs, &Imgdisp_IoFuncAttr)) == -1 )
	{
		fprintf( stderr, "%s: Unable to attach name.\n", argv[0] );
		return EXIT_FAILURE;
	}
    
	/* allocate a context structure */
	ctp = resmgr_context_alloc( dpp );

    /*start SW watchdog thread*/
    pthread_attr_init(&pattr);
	pthread_attr_setschedpolicy(&pattr, SCHED_RR);
	param.sched_priority = 21;
	pthread_attr_setschedparam(&pattr, &param);
	pthread_attr_setinheritsched(&pattr, PTHREAD_EXPLICIT_SCHED);

	// Create thread for this interface
	if (pthread_create(&pid, &pattr, (void *)TaskMonitor_DriverThread, NULL) != EOK) {
		perror("pthread_create() failed");
		return EINVAL;
	}
    
	/* start the resource manager message loop */
	while (1)
	{
		if ( (ctp = resmgr_block( ctp )) == NULL )
		{
			fprintf(stderr, "block error\n");
			return EXIT_FAILURE;
		}
		resmgr_handler(ctp);
	}
	return EXIT_FAILURE;
}





