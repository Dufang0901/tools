/*****************************************************************************
  FileName：    BleProtocol.h
  Author:       林锴
  Version:      1.0
  Date:         2016/4/19 18:37:10
  Description:  BLE的自定义协议
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.19    V1.0      create
*****************************************************************************/

#ifndef _BLE_PROTOCOL_H_
#define _BLE_PROTOCOL_H_

#include "OsPack.h"
#include "OsCommon.h"


#define BLE_PROT_OK                     1
#define BLE_PROT_ERROR_UNKNOW           0
#define BLE_PROT_ERROR_MSGLEN           -1
#define BLE_PROT_ERROR_HEAD             -2
#define BLE_PROT_ERROR_CRC              -3
#define BLE_PROT_ERROR_DATALEN          -4

#define BLE_PACKET_BTYES_MAX            128 

/*与BLE通信的相关命令*/

#define BLE_MSG_HEAD0                   'H'         /*固定消息头0*/
#define BLE_MSG_HEAD1                   'T'         /*固定消息头1*/

/*BLE消息命令*/

/*由BLE发起*/
#define BLE_CMD_DEV_START               0x01        /*设备启动*/
#define BLE_CMD_DEV_ABNOR               0x02        /*设备异常*/
#define BLE_CMD_TAG_SCAN_REP            0x03        /*扫描到标签*/
#define BLE_CMD_MT_SCAN_REP             0x04        /*扫描到移动终端*/
#define BLE_CMD_RN_SCAN_REP             0x05        /*扫描到路侧节点*/
#define BLE_CMD_BL_CALL                 0x06        /*盲人呼叫*/
#define BLE_CMD_MT_COMM_RET             0x07        /*设备通信连接结果-连接or断开*/
#define BLE_CMD_RN_COMM_RET             0x08        /*路侧节点连接结果-连接or断开*/
#define BLE_CMD_RN_CON_RET              0x09        /*路侧节点-连接结果*/
#define BLE_CMD_RN_TRAN_OVER            0x0A        /*路侧节点-传输完成*/

/*由终端发起*/
#define BLE_CMD_DEV_RESTART             0x11        /*要求BLE重启*/
#define BLE_CMD_MODE_SET                0x12        /*工作模式设置*/
#define BLE_CMD_CHAR_DATA_WR            0x13        /*写从机的指定特征写数据*/


#define BLE_CMD_HB                      0x21        /*心跳*/
#define BLE_CMD_COMMON                  0x22        /*通用*/


/*BLE消息头*/
typedef struct
{
    Uint8  head[2];
    Uint8  cmd;
    Uint8  crc;
    Uint8  sn;
    Uint8  reserve;
    Uint8  dataLen[2];
}S_BleMsgHead;

#define BLE_MSG_HEAD_LEN                sizeof(S_BleMsgHead)

/*BLE的相关信息*/
typedef struct
{
    Uint8 uuid[2];
}S_BleUuid;

typedef struct
{
    Uint8  addrType;
    Uint8  addr[6];
}S_BleGapAddr;


/*标签广播扫描结果上报*/
typedef struct
{
    Uint8  tagInfo[5];                  /*遵循Ibeacon的2B major + 2B minor + 1B rssi*/
    Int8   rssi;
}S_BleScanTagRep;

/*移动终端广播扫描结果上报*/
typedef struct
{
    Uint8  mtAdvInfo[5];
    Int8   rssi;
}S_BleScanMtRep;

/*移动终端广播扫描结果上报*/
typedef struct
{
    S_BleGapAddr sRnBleGapAddr;
    S_BleUuid    sRnUuid;
    Int8   rssi;
}S_BleScanRnRep;

/*盲人呼叫*/
typedef struct
{
    Uint8  info[16];
}S_BleBlindCall;

/*BLE其他设备连接结果*/
typedef struct
{
    Uint8 ret;
    Uint8 reserve[4];
}S_BleMtCommRet;

/*BLE与路侧节点通信的结果*/
typedef struct
{
    Uint8 ret;
}S_BleRnCommRet;

/*BLE连接路侧节点结果*/
#define BLE_PER_SERVICE_CHAR_MAX        8
typedef struct
{
    Uint8 ret;
    Uint8 gattcCharTotal;
    S_BleUuid sCharUuid[BLE_PER_SERVICE_CHAR_MAX];
}S_BleRnConRet;

/*模式设置*/
typedef struct
{
    Uint8 tagUuid[16];
    Uint8 mtUuid[16];
    Char  rnDevName[16];
}S_BleObModePar;

typedef struct
{
    Uint8 uuid[16];
    Uint8 major[2];
    Uint8 minor[2];
    Int8 rssi;
    Char perDevName[16];
}S_BlePerModePar;

typedef struct
{
    Uint8 tagUuid[16];
    Uint8 mtUuid[16];
    Char  rnDevName[16];
    Char  perDevName[16];
    Uint8 major[2];
    Uint8 minor[2];
    Int8 rssi;
}S_BleOpModePar;

typedef struct
{
    S_BleGapAddr    sPerBleGapAddr;
    S_BleUuid       serviceUuid;
    Uint8           serviceCharNum;
}S_BleCentralModePar;

typedef struct
{
    Uint8 mode;
    Uint8 reserve;
    union
    {
        S_BleObModePar      sBleObModePar;
        S_BlePerModePar     sBlePerModePar;
        S_BleOpModePar      sBleOpModePar;
        S_BleCentralModePar sBleCentralModePar;
    }params;
}S_BleModeSet;

/*主机写特征数据*/
typedef struct
{
    S_BleUuid   sCharUuid;
    Uint8       dataLen[2];
    Uint8       data[20];
}S_BleCentralDataWr;


/*心跳*/
typedef struct
{
    Uint8 mode;
}S_BleHb;

#define BLE_MODE_IDEL                   0
#define BLE_MODE_OBSERVER               1
#define BLE_MODE_PERIPHERAL             2
#define BLE_MODE_OP                     3
#define BLE_MODE_CENTREAL               4

typedef struct
{
    Uint8  major[2];
    Uint8  minor[2];
    Uint8  rssi;
}S_BleIbeaconInfo;

Uint8 BleMsgCrc(Uint8 *pData, Uint16 len);
Int32 BleMsgVerif(Uint8 *pMsg,Uint16 msgLen);
void  BleTagInfoDec(S_BleIbeaconInfo *sTagInfoEncrypt,S_BleIbeaconInfo *sTagInfoOrig);
void  BleTagInfoEnc(S_BleIbeaconInfo *sTagInfoOrig,S_BleIbeaconInfo *sTagInfoEncrypt);

#endif
