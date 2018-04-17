/*****************************************************************************
  FileName:     MobCommRun.h
  Author:       ����
  Version:      1.0
  Date:         2016/3/9 17:20:21
  Description:  �ƶ�ͨ�ŵ�Ӧ��
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.03.09    V1.0       create
*****************************************************************************/

#ifndef _MOB_COMM_RUN_H_
#define _MOB_COMM_RUN_H_


#include "OsPack.h"
#include "OsCommon.h"

/*MC�Ĵ�����*/
#define MC_OK               1       //�ɹ�
#define MC_ERROR_UNKNOW     0       //ʧ�� - ԭ��δ֪
#define MC_ERROR_NO_MEM     -1      //ʧ�� - �ڴ�ռ����
#define MC_ERROR_ROCK       -2      //ʧ�� - ��д���쳣
#define MC_ERROR_THREAD     -3      //ʧ�� - �߳�ʧ��
#define MC_ERROR_FD         -4      //ʧ�� - �ļ�������
#define MC_ERROR_FILE       -5      //ʧ�� - �ļ������쳣
#define MC_ERROR_NO_INIT    -6      //ʧ�� - δ��ʼ��
#define MC_ERROR_PARAM      -7      //ʧ�� - ��������
#define MC_ERROR_NO_SUPPORT -8      //ʧ�� - ��֧��


typedef Uint32      McModuleId;

typedef Int32       (*McModuleInitFunc) (void);             /*ģ���ʼ����������*/
typedef Int32       (*McModuleStartFunc) (void);            /*ģ��������������*/
typedef Int32       (*McModuleReleaseFunc) (void);          /*ģ���ͷź�������*/
typedef Int32       (*McModuleHardRstFunc) (void);          /*ģ��Ӳ����λ��������*/
typedef void        (*McCallStatCgHandle) (Int32 mode,Int32 stat,Char *pCallNum);                  /*ͨ��״̬���*/
typedef void        (*McSigCgHandler) (Int32 sigLeave,Int32 sig,Int32 ber);             /*�ź�״̬���*/
typedef void        (*McStatCgHandler) (Int32 stat);                                    /*ģ��״̬���*/
typedef void        (*McOpeRegCgHandler) (Int32 stat);                                  /*��Ӫ��ע��״̬���*/
typedef void        (*McNwTypeCgHandler) (Int32 type);                                  /*����ģʽ���*/
typedef void        (*McErrorEvtFunc) (Uint32 mcId,Int32 error);                        /*ģ���쳣����*/

/*�ƶ�ͨ�Žڵ��״̬*/
typedef enum
{
    E_MC_STAT_FREE,                                 /*����*/
    E_MC_STAT_ALLOATED                              /*ʹ����*/
}E_MC_ALLOC_STAT;

/*�ƶ�ͨ�Žڵ��ģ��*/
typedef enum
{
    E_MC_MODULE_M35,                                /*M35ģ��*/
    E_MC_MODULE_U7500,                              /*U7500��ģ��*/
    E_MC_MODULE_U8300,                              /*U8300��ģ��*/
    E_MC_MODULE_NONE                                /*����ģ��*/
}E_MC_MODULE;

/*�ƶ�ͨ��ģ��Ĺ���ģʽ*/
typedef enum
{
    E_MC_START_ONLY,                                /*��ģ����*/
    E_MC_START_COMPLICATE,                          /*��������*/
    E_MC_START_DUAL_MASTER,                         /*˫ģ���豸*/
    E_MC_START_DUAL_SLAVE                           /*˫ģ���豸*/
}E_MC_START_MODE;


