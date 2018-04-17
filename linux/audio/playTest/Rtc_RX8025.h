/*****************************************************************************
  FileName:     Rtc_RX8025.h
  Author:       林锴
  Version:      1.0
  Date:         2014.08.11
  Description:  RTC操作,芯片对应RX8025
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2014.08.11     V1.0      create
*****************************************************************************/

#ifndef _RTC_RX8025_H_
#define _RTC_RX8025_H_

#include "Rtc.h"

#define RTC_IIC_ADDR        0x32

#define RTC_REG_SECOND        0x00
#define RTC_REG_MINUTE        0x01
#define RTC_REG_HOUR        0x02
#define RTC_REG_WEEK        0x03
#define RTC_REG_DAY         0x04
#define RTC_REG_MONTH        0x05
#define RTC_REG_YEAR        0x06
#define RTC_REG_DIG_OFFSET  0x07
#define RTC_REG_AW_MINUTE    0x08
#define RTC_REG_AW_HOUR        0x09
#define RTC_REG_AW_WEEK        0x0A
#define RTC_REG_AD_MINUTE    0x0B
#define RTC_REG_AD_HOUR        0x0C
#define RTC_REG_CTRL1        0x0E
#define RTC_REG_CTRL2        0x0F

int Rtc8025Init(int iicFd);
int Rtc8025GetTime(int iicFd,S_RtcTime *time);
void Rtc8025SetTime(int iicFd,S_RtcTime *time);

#endif
