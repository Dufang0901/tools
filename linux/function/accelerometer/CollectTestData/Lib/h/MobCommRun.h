/*****************************************************************************
  FileName:     MobCommRun.h
  Author:       林锴
  Version:      1.0
  Date:         2016/3/9 17:20:21
  Description:  移动通信的应用
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.03.09    V1.0       create
*****************************************************************************/

#ifndef _MOB_COMM_RUN_H_
#define _MOB_COMM_RUN_H_


#include "OsPack.h"
#include "OsCommon.h"

/*MC的错误定义*/
#define MC_OK               1       //成功
#define MC_ERROR_UNKNOW     0       //失败 - 原因未知
#define MC_ERROR_NO_MEM     -1      //失败 - 内存空间错误
#define MC_ERROR_ROCK       -2      //失败 - 读写锁异常
#define MC_ERROR_THREAD     -3      //失败 - 线程失败
#define MC_ERROR_FD         -4      //失败 - 文件描述符
#define MC_ERROR_FILE       -5      //失败 - 文件操作异常
#define MC_ERROR_NO_INIT    -6      //失败 - 未初始化
#define MC_ERROR_PARAM      -7      //失败 - 参数错误
#define MC_ERROR_NO_SUPPORT -8      //失败 - 不支持


typedef Uint32      McModuleId;

typedef Int32       (*McModuleInitFunc) (void);             /*模块初始化函数类型*/
typedef Int32       (*McModuleStartFunc) (void);            /*模块启动函数类型*/
typedef Int32       (*McModuleReleaseFunc) (void);          /*模块释放函数类型*/
typedef Int32       (*McModuleHardRstFunc) (void);          /*模块硬件复位函数类型*/
typedef void        (*McCallStatCgHandle) (Int32 mode,Int32 stat,Char *pCallNum);                  /*通话状态变更*/
typedef void        (*McSigCgHandler) (Int32 sigLeave,Int32 sig,Int32 ber);             /*信号状态变更*/
typedef void        (*McStatCgHandler) (Int32 stat);                                    /*模块状态变更*/
typedef void        (*McOpeRegCgHandler) (Int32 stat);                                  /*运营商注册状态变更*/
typedef void        (*McNwTypeCgHandler) (Int32 type);                                  /*网络模式变更*/
typedef void        (*McErrorEvtFunc) (Uint32 mcId,Int32 error);                        /*模块异常错误*/

/*移动通信节点的状态*/
typedef enum
{
    E_MC_STAT_FREE,                                 /*空闲*/
    E_MC_STAT_ALLOATED                              /*使用中*/
}E_MC_ALLOC_STAT;

/*移动通信节点的模块*/
typedef enum
{
    E_MC_MODULE_M35,                                /*M35模块*/
    E_MC_MODULE_U7500,                              /*U7500口模块*/
    E_MC_MODULE_U8300,                              /*U8300口模块*/
    E_MC_MODULE_NONE                                /*其他模块*/
}E_MC_MODULE;

/*移动通信模块的工作模式*/
typedef enum
{
    E_MC_START_ONLY,                                /*单模运行*/
    E_MC_START_COMPLICATE,                          /*并发运行*/
    E_MC_START_DUAL_MASTER,                         /*双模主设备*/
    E_MC_START_DUAL_SLAVE                           /*双模从设备*/
}E_MC_START_MODE;


/*移动通信模块的运行状态*/
typedef enum
{
    E_MC_STAT_HARDRST       =   3,                  /*运行状态 - 模块断开连接后进行硬件复位中*/
    E_MC_STAT_HOLDON        =   2,                  /*运行状态 - 挂起*/
    E_MC_STAT_RUNING        =   1,                  /*运行状态 - 运行中*/
    E_MC_STAT_INIT          =   0,                  /*运行状态 - 初始化*/
    E_MC_STAT_ABNOR_DEV     =   -1,                 /*运行状态 - 设备异常*/
    E_MC_STAT_ABNOR_COM     =   -2,                 /*运行状态 - 串口设备异常*/
    E_MC_STAT_ABNOR_AT      =   -3,                 /*运行状态 - AT指令收发异常*/
    E_MC_STAT_ABNOR_SIM     =   -4,                 /*运行状态 - SIM卡异常*/
    E_MC_STAT_ABNOR_REG     =   -5,                 /*运行状态 - 注册运营商失败*/
    E_MC_STAT_ABNOR_TIMER   =   -6                  /*运行状态 - 定时器异常*/
}E_MC_RUN_STAT;