/*�ƶ�ͨ��ģ�������״̬*/
typedef enum
{
    E_MC_STAT_HARDRST       =   3,                  /*����״̬ - ģ��Ͽ����Ӻ����Ӳ����λ��*/
    E_MC_STAT_HOLDON        =   2,                  /*����״̬ - ����*/
    E_MC_STAT_RUNING        =   1,                  /*����״̬ - ������*/
    E_MC_STAT_INIT          =   0,                  /*����״̬ - ��ʼ��*/
    E_MC_STAT_ABNOR_DEV     =   -1,                 /*����״̬ - �豸�쳣*/
    E_MC_STAT_ABNOR_COM     =   -2,                 /*����״̬ - �����豸�쳣*/
    E_MC_STAT_ABNOR_AT      =   -3,                 /*����״̬ - ATָ���շ��쳣*/
    E_MC_STAT_ABNOR_SIM     =   -4,                 /*����״̬ - SIM���쳣*/
    E_MC_STAT_ABNOR_REG     =   -5,                 /*����״̬ - ע����Ӫ��ʧ��*/
    E_MC_STAT_ABNOR_TIMER   =   -6                  /*����״̬ - ��ʱ���쳣*/
}E_MC_RUN_STAT;

/*��Ӫ��ע��״̬*/
typedef enum
{
    E_MC_OPE_REG_LOSE       =   0,                  /*��Ӫ��״̬ - ��ʧ*/
    E_MC_OPE_REG_OK         =   1                   /*��Ӫ��״̬ - ע��ɹ�*/
}E_MC_OPE_REG_STAT;

/*����״̬*/
typedef enum
{
    E_MC_NWTYPE_UNKNOW      =  0,                   /*����״̬ - δ֪*/
    E_MC_NWTYPE_NOSERVICE   =  1,                   /*����״̬ - �޷���*/
    E_MC_NWTYPE_GSM         =  2,                   /*����״̬ - GSM*/
    E_MC_NWTYPE_GPRS        =  3,                   /*����״̬ - GPRS*/
    E_MC_NWTYPE_EDGE        =  4,                   /*����״̬ - EDGE*/
    E_MC_NWTYPE_WCDMA       =  5,                   /*����״̬ - WCDMA*/
    E_MC_NWTYPE_HSDPA       =  6,                   /*����״̬ - HSDPA*/
    E_MC_NWTYPE_HSUPA       =  7,                   /*����״̬ - HSUPA*/
    E_MC_NWTYPE_HSDPAPLUS   =  8,                   /*����״̬ - HSDPAPLUS*/
    E_MC_NWTYPE_TDSCDMA     =  9,                   /*����״̬ - TDSCDMA*/
    E_MC_NWTYPE_LTEFDD      =  10,                  /*����״̬ - LTEFDD*/
    E_MC_NWTYPE_LTETDD      =  11                   /*����״̬ - LTETDD*/
}E_MC_NETWORK_TYPE;


/*�ƶ�ͨ��ģ��Ĳ���״̬*/
typedef enum
{
    E_MC_PPPD_RE            =   4,                  /*����״̬ - �ز�����*/
    E_MC_PPPD_DISC          =   3,                  /*����״̬ - �ж�*/
    E_MC_PPPD_ING           =   2,                  /*����״̬ - ������*/
    E_MC_PPPD_CONN          =   1,                  /*����״̬ - ����������*/
    E_MC_PPPD_WAIT          =   0,                  /*����״̬ - �ȴ�����*/
    E_MC_PPPD_ABNOR         =   -1,                 /*����״̬ - �����쳣*/
}E_MC_PPPD_STAT;

/*�ƶ�ͨ��ģ���ͨ��״̬*/
typedef enum
{
    E_MC_CALL_IDLE          =  0,                   /*ͨ��״̬ - ����*/
    E_MC_CALL_ING           =  1,                   /*ͨ��״̬ - ���ں���*/
    E_MC_CALL_RING          =  2,                   /*ͨ��״̬ - ����*/
    E_MC_CALL_CONN          =  3                    /*ͨ��״̬ - ����״̬*/
}E_MC_CALL_STAT;


