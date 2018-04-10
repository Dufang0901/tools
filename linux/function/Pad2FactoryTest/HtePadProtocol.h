/*****************************************************************************  
FileName:     HtePadProtocol.h  
Author:       杜衡  
Version:      1.0  
Date:         2017/02/17 16:05:43  
Description:  第二代pad屏的HTE协议  
History:      <author>    <time>    <version>    <desc>                 
               duh      2017.02.17     V1.0      create
*****************************************************************************/
#ifndef _HTE_PAD_PROTOCOL_H_
#define _HTE_PAD_PROTOCOL_H_

#include "MyType.h"

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

#define HTE_MSG_HEAD_LEN            sizeof(S_HteMsgHead)/*消息头结构*/

/*协议帧类型定义*/
/*pad -> other device*/
#define HTE_PAD_HEARTBEAT           0X71        /*通用心跳*/
#define HTE_PAD_POWER_ON            0x01        /*设备启动*/
#define HTE_PAD_SELF_CHECK          0x02        /*设备自检*/
#define HTE_PAD_KEY_DOWN            0x11        /*键盘按下*/
#define HTE_PAD_KEY_HOLD            0x12        /*键盘长按*/
#define HTE_PAD_KEY_RELEASE         0x13        /*键盘释放*/
#define HTE_PAD_TS_DOWN             0x21        /*触摸屏按下*/
#define HTE_PAD_TS_RELEASE          0x22        /*触摸屏释放*/
#define HTE_PAD_TS_COORD            0x23        /*触摸屏坐标*/
#define HTE_PAD_MIC_STATE           0x41        /*手唛状态*/


/*other device -> pad*/
#define HTE_PAD_BL_SET              0x31        /*背光设置*/
#define HTE_PAD_MODE_SET            0x32        /*显示模式设置*/
#define HTE_PAD_RESTART             0x3A        /*设备重启*/
#define HTE_PAD_VERSION_QUERY       0x3B        /*设备版本查询*/
#define HTE_PAD_SCREEN_REVERSE      0x3E        /*显示屏翻转*/
#define HTE_PAD_BUZZER_CONTROL      0x35        /*蜂鸣器控制*/

/*协议帧结构体定义*/

/*HTE协议帧消息头*/
typedef struct{    
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

/*设备启动消息*/
typedef struct{
    Char hwVer[16];             /*pad的硬件版本信息*/
    Char swVer[16];             /*pad的软件版本信息*/
}S_HtePadPowerOn;

/*设备心跳*/
typedef struct{
    Int32 devState;             /*设备状态*/
}S_HetPadHb;

/*键盘按键序号定义*/
#define KEY_WEISHENG     0x05       /*卫生*/
#define KEY_HOUZOU       0x06       /*后走*/
#define KEY_GUZHANG      0x07       /*故障*/
#define KEY_BAOJING      0x08       /*报警*/
#define KEY_JINZHAN      0x00       /*进站*/
#define KEY_QUXIAO       0x01       /*取消*/
#define KEY_UP           0x02       /*上*/
#define KEY_QUEREN       0x04       /*确认*/
#define KEY_FN           0x0A       /*FN*/
#define KEY_RANGZUO      0x0B       /*让座*/
#define KEY_ZHUANWAN     0x0C       /*转弯*/
#define KEY_CAIDAN       0x0D       /*菜单*/
#define KEY_CHUZHAN      0x0F       /*出站*/
#define KEY_LEFT         0x10       /*左*/
#define KEY_DOWN         0x11       /*下*/
#define KEY_RIGHT        0x13       /*右*/

/*键盘按下、长按、释放的按键序号*/
typedef struct{
    Uint8 keyId;                /*按键序号*/
}S_HtePadKeyId;

/*触摸屏按下*/
typedef struct{
    Uint16 xVoltage;            /*x轴电压采样值*/
    Uint16 yVoltage;            /*y轴电压采样值*/
}S_HtePadTsDown;

/*触摸屏坐标信息*/
typedef struct{
    Uint16 xVoltage;            /*x轴电压采样值*/
    Uint16 yVoltage;            /*y轴电压采样值*/
}S_HtePadTsCoord;

/*麦克风状态*/
typedef struct{
    Int32 micState;             /*手唛状态 0-释放 1-按下*/
}S_HtePadMicState;

/*背光设置*/
typedef struct{
    Uint8 blValue;              /*背光亮度值,取值0-10,0-关闭背光 10-最亮*/
}S_HtePadBlSet;

/*显示模式设置*/
typedef struct{
    Uint8 displayMode;          /*显示模式*/
}S_HtePadModeSet;

/*显示模式设置应答*/
typedef struct{
    Uint8 result;               /*设置结果 0x10-切换成功 0x01-切换失败*/
}S_HtePadModeSetRply;

/*设备重启*/
typedef struct{
    Uint8 type;                 /*重启类型 0-整机重启  1-显示模组重启*/
}S_HtePadRestart;

/*版本号查询*/
typedef struct{
    Uint8 devType;              /*查询设备 0-stm32 1-tw8836*/
}S_HtePadVerQuery;

/*版本查询应答*/
typedef struct{
    Char hwVer[16];             /*查询设备的硬件版本信息*/
    Char swVer[16];             /*查询设备的软件版本信息*/
}S_HtePadVerion;

/*屏幕翻转*/
typedef struct{
    Uint8 reverseType;          /*翻转类型 0-上下翻转 1-左右翻转*/
}S_HtePadReverse;

/*蜂鸣器控制*/
typedef struct{
    Uint8 cmd;                  /*控制命令 0-关 1-开*/
}S_HtePadBuzCtrl;




#endif