/*运营商注册状态*/
typedef enum
{
    E_MC_OPE_REG_LOSE       =   0,                  /*运营商状态 - 丢失*/
    E_MC_OPE_REG_OK         =   1                   /*运营商状态 - 注册成功*/
}E_MC_OPE_REG_STAT;

/*网络状态*/
typedef enum
{
    E_MC_NWTYPE_UNKNOW      =  0,                   /*网络状态 - 未知*/
    E_MC_NWTYPE_NOSERVICE   =  1,                   /*网络状态 - 无服务*/
    E_MC_NWTYPE_GSM         =  2,                   /*网络状态 - GSM*/
    E_MC_NWTYPE_GPRS        =  3,                   /*网络状态 - GPRS*/
    E_MC_NWTYPE_EDGE        =  4,                   /*网络状态 - EDGE*/
    E_MC_NWTYPE_WCDMA       =  5,                   /*网络状态 - WCDMA*/
    E_MC_NWTYPE_HSDPA       =  6,                   /*网络状态 - HSDPA*/
    E_MC_NWTYPE_HSUPA       =  7,                   /*网络状态 - HSUPA*/
    E_MC_NWTYPE_HSDPAPLUS   =  8,                   /*网络状态 - HSDPAPLUS*/
    E_MC_NWTYPE_TDSCDMA     =  9,                   /*网络状态 - TDSCDMA*/
    E_MC_NWTYPE_LTEFDD      =  10,                  /*网络状态 - LTEFDD*/
    E_MC_NWTYPE_LTETDD      =  11                   /*网络状态 - LTETDD*/
}E_MC_NETWORK_TYPE;


/*移动通信模块的拨号状态*/
typedef enum
{
    E_MC_PPPD_RE            =   4,                  /*拨号状态 - 重拨号中*/
    E_MC_PPPD_DISC          =   3,                  /*拨号状态 - 中断*/
    E_MC_PPPD_ING           =   2,                  /*拨号状态 - 拨号中*/
    E_MC_PPPD_CONN          =   1,                  /*拨号状态 - 拨号已连接*/
    E_MC_PPPD_WAIT          =   0,                  /*拨号状态 - 等待拨号*/
    E_MC_PPPD_ABNOR         =   -1,                 /*拨号状态 - 拨号异常*/
}E_MC_PPPD_STAT;

/*移动通信模块的通话状态*/
typedef enum
{
    E_MC_CALL_IDLE          =  0,                   /*通话状态 - 空闲*/
    E_MC_CALL_ING           =  1,                   /*通话状态 - 正在呼叫*/
    E_MC_CALL_RING          =  2,                   /*通话状态 - 响铃*/
    E_MC_CALL_CONN          =  3                    /*通话状态 - 连接状态*/
}E_MC_CALL_STAT;


/*移动通信模块的呼叫模式*/
typedef enum
{
    E_MC_CALL_MODE_NONE     = 0,                    /*通话模式 - 未通话*/
    E_MC_CALL_MODE_ACTIVE   = 1,                    /*通话模式 - 主动呼叫*/
    E_MC_CALL_MODE_PASSIVE  = 2                     /*通话模式 - 被动呼叫*/
}E_MC_CALL_MODE;


