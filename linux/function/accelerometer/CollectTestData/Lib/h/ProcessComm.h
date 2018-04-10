/*****************************************************************************
  FileName:     ProcessComm.h                                                 
  Author:       ����                                                          
  Version:      1.0                                                           
  Date:         2016/5/9 15:52:48                                             
  Description:  ���̼�ͨ�ŵ�ͨ�ÿⶨ��,������Ϣ����ʵ��                          
  History:                                                                    
                <author>    <time>    <version>    <desc>                     
                 linca    2016.05.09     V1.0      create                     
*****************************************************************************/

#ifndef _PROCESS_COMM_H_
#define _PROCESS_COMM_H_

#include "OsPack.h"
#include "OsCommon.h"

#define PC_CM_OK               1
#define PC_CM_ERROR_UNKNOW     0
#define PC_CM_ERRPR_MSGQ       -1
#define PC_CM_ERROR_PIPE       -2
#define PC_CM_ERROR_SEM        -3
#define PC_CM_ERROR_THREAD     -4
#define PC_CM_ERROR_NO_MEM     -5
#define PC_CM_ERROR_MSGLEN     -6
#define PC_CM_ERROR_NULLPTR    -7

#define PC_MSG_MAGIC            0xAABBCCDD

#define PC_MSG_RS_LEN           sizeof(S_PcMsgData)
#define PC_MSG_DATA_LEN         1024    /*��Ϣ�������ݳ���*/

#define PC_MSG_TYPE_NONE        0x00    /*һ����Ϣ����*/
#define PC_MSG_TYPE_REQ         0x01    /*������Ϣ����*/
#define PC_MSG_TYPE_RPLY        0x10    /*Ӧ����Ϣ����*/


/*������Ϣ����ͨ�ŵĹ�ͬ�ṹ*/
typedef struct
{
    Uint8  reserved[3];                  /* Ԥ�� */
    Uint8  subId;                        /* �������Ա��Ӷ��� */
    Uint8  data[PC_MSG_DATA_LEN];        /* ���� */
}S_PcMsgData;

typedef struct 
{
    Int32        processId;		        /* �����Ա�� */
    S_PcMsgData sPcMsgData;             /* ��Ϣ���е����ݲ��� */
}S_PcMsgCommon;


/*������Ϣ����ͨ�ŵ�������Ϣͷ*/
typedef struct 
{
	Uint32 magicNum;                    /*ָ��Ϊ0xAABBCCDD*/
	Uint32 type;                        /*��Ϣ����*/
	Uint32 cmd;                         /*��Ϣ����*/
	Uint32 sn;                          /*��Ϣ��ˮ�ź�*/
	Uint32 msgLen;                      /*��Ϣ�ܳ��ȣ�������Ϣͷ��*/
	Uint32 reserved;                    /*����λ*/
}S_PcMsgHead;

typedef struct
{
    int     msgqId;                     /*��Ϣ���е�ID*/
    int     processId;                  /*���̵��Ա��ID*/
    int     syncFlag;                   /*ʹ��������ͬ���ı�־*/    
    int     syncFd[2];                  /*����Ϣ����ת��������ͬ���Ĺܵ�������*/
    sem_t   syncSem;                    /*ͬ���ź���*/
    pthread_t sysThreadId;
    S_PcMsgCommon *psPcMsgSend;
    S_PcMsgCommon *psPcMsgRecv;
}S_ProcessMsgqPar;

void PcBdNormalHead(S_PcMsgHead *sPcMsgHeadReq,Uint32 sn,Uint32 cmd,Uint32 len);
void PcBdReqHead(S_PcMsgHead *sPcMsgHeadReq,Uint32 sn,Uint32 cmd,Uint32 len);
void PcBdRplyHead(S_PcMsgHead *sPcMsgHeadRply,Uint32 sn,Uint32 cmd,Uint32 len);

Int32 ProcessMsgqCreate(key_t key,int fdSyncFlag,Int32 processId,S_ProcessMsgqPar *psProcessMsgqPar);
Int32 ProcessMsgqGet(key_t key,int *pMsgqId);
Int32 ProcessMsgqDelete(S_ProcessMsgqPar *psProcessMsgqPar);
Int32 ProcessMsgqRecv(S_ProcessMsgqPar *psPcMsgqPar,int *pSrcProcessId,S_PcMsgHead *pcMsgHead,void *pMsg);
Int32 ProcessMsgqSend(S_ProcessMsgqPar *psPcMsgqPar,int pDestMsgId,S_PcMsgHead *pcMsgHead,void *pMsg);

#endif