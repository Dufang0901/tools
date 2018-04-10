/*****************************************************************************
  FileName:     Rtc.h
  Author:       林锴
  Version:      1.0
  Date:         2015/9/9 15:14:07
  Description:  RTC操作
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2015.09.09     V1.0      create
*****************************************************************************/

#ifndef _RTC_H_
#define _RTC_H_

#include "HteProtocol.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/reboot.h>
#include <linux/watchdog.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/timerfd.h>
#include <sys/reboot.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>

#define RTC_IIC_DEV                     "/dev/i2c-2"

/*时间*/
typedef struct
{
    Uint8 year;
    Uint8 month;
    Uint8 date;
    Uint8 week;
    Uint8 hour;
    Uint8 minute;
    Uint8 second;
}S_RtcTime;

int RtcInit(void);
int RtcGetTimeToSys(void);
int RtcSetTimeFromSys(void);

#endif
