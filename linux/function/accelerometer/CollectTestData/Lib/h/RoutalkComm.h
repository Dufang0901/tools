/*****************************************************************************
  FileName:     RoutalkComm.h
  Author:       林锴
  Version:      1.0
  Date:         2016/5/11 11:39:18
  Description:  基于routalk协议通信的通用库定义
  History:
                <author>    <time>    <version>    <desc>
                 linca    2016.05.11     V1.0      create
*****************************************************************************/

#ifndef _ROUTALK_COMM_H_
#define _ROUTALK_COMM_H_

#include "OsPack.h"
#include "OsCommon.h"

#define RC_CM_OK                1
#define RC_CM_ERROR_UNKNOW      0
#define RC_CM_ERROR_MSGLEN      -1
#define RC_CM_ERROR_CRC         -2
#define RC_CM_ERROR_NULLPTR     -3
#define RC_CM_ERROR_SENDTO      -4


#define ROUTALK_MSG_LEN_MAX     512
#define ROUTALK_HEAD_LEN        sizeof(S_RoutalkHead)

typedef struct
{
    Uint8    srcDevId[3];                           //起始设备编码
    Uint8    destDevId[3];                          //目的设备编码
    Uint8    sn[4];                                 //流水号
    Uint8    len[2];                                //帧长度
    Uint8    crc[2];                                //CRC校验
    Uint8    cmd;                                   //帧类型
}S_RoutalkHead;

Uint16 RoutalkCrcCalc(Uint8 *pMsg, Uint16 msgLen);
Int32  RoutalkBuildHead(Uint8 *pMsg,Uint8 cmd,Uint32 sn,Uint32 srcDevId,Uint32 destDevId);
Int32  RoutalkBuildMsg(Uint8 *pMsg,Uint16 *pMsgLen,Uint8 needRply,Uint8 *pData,Uint16 dataLen);
Int32  RoutalkSendMsg(int fd,Uint8 *pMsg,Uint16 msgLen,struct sockaddr *destAddr);
Int32  RoutalkRecvMsg(int fd,Uint8 *pMsg,Uint16 *pMsgLen,struct sockaddr *srcAddr);
Int32  RoutalkMsgVerify(Uint8 *pMsg, Uint16 msgLen);

#endif