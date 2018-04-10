/*****************************************************************************
  FileName:     ProcessComm.h                                                 
  Author:       林锴                                                          
  Version:      1.0                                                           
  Date:         2016/5/9 15:52:48                                             
  Description:  进程间通信的通用库定义,基于消息队列实现                          
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
#define PC_MSG_DATA_LEN         1024    /*消息队列数据长度*/

#define PC_MSG_TYPE_NONE        0x00    /*一般消息类型*/
#define PC_MSG_TYPE_REQ         0x01    /*请求消息类型*/
#define PC_MSG_TYPE_RPLY        0x10    /*应答消息类型*/


/*用于消息队列通信的共同结构*/
typedef struct
{
    Uint8  reserved[3];                  /* 预留 */
    Uint8  subId;                        /* 进程内自编子对象 */
    Uint8  data[PC_MSG_DATA_LEN];        /* 数据 */
}S_PcMsgData;

typedef struct 
{
    Int32        processId;		        /* 进程自编号 */
    S_PcMsgData sPcMsgData;             /* 消息队列的数据部分 */
}S_PcMsgCommon;


/*用于消息队列通信的数据消息头*/
typedef struct 
{
	Uint32 magicNum;                    /*指定为0xAABBCCDD*/
	Uint32 type;                        /*消息类型*/
	Uint32 cmd;                         /*消息命令*/
	Uint32 sn;                          /*消息流水号号*/
	Uint32 msgLen;                      /*消息总长度（包括消息头）*/
	Uint32 reserved;                    /*保留位*/
}S_PcMsgHead;

typedef struct
{
    int     msgqId;                     /*消息队列的ID*/
    int     processId;                  /*进程的自编号ID*/
    int     syncFlag;                   /*使用描述符同步的标志*/    
    int     syncFd[2];                  /*将消息队列转换描述符同步的管道描述符*/
    sem_t   syncSem;                    /*同步信号量*/
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