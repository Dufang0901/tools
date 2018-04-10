/*****************************************************************************
  FileName:     HteComm.h
  Author:       ����
  Version:      1.0
  Date:         2016/9/26 15:42:03
  Description:  ����HteЭ��ͨ�ŵ�ͨ�ÿⶨ��
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


/*��Ϣͷ*/
#define HTE_MSG_HEAD0               'H'                 /*��Ϣͷ0*/
#define HTE_MSG_HEAD1               'T'                 /*��Ϣͷ1*/

/*��Ϣ����*/
#define HTE_MSG_TYPE_NONE           0x00                /*һ����Ϣ*/
#define HTE_MSG_TYPE_REQ            0x01                /*������Ϣ*/
#define HTE_MSG_TYPE_RPLY           0x02                /*Ӧ����Ϣ*/

/*�豸����*/
#define HTE_MSG_DEV_UNKNOW          0x00                /*�豸δ֪*/
#define HTE_MSG_DEV_BIC             0x01                /*������Ϣ����*/
#define HTE_MSG_DEV_OBU             0x02                /*���ص�Ԫ*/
#define HTE_MSG_DEV_BGB             0x03                /*���ص�ä�ն�*/
#define HTE_MSG_DEV_PAD1            0x04                /*��һ��������*/
#define HTE_MSG_DEV_PAD2            0x05                /*�ڶ���������*/
#define HTE_MSG_DEV_KB              0x06                /*���ؼ���*/
#define HTE_MSG_DEV_LES             0x60                /*�Ͷ˵���վ��*/
#define HTE_MSG_DEV_HES             0x61                /*�߶˵���վ��*/
#define HTE_MSG_DEV_IES             0x62                /*����վͤ*/
#define HTE_MSG_DEV_RN              0x90                /*·��ڵ�*/
#define HTE_MSG_DEV_CBLE            0xE0                /*ͨ������ģ��*/

#define HTE_MSG_HEAD_LEN            sizeof(S_HteMsgHead)

/*��Ϣͷ�ṹ*/
typedef struct
{
    Uint8 head[2];                  /*��Ϣͷ*/
    Uint8 srcDevType;               /*Դ�豸����*/
    Uint8 srcDevId[3];              /*Դ�豸ID*/
    Uint8 dstDevType;               /*Ŀ���豸����*/
    Uint8 dstDevId[3];              /*Ŀ���豸ID*/
    Uint8 sn[4];                    /*��ˮ��*/
    Uint8 type;                     /*��Ϣ����*/
    Uint8 cmd;                      /*����*/
    Uint8 msgLen[2];                /*��Ϣ����*/
    Uint8 crc[2];                   /*У��ֵ*/
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