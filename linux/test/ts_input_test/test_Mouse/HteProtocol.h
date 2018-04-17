/*****************************************************************************
  FileName：    HteProtocol.h
  Author:       林锴
  Version:      1.0
  Date:         2016/9/9 9:13:51
  Description:  华途嵌入式通用协议相关定义
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


/*************第二代车载屏-标准命令********************************/
#define PAD_CAN_CMD_HEART			0x71				/*心跳*/

#define PAD_CAN_CMD_START			0x01				/*设备启动*/
#define PAD_CAN_CMD_SELF			0x02				/*设备自检*/

#define PAD_CAN_CMD_KEY_PRESS		0x11				/*键盘按下*/
#define PAD_CAN_CMD_KEY_PRESS_LONG	0x12				/*键盘长按*/
#define PAD_CAN_CMD_RELEASE			0x13				/*键盘释放*/

#define PAD_CAN_CMD_TOUCH_PRESS		0x21				/*触摸屏按下*/
#define PAD_CAN_CMD_TOUCH_RELEASE	0x22				/*触摸屏释放*/
#define PAD_CAN_CMD_TOUCH_XY		0x23				/*触摸坐标信息*/


/*触摸坐标信息结构*/
typedef struct
{
    Uint16 valueX;		/*触摸屏X轴*/
    Uint16 valueY;		/*触摸屏Y轴*/
}S_PadCanTouchXY;



/*消息头结构*/
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
