/*****************************************************************************
  FileName：    GpsCommon.h
  Author:       林锴
  Version:      1.0
  Date:         2016/4/7 14:26:13
  Description:  Gps应用的共用接口
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.07    V1.0      create
*****************************************************************************/

#ifndef _GPS_COMMON_H_
#define _GPS_COMMON_H_

#include "OsPack.h"
#include "OsCommon.h"

#define GPS_CM_OK               1
#define GPS_CM_ERROR_UNKNOW     0
#define GPS_CM_ERRPR_OPEN       -1
#define GPS_CM_ERROR_FCNTL      -2
#define GPS_CM_ERROR_TC         -3
#define GPS_CM_ERROR_RDWR       -4


#define GPS_MODULE_UNKNOW       0
#define GPS_MODULE_NORMAL       1
#define GPS_MODULE_TRIMBLE      2
#define GPS_MODULE_UBLOX        3


#define GPS_PACKET_MAX_BYTES    1024           /*GPS单包最大字节数*/

Int32  GpsCmLibVersionGet(Char *pInVer);

int    GpsComOpenCfg(const Char *pInDev,speed_t inBaud);

Int32  GpsFlushBuffer(const int inComFd);
Int32  GpsRecvOnePacket(const int inComFd,Uint8 *pInPacketMsg);
Int32  GpsSendMsg(int inComFd,Uint8 *pInMsg,Uint16 inMsgLen);
Int32  GpsRecvMsg(int inComFd,Uint8 *pInMsg,Uint16 *pInMsgLen,Uint16 inMaxLen);

double GpsLoLaCharToDouble(Char *pInlolaChar);
double GpsLoLaDmToD(double inLolaDm);
double GpsLoLaDToDm(double inLolaD);
double GpsGetTwoPointDist(double inLatitude1,double inLongitude1,double inLatitude2,double inLongitude2);
Uint32 GpsTimeUtcToBcd(Char *pInUtcTime);
Uint32 GpsTimeDmyToBcd(Char *pInDmyTime);

#endif
