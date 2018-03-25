/******************************************************************************/
/* @F_NAME :          taskmonitor.h                                           */
/* @F_PURPOSE :       safety monitor GPU task                                 */
/* @F_CREATED_BY :    Ruan Haynes                                             */
/* @F_CREATION_DATE : 2017/12/12                                              */
/* @F_LANGUAGE :      C                                                       */
/* @F_MPROC_TYPE :    target independent                                      */
/*************************************** (C) Copyright 2017 Magneti Marelli ***/

#ifndef __TASKMONITOR_H__
#define __TASKMONITOR_H__

/*______ I N C L U D E - F I L E S ___________________________________________*/
#include <devctl.h>
/*______ G L O B A L - D E F I N E S _________________________________________*/

#define Monitor_SSCS_Task_Id                    0
#define Monitor_WKSS_Task_Id                    1
#define Monitor_EEFM_Task_Id                    2
#define Monitor_IMCM_PLA_Task_Id                3
#define Monitor_IMCM_Task_Id                    4
#define Monitor_VERS_Task_Id                    5
#define Monitor_IMC2DIAG_Task_Id                6
#define Monitor_APSM_Task_Id                    7
#define Monitor_IODC_Task_Id                    8
#define Monitor_HMIH_Task_Id                    9
#define Monitor_SNDP_Task_Id                    10
#define Monitor_DIMM_Task_Id                    11
#define Monitor_SAFM_Task_Id                    12
#define Monitor_IMPM_Task_Id                    13
#define Monitor_IMPM_RecvCommand_Task_Id        14
#define Monitor_IMPM_RecvMap_Task_Id            15
#define Monitor_NAVI_Task_Id                    16
#define Monitor_NVGM_Task_Id                    17
#define Monitor_MAIN_Task_Id                    18
#define Monitor_GPMM_Task_Id                    19
#define Monitor_KANZI_Task_Id                   24
#define Monitor_DWLM_Task_Id                    25
#define Monitor_SAFETY_QUOS_Task_Id             26
#define Monitor_SAFETY_DIS_Task_Id              27
#define Monitor_SAFETY_CRC_Task_Id              28
#define Monitor_SAFETY_IO_Task_Id               29
#define Monitor_ImagesDisplay_Task_Id           30
#define Monitor_HAM_task_Id                     31
#define Monitor_TASK_MAX_Id                     32

#define SAFM_WDG_MQ_SEND_PATH                   "/MONITOR_MQ_SEND"
#define SAFM_WDG_MQ_RECV_PATH                   "/MONITOR_MQ_RECV"
#define T_MONITOR_NODE                          "/dev/threadMonitor"

#define _DCMD_TASK_MONITOR                      _DCMD_MISC
#define DCMD_TASK_MONITOR_HB                    __DIOT (_DCMD_TASK_MONITOR, 0x1, Monitor_Task_t)
#define	DCMD_IMG_MONITOR_START_BUFFER           __DIOT (_DCMD_TASK_MONITOR, 0x2, Monitor_Task_t)
#define	DCMD_IMG_MONITOR_STOP                   __DIOT (_DCMD_TASK_MONITOR, 0x3, Monitor_Task_Flag_t)
#define DCMD_RESOURCE_MONITOR_SHUTDOWN_NOTIFY   __DION (_DCMD_TASK_MONITOR, 0x4)
#define DCMD_RESOURCE_MONITOR_STATUS            __DIOTF (_DCMD_TASK_MONITOR, 0x5, int)
#define	DCMD_RESOURCE_MONITOR_SHUTDOWN          __DION (_DCMD_TASK_MONITOR, 0x6)
#define	DCMD_RESOURCE_MONITOR_POWEROFF          __DIOT (_DCMD_TASK_MONITOR, 0x7,Monitor_PowerOff_t)
#define	DCMD_RESOURCE_MONITOR_SESSION           __DIOT (_DCMD_TASK_MONITOR, 0x8,Monitor_BootSession_t)

/*______ G L O B A L - T Y P E S _____________________________________________*/
typedef enum
{
	Monitor_BOOT_SESSION_NONE,
	Monitor_BOOT_SESSION_CLIENT,
	Monitor_BOOT_SESSION_FLASHER,
	Monitor_BOOT_SESSION_EOL
}Monitor_BootSession_t;

typedef enum
{
	Monitor_SHUTDOWN_NOT_READY,
	Monitor_SHUTDOWN_READY
}Monitor_ShutDownReady_t;

typedef struct {
    int taskID;
    int monitor_flag;
}Monitor_Task_Flag_t;

typedef struct {
    int taskID;
    int timeout;
}Monitor_Task_t;

typedef struct {
    int taskID;
    int delaytime;
}Monitor_PowerOff_t;


/*______ G L O B A L - D A T A _______________________________________________*/

/*______ G L O B A L - M A C R O S ___________________________________________*/
/*______ G L O B A L - F U N C T I O N S - P R O T O T Y P E S _______________*/
#endif /* TASKMONITOR_H_ */



