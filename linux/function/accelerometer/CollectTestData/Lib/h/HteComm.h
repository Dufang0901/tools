/*****************************************************************************
  FileName:     HteComm.h
  Author:       林锴
  Version:      1.0
  Date:         2016/9/26 15:42:03
  Description:  基于Hte协议通信的通用库定义
  History:
                <author>    <time>    <version>    <desc>
                 linca    2016.09.26     V1.0      create
*****************************************************************************/

#ifndef _ROUTALK_COMM_H_
#define _ROUTALK_COMM_H_

#include "OsPack.h"
#include "OsCommon.h"

#define HC_CM_OK                1
#define HC_CM_ERROR_UNKNOW      0
#define HC_CM_ERROR_MSGLEN      -1
#define HC_CM_ERROR_CHC         -2
#define HC_CM_ERROR_NULLPTR     -3
#define HC_CM_ERROR_SENDTO      -4
#define HC_CM_ERROR_TIMEOUT     -5


/*消息头*/
#define HTE_MSG_HEAD0               'H'                 /*消息头0*/
#define HTE_MSG_HEAD1               'T'                 /*消息头1*/

/*消息类型*/
#define HTE_MSG_TYPE_NONE           0x00                /*一般消息*/
#define HTE_MSG_TYPE_REQ            0x01                /*请求消息*/
#define HTE_MSG_TYPE_RPLY           0x02                /*应答消息*/

/*设备编码*/
#define HTE_MSG_DEV_UNKNOW          0x00                /*设备未知*/
#define HTE_MSG_DEV_BIC             0x01                /*车载信息中心*/
#define HTE_MSG_DEV_OBU             0x02                /*车载单元*/
#define HTE_MSG_DEV_BGB             0x03                /*车载导盲终端*/
#define HTE_MSG_DEV_PAD1            0x04                /*第一代车载屏*/
#define HTE_MSG_DEV_PAD2            0x05                /*第二代车载屏*/
#define HTE_MSG_DEV_KB              0x06                /*车载键盘*/
#define HTE_MSG_DEV_LES             0x60                /*低端电子站牌*/
#define HTE_MSG_DEV_HES             0x61                /*高端电子站牌*/
#define HTE_MSG_DEV_IES             0x62                /*智能站亭*/
#define HTE_MSG_DEV_RN              0x90                /*路侧节点*/
#define HTE_MSG_DEV_CBLE            0xE0                /*通用蓝牙模块*/

#define HTE_MSG_HEAD_LEN            sizeof(S_HteMsgHead)

/*消息头结构*/
typedef struct
{
    Uint8 head[2];                  /*消息头*/
    Uint8 srcDevType;               /*源设备类型*/
    Uint8 srcDevId[3];              /*源设备ID*/
    Uint8 dstDevType;               /*目的设备类型*/
    Uint8 dstDevId[3];              /*目的设备ID*/
    Uint8 sn[4];                    /*流水号*/
    Uint8 type;                     /*消息类型*/
    Uint8 cmd;                      /*命令*/
    Uint8 msgLen[2];                /*消息长度*/
    Uint8 crc[2];                   /*校验值*/
}S_HteMsgHead;

Uint16 HteMsgCrc(Uint8 *pInMsg, Uint16 inMsgLen);
Int32  HteBuildMsgHead(Uint8 *pInMsg,Uint8 inSrcDev,Uint32 inSrcId,Uint8 inDstDev,Uint32 inDstId,Uint32 inMsgSn,Uint8 inMsgType,Uint8 inMsgCmd);
Int32  HteBuildMsg(Uint8 *pInMsg,Uint16 *pInMsgLen,Uint8 *pInData,Uint16 inDataLen);
Int32  HteUdpSendMsg(int inFd,Uint8 *pInMsg,Uint16 inMsgLen,struct sockaddr *psInDestAddr);
Int32  HteUdpRecvMsg(int inFd,Uint8 *pInMsg,Uint16 *pInMsgLen,Uint16 inMaxMsgLen,struct sockaddr *psInSrcAddr);
Int32  HteUnixSendMsg(int inFd,Uint8 *pInMsg,Uint16 inMsgLen,struct sockaddr *psInDestAddr);
Int32  HteUnixRecvMsg(int inFd,Uint8 *pInMsg,Uint16 *pInMsgLen,Uint16 inMaxMsgLen,struct sockaddr *psInSrcAddr);
Int32  HteSteamSendMsg(int inFd,Uint8 *pInMsg,Uint16 inMsgLen);
Int32  HteSteamRecvOnePacket(int inFd,Uint8 *pInPacketMsg,Uint16 *pInPacketLen,Uint16 inMaxMsgLen);
Int32  HteMsgVerify(Uint8 *pInMsg, Uint16 inMsgLen);

#endif