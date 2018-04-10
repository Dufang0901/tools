/*****************************************************************************
  FileName：    GpsNmeaTrimble.h
  Author:       林锴
  Version:      1.0
  Date:         2016/4/8 10:04:03
  Description:  Trimble的Gps模块自定义nmea协议的共用接口
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.08    V1.0      create
*****************************************************************************/

#ifndef _GPS_NMEA_TRIMBLE_H_
#define _GPS_NMEA_TRIMBLE_H_

#include "OsPack.h"
#include "OsCommon.h"
#include "GpsNmea.h"

/*PTNLRDR,STAT1规定的结构,按照NMEA协议排序下来的,不考虑对齐*/
typedef struct
{
    Char statusFlags[8];
    Char timeSource[4];
    Char gpsDataAge[8];
    Char gpsTimeOfWeek[12];
    Char gpsWeekNumer[8];
    Char deltaTime[8];      //ms
    Char deltaDis[8];       //m
    Char deltaHeading[8];   //degress
    Char freOffset[12];     //m/s
    Char utcOffset[8];
}S_GpsNmeaCharStat1;

/*PTNLRDR,STAT2规定的结构,按照NMEA协议排序下来的,不考虑对齐*/
typedef struct
{
    Char positionAccu[8];   //平面精度，单位m
    Char altitudeAccu[8];
    Char headingAccu[8];
    Char horizontalSpeedAccu[8];
    Char verticalSpeedAccu[8];
    Char deltaDisAccu[8];
    Char deltaHeadingAccu[8];
}S_GpsNmeaCharStat2;

/*PTNLRDR,SENS1规定的结构,按照NMEA协议排序下来的,不考虑对齐*/
typedef struct
{
    Char systemTime[12];  
    Char timeSource[8];
    Char systemTimeOffset[12];
    Char ppsTime[12];
    Char direction[4];
    Char tachoCount[4];
}S_GpsNmeaCharSens1;

/*PTNLRDR,SENS2规定的结构,按照NMEA协议排序下来的,不考虑对齐*/
typedef struct
{
    Char systemTime[12];  
    Char numOfGyro[12];
    Char sumOfGyro_x[12];
    Char sumOfGyro_y[12];
    Char sumOfGyro_z[12];
    Char gypoTemperature[8];
    Char gypoTemperatureValidiy[4];
}S_GpsNmeaCharSens2;


Int32 GpsGetTbDrGgaFromPacket(Char *pPacketMsg,S_GpsNmeaCharGpgga *psNmeaGpgga);
Int32 GpsGetTbDrRmcFromPacket(Char *pPacketMsg,S_GpsNmeaCharGprmc *psNmeaGprmc);
Int32 GpsGetTbDrStat1FromPacket(Char *pPacketMsg,S_GpsNmeaCharStat1 *psNmeaDrStat1);
Int32 GpsGetTbDrStat2FromPacket(Char *pPacketMsg,S_GpsNmeaCharStat2 *psNmeaDrStat2);
Int32 GpsGetTbDrSens1FromPacket(Char *pPacketMsg,S_GpsNmeaCharSens1 *psNmeaDrSens1);
Int32 GpsGetTbDrSens2FromPacket(Char *pPacketMsg,S_GpsNmeaCharSens2 *psNmeaDrSens2);

Int32 GpsTbDrUnpackStat1(Char *pInDataStart,S_GpsNmeaCharStat1 *psInNmeaDrStat1);
Int32 GpsTbDrUnpackStat2(Char *pInDataStart,S_GpsNmeaCharStat2 *psInNmeaDrStat2);
Int32 GpsTbDrUnpackSens1(Char *pInDataStart,S_GpsNmeaCharSens1 *psInNmeaDrSens1);
Int32 GpsTbDrUnpackSens2(Char *pInDataStart,S_GpsNmeaCharSens2 *psInNmeaDrSens2);


#endif
