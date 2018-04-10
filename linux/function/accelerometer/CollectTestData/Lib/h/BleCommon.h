/*****************************************************************************
  FileName：    BleCommon.h
  Author:       林锴
  Version:      1.0
  Date:         2016/4/20 10:30:10
  Description:  BLE应用的共用接口
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.20    V1.0      create
*****************************************************************************/

#ifndef _BLE_COMMON_H_
#define _BLE_COMMON_H_

#include "BleProtocol.h"

#define BLE_CM_OK               1
#define BLE_CM_ERROR_UNKNOW     0
#define BLE_CM_ERRPR_OPEN       -1
#define BLE_CM_ERROR_FCNTL      -2
#define BLE_CM_ERROR_TC         -3
#define BLE_CM_ERROR_RDWR       -4
#define BLE_CM_ERROR_TIMEOUT    -5
#define BLE_CM_ERROR_MSG        -6


typedef struct
{
    Uint8  msgCmd;
    Uint8  msgSn;
    Uint16 msgLen;
}S_BleMsgHeader;

typedef struct
{
    S_BleMsgHeader sBleMsgHeader;
    union
    {
        S_BleScanTagRep sBleTagRep;
        S_BleScanMtRep  sBleMtRep;
        S_BleScanRnRep  sBleRnRep;
        S_BleBlindCall  sBleBlindCall;
        S_BleMtCommRet  sBleMtCommRet;
        S_BleRnCommRet  sBleRnCommRet;
        S_BleRnConRet   sBleRnConRet;
        S_BleHb         sBleHb;
    }U_BleMsg;
}S_BleMsg;

int BleComOpenCfg(const Char *dev);
Int32 BleFlushBuffer(const int comFd);
Int32 BleRecvOnePacket(const int comFd,Uint8 *pPacketMsg,Uint16 *pPacketLen);
Int32 BleSendMsg(int comFd,Uint8 *pMsg,size_t msgLen);
Int32 BleRecvPacketPro(Uint8 *pPacketMsg,Uint16 packetLen,S_BleMsg *pBleMsgPro);
Int32 BleWaitRplyMsg(int comFd,Uint8 cmd,Uint8 sendSn);
Int32 BleModeSet(int comFd,S_BleModeSet *psBleMode);
Int32 BleRestart(int comFd);
Int32 BleCentralWrData(int comFd,Uint16 charUuid,Uint8 *pData,Uint16 dataLen);
Int32 BleHeartbeatRply(int comFd,Uint8 mode,Uint8 rdSn);



#endif