/*移动通信的模块配置参数*/
typedef struct
{
    E_MC_MODULE             eMcModule;              /*模块型号*/
    McModuleInitFunc        FuncModuleInit;         /*模块初始化函数-用于初始化为原始状态的函数*/
    McModuleStartFunc       FuncModuleStart;        /*模块启动函数-用于启动模块工作的函数*/
    McModuleReleaseFunc     FuncModuleRelease;      /*模块释放函数*/
    McModuleHardRstFunc     FuncModuleHardRst;      /*模块硬件复位函数*/
    McSigCgHandler          HandlerSigCg;           /*模块信号变更回调句柄*/
    McStatCgHandler         HandlerRunStatCg;       /*模块运行状态改变回调句柄*/
    McOpeRegCgHandler       HandleOpeRegStatCg;     /*模块运营商注册状态改变回调句柄*/
    McStatCgHandler         HandlerPppStatCg;       /*模块拨号状态改变回调句柄*/
    McNwTypeCgHandler       HandlerNwTypeCg;        /*模块网络状态改变回调句柄*/
    
    McCallStatCgHandle      HandlerCallStatCg;      /*模块通话状态改变回调句柄*/
    
    Int32                   pppdPersist;            /*拨号自动重连*/
    Int32                   pppdDefaultRoute;       /*默认网关*/
    Int32                   pppdUnit;               /*拨号单元*/
    Int32                   pppdDebug;              /*拨号调试开关*/
    Int32                   pppdLcpEchoFaiure;      /*LCP的失败次数*/
    Int32                   pppdLcpEchoInterval;    /*LCP的发送间隔*/
    Int32                   nicDiscTime;            /*网卡掉线的时间*/
    Char                    *pppdLogFile;           /*拨号的LOG文件*/
    Char                    *pppdIpScript;          /*拨号的ip-up和ip-down的脚本文件*/
    Char                    *atDev;                 /*用于收发AT指令设备名 - 部分模块使用*/
    Char                    *pppdDev;               /*用于拨号的设备名 - 部分模块使用*/
    Char                    *pppdApn;               /*拨号APN*/
    Char                    *pppdCallNum;           /*拨号号码*/
    Char                    *pingIp;                /*检测状态的PING的IP*/
}S_McCfgPar;

/*移动通信的模块启动参数*/
typedef struct
{
    E_MC_START_MODE         eMcStartMode;
    McModuleId              pairMcId;
}S_McStartPar;

/*移动通信模块的节点参数*/
typedef struct
{
    E_MC_ALLOC_STAT         eAllocStat;             /*分配状态*/
    E_MC_MODULE             eMcModule;              /*模块型号*/
    
    McErrorEvtFunc          FuncMcErrorEvtCb;       /*异常退出的回调函数*/
    
    McModuleInitFunc        FuncModuleInit;         /*模块初始化函数-用于初始化为原始状态的函数*/
    McModuleStartFunc       FuncModuleStart;        /*模块启动函数-用于启动模块工作的函数*/
    McModuleReleaseFunc     FuncModuleRelease;      /*模块释放函数*/
    McModuleHardRstFunc     FuncModuleHardRst;      /*模块硬件复位函数*/
    McSigCgHandler          HandlerSigCg;           /*模块信号变更回调句柄*/
    McStatCgHandler         HandlerRunStatCg;       /*模块运行状态改变回调句柄*/
    McOpeRegCgHandler       HandleOpeRegStatCg;     /*模块运营商注册状态改变回调句柄*/
    McStatCgHandler         HandlerPppStatCg;       /*模块拨号状态改变回调句柄*/
    McNwTypeCgHandler       HandlerNwTypeCg;        /*模块网络状态改变回调句柄*/
    McCallStatCgHandle      HandlerCallStatCg;      /*模块通话状态改变回调句柄*/
    E_MC_RUN_STAT           eMcRunStat;             /*模块运行状态*/
    E_MC_OPE_REG_STAT       eMcRegStat;             /*注册状态*/
    E_MC_PPPD_STAT          eMcPppdStat;            /*拨号状态*/
    E_MC_NETWORK_TYPE       eMcNetworkType;         /*网络类型*/
    E_MC_START_MODE         eMcStartMode;           /*模块启动的模式*/
    
    /*模块参数和状态*/
    Int32                   sigLevel;               /*信号等级*/
    Int32                   sigPower;               /*信号强度*/
    Int32                   sigBer;                 /*信号误码率*/
    Char                    atDev[16];              /*用于收发AT指令设备名*/
    Char                    pppdDev[16];            /*用于拨号的设备名*/
    Char                    simImsi[32];            /*SIM的Imsi号*/
    Char                    simCid[32];             /*SIM卡的CID*/
    Char                    simOperator[32];        /*SIM卡的运营商*/
    
    /*拨号参数和状态*/
    Int32                   pppdUnit;               /*拨号单元*/
    Int32                   pppdDefaultRoute;       /*默认网关*/
    Int32                   pppdPersist;            /*拨号自动重连*/
    Int32                   pppdDebug;              /*拨号调试开关*/
    Int32                   pppdLcpEchoFaiure;      /*LCP的失败次数*/
    Int32                   pppdLcpEchoInterval;    /*LCP的发送间隔*/
    Char                    pppdLogFile[128];       /*拨号的LOG文件*/
    Char                    pppdIpScript[128];      /*拨号的ip-up和ip-down的脚本文件*/
    Char                    pppdNic[8];             /*拨号网卡*/
    Char                    pppdApn[32];            /*拨号APN*/
    Char                    pppdCallNum[32];        /*拨号号码*/
    Char                    pppdIp[16];             /*PPPD的IP*/
    Char                    pingIp[16];				/*检测状态的PING的IP*/
    Int32                   nicDiscTime;            /*网卡掉线的时间*/
    
    /*通话的参数和状态*/
    Int32                   callFlag;               /*呼叫标志*/
    Int32                   answerFlag;             /*接听标志*/
    Int32                   overCallFlag;           /*结束呼叫标志*/
    Int32                   callListenFlag;         /*拨号接听音量设置标志*/
    Int32                   callListenVol;          /*拨号接听音量*/
    Int32                   callSayMuteFlag;        /*拨号说话静音设置标志*/
    Int32                   callSayMuteStat;        /*拨号说话静音设置状态*/
    E_MC_CALL_STAT          eCallStat;               /*呼叫状态*/
    Char                    callNum[32];            /*呼叫的号码*/
    
    /*重上电操作*/
    Int32                   rePowerFlag;            /*重上电标志位*/
    
    pthread_rwlock_t        mcThredRwLock;          /*线程读写锁*/
    pthread_t               mcThreadId;             /*线程ID*/
    pthread_t               mcRunThreadId;
    pthread_t               mcPppdThreadId;
}S_McNode;