/*�ƶ�ͨ��ģ��ĺ���ģʽ*/
typedef enum
{
    E_MC_CALL_MODE_NONE     = 0,                    /*ͨ��ģʽ - δͨ��*/
    E_MC_CALL_MODE_ACTIVE   = 1,                    /*ͨ��ģʽ - ��������*/
    E_MC_CALL_MODE_PASSIVE  = 2                     /*ͨ��ģʽ - ��������*/
}E_MC_CALL_MODE;


/*�ƶ�ͨ�ŵ�ģ�����ò���*/
typedef struct
{
    E_MC_MODULE             eMcModule;              /*ģ���ͺ�*/
    McModuleInitFunc        FuncModuleInit;         /*ģ���ʼ������-���ڳ�ʼ��Ϊԭʼ״̬�ĺ���*/
    McModuleStartFunc       FuncModuleStart;        /*ģ����������-��������ģ�鹤���ĺ���*/
    McModuleReleaseFunc     FuncModuleRelease;      /*ģ���ͷź���*/
    McModuleHardRstFunc     FuncModuleHardRst;      /*ģ��Ӳ����λ����*/
    McSigCgHandler          HandlerSigCg;           /*ģ���źű���ص����*/
    McStatCgHandler         HandlerRunStatCg;       /*ģ������״̬�ı�ص����*/
    McOpeRegCgHandler       HandleOpeRegStatCg;     /*ģ����Ӫ��ע��״̬�ı�ص����*/
    McStatCgHandler         HandlerPppStatCg;       /*ģ�鲦��״̬�ı�ص����*/
    McNwTypeCgHandler       HandlerNwTypeCg;        /*ģ������״̬�ı�ص����*/
    
    McCallStatCgHandle      HandlerCallStatCg;      /*ģ��ͨ��״̬�ı�ص����*/
    
    Int32                   pppdPersist;            /*�����Զ�����*/
    Int32                   pppdDefaultRoute;       /*Ĭ������*/
    Int32                   pppdUnit;               /*���ŵ�Ԫ*/
    Int32                   pppdDebug;              /*���ŵ��Կ���*/
    Int32                   pppdLcpEchoFaiure;      /*LCP��ʧ�ܴ���*/
    Int32                   pppdLcpEchoInterval;    /*LCP�ķ��ͼ��*/
    Int32                   nicDiscTime;            /*�������ߵ�ʱ��*/
    Char                    *pppdLogFile;           /*���ŵ�LOG�ļ�*/
    Char                    *pppdIpScript;          /*���ŵ�ip-up��ip-down�Ľű��ļ�*/
    Char                    *atDev;                 /*�����շ�ATָ���豸�� - ����ģ��ʹ��*/
    Char                    *pppdDev;               /*���ڲ��ŵ��豸�� - ����ģ��ʹ��*/
    Char                    *pppdApn;               /*����APN*/
    Char                    *pppdCallNum;           /*���ź���*/
    Char                    *pingIp;                /*���״̬��PING��IP*/
}S_McCfgPar;

/*�ƶ�ͨ�ŵ�ģ����������*/
typedef struct
{
    E_MC_START_MODE         eMcStartMode;
    McModuleId              pairMcId;
}S_McStartPar;

