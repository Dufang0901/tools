/*****************************************************************************
  FileName:     CardDev.h
  Author:       林锴
  Version:      1.0
  Date:         2015/11/9 17:41:57
  Description:  刷卡设备的操作
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2015.11.09    V1.0       create
*****************************************************************************/

#ifndef _CARD_DEV_H_
#define _CARD_DEV_H_

#include "OsPack.h"
#include "OsCommon.h"

#define CARD_DEV_DEBUG             0

#define CARD_DEV_MSG_BUF_MAX        512

#define CARD_DEV_OK                 1
#define CARD_DEV_NONE_OK            0

int CardDevInit();
int CardDevRecvMsg(Uint8 *pMsg,Uint16 *msgLen);
int CardDevSendMsg(Uint8 *pMsg,Uint16 msgLen);

#endif