#define MOB_COMM_INIT(MAX_MC_MODULE,MC_ERROR_FUNC)                              \
    do                                                                          \
    {                                                                           \
        static Uint8 mcBuf[sizeof(S_McNode) * MAX_MC_MODULE];                   \
        MobCommInit(MAX_MC_MODULE,mcBuf,MC_ERROR_FUNC);                         \
    }while(0);                                                                  \


Int32 McCmLibVersionGet(Char *pInVer);
Int32 MobCommInit(Uint8 inMaxMcModule,void *pInMcNodeBuf,McErrorEvtFunc inFuncMcErrorEvt);
Int32 MobCommCreate(McModuleId *pInMcId,S_McCfgPar *psInMcCfgPar);
Int32 MobCommDelete(McModuleId inMcId);
Int32 MobCommStart(McModuleId inMcId,S_McStartPar *psInMcStartPar);
Int32 MobCommCance(McModuleId inMcId);

Int32 MobCommCall(McModuleId inMcId,Char *pInCallNum);
Int32 MobCommAnswerCall(McModuleId inMcId);
Int32 MobCommOverCall(McModuleId inMcId);

Int32 MobCommCallListenVolSet(McModuleId inMcId,Int32 inVolume);
Int32 MobCommCallListenVolGet(McModuleId inMcId,Int32 *pInVolume);
Int32 MobCommCallSayMuteSet(McModuleId inMcId,Int32 inMuteStat);
Int32 MobCommCallSayMuteGet(McModuleId inMcId,Int32 *pInMuteStat);

Int32 MobCommSimImsiGet(McModuleId inMcId,Char *pInImsi);
Int32 MobCommSimIccidGet(McModuleId inMcId,Char *pInIccid);
Int32 MobCommSimOperatorGet(McModuleId inMcId,Char *pInOperator);
Int32 MobCommPppdIpGet(McModuleId inMcId,Char *pInPppdIp);

Int32 MobCommRePower(McModuleId inMcId);

#endif