/*�ƶ�ͨ��ģ��Ľڵ����*/
typedef struct
{
    E_MC_ALLOC_STAT         eAllocStat;             /*����״̬*/
    E_MC_MODULE             eMcModule;              /*ģ���ͺ�*/
    
    McErrorEvtFunc          FuncMcErrorEvtCb;       /*�쳣�˳��Ļص�����*/
    
    McModuleInitFunc        FuncModuleInit;         /*ģ���ʼ������-���ڳ�ʼ��Ϊԭʼ״̬�ĺ���*/
    McModuleStartFunc       FuncModuleStart;        /*ģ����������-��������ģ�鹤���ĺ���*/
    McModuleReleaseFunc     FuncModuleRelease;      /*ģ���ͷź���*/
    McModuleHardRstFunc     FuncModuleHardRst;      /*ģ��Ӳ����λ����*/
    McSigCgHandler          HandlerSigCg;           /*ģ���źű���ص����*/
    McStatCgHandler         HandlerRunStatCg;       /*ģ������״̬�ı�ص����*/
    McOpeRegCgHandler       HandleOpeRegStatCg;     /*ģ����Ӫ��ע��״̬�ı�ص����*/
    McStatCgHandler         HandlerPppStatCg;       /*ģ�鲦��״̬�ı�ص����*/
    McNwTypeCgHandler       HandlerNwTypeCg;        /*ģ������״̬�ı�ص����*/
    McCallStatCgHandle      HandlerCallStatCg;      /*ģ��ͨ��״̬�ı�ص����*/
    E_MC_RUN_STAT           eMcRunStat;             /*ģ������״̬*/
    E_MC_OPE_REG_STAT       eMcRegStat;             /*ע��״̬*/
    E_MC_PPPD_STAT          eMcPppdStat;            /*����״̬*/
    E_MC_NETWORK_TYPE       eMcNetworkType;         /*��������*/
    E_MC_START_MODE         eMcStartMode;           /*ģ��������ģʽ*/
    
    /*ģ�������״̬*/
    Int32                   sigLevel;               /*�źŵȼ�*/
    Int32                   sigPower;               /*�ź�ǿ��*/
    Int32                   sigBer;                 /*�ź�������*/
    Char                    atDev[16];              /*�����շ�ATָ���豸��*/
    Char                    pppdDev[16];            /*���ڲ��ŵ��豸��*/
    Char                    simImsi[32];            /*SIM��Imsi��*/
    Char                    simCid[32];             /*SIM����CID*/
    Char                    simOperator[32];        /*SIM������Ӫ��*/
    
    /*���Ų�����״̬*/
    Int32                   pppdUnit;               /*���ŵ�Ԫ*/
    Int32                   pppdDefaultRoute;       /*Ĭ������*/
    Int32                   pppdPersist;            /*�����Զ�����*/
    Int32                   pppdDebug;              /*���ŵ��Կ���*/
    Int32                   pppdLcpEchoFaiure;      /*LCP��ʧ�ܴ���*/
    Int32                   pppdLcpEchoInterval;    /*LCP�ķ��ͼ��*/
    Char                    pppdLogFile[128];       /*���ŵ�LOG�ļ�*/
    Char                    pppdIpScript[128];      /*���ŵ�ip-up��ip-down�Ľű��ļ�*/
    Char                    pppdNic[8];             /*��������*/
    Char                    pppdApn[32];            /*����APN*/
    Char                    pppdCallNum[32];        /*���ź���*/
    Char                    pppdIp[16];             /*PPPD��IP*/
    Char                    pingIp[16];				/*���״̬��PING��IP*/
    Int32                   nicDiscTime;            /*�������ߵ�ʱ��*/
    
    /*ͨ���Ĳ�����״̬*/
    Int32                   callFlag;               /*���б�־*/
    Int32                   answerFlag;             /*������־*/
    Int32                   overCallFlag;           /*�������б�־*/
    Int32                   callListenFlag;         /*���Ž����������ñ�־*/
    Int32                   callListenVol;          /*���Ž�������*/
    Int32                   callSayMuteFlag;        /*����˵���������ñ�־*/
    Int32                   callSayMuteStat;        /*����˵����������״̬*/
    E_MC_CALL_STAT          eCallStat;               /*����״̬*/
    Char                    callNum[32];            /*���еĺ���*/
    
    /*���ϵ����*/
    Int32                   rePowerFlag;            /*���ϵ��־λ*/
    
    pthread_rwlock_t        mcThredRwLock;          /*�̶߳�д��*/
    pthread_t               mcThreadId;             /*�߳�ID*/
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
