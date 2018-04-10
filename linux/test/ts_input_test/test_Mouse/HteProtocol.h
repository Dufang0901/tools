/*****************************************************************************
  FileName��    HteProtocol.h
  Author:       ����
  Version:      1.0
  Date:         2016/9/9 9:13:51
  Description:  ��;Ƕ��ʽͨ��Э����ض���
  History:
                <author>    <time>    <version>    <desc>
                 linca    2016.09.09     V1.0      create
*****************************************************************************/

#ifndef _HTE_PROTOCOL_H_
#define _HTE_PROTOCOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef unsigned char  Uint8;
typedef unsigned short Uint16;
typedef int            Int32;
typedef unsigned int   Uint32;

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


/*************�ڶ���������-��׼����********************************/
#define PAD_CAN_CMD_HEART			0x71				/*����*/

#define PAD_CAN_CMD_START			0x01				/*�豸����*/
#define PAD_CAN_CMD_SELF			0x02				/*�豸�Լ�*/

#define PAD_CAN_CMD_KEY_PRESS		0x11				/*���̰���*/
#define PAD_CAN_CMD_KEY_PRESS_LONG	0x12				/*���̳���*/
#define PAD_CAN_CMD_RELEASE			0x13				/*�����ͷ�*/

#define PAD_CAN_CMD_TOUCH_PRESS		0x21				/*����������*/
#define PAD_CAN_CMD_TOUCH_RELEASE	0x22				/*�������ͷ�*/
#define PAD_CAN_CMD_TOUCH_XY		0x23				/*����������Ϣ*/


/*����������Ϣ�ṹ*/
typedef struct
{
    Uint16 valueX;		/*������X��*/
    Uint16 valueY;		/*������Y��*/
}S_PadCanTouchXY;



/*��Ϣͷ�ṹ*/
typedef struct
{
    Uint8 head[2];
    Uint8 srcDevType;
    Uint8 srcDevId[3];
    Uint8 dstDevType;
    Uint8 dstDevId[3];
    Uint8 sn[4];
    Uint8 type;
    Uint8 cmd;
    Uint8 msgLen[2];
    Uint8 crc[2];
}S_HteMsgHead;

Uint16 HteMsgCrc(Uint8 *pInMsg, Uint16 inMsgLen);
void   HteBdMsgHead(Uint8 *pInMsg,Uint8 inSrcDev,Uint32 inSrcId,Uint8 inDstDev,Uint32 inDstId,Uint32 inMsgSn,Uint8 inMsgType,Uint8 inMsgCmd);
void   HteBdMsg(Uint8 *pInMsgHead,Uint8 *pInData,Uint16 inDataLen);
Int32  HteMsgRdByte(Uint8 *pInRdMsg,Uint16 inMsgMaxLen,Uint16 *pInRdLen,Uint8 inRdData);

#endif
