/*****************************************************************************
  FileName：    GpsNmea.h
  Author:       林锴
  Version:      1.0
  Date:         2016/4/7 14:45:41
  Description:  Gps应用的共用接口
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.07    V1.0      create
*****************************************************************************/

#ifndef _GPS_NMEA_H_
#define _GPS_NMEA_H_

#include "OsPack.h"
#include "OsCommon.h"


#define GPS_NMEA_MSG_MAX_BYTES          256     /*单条NMEA消息的最长字节数*/

#define GPS_NMEA_OK                     1       /*GPS的NMEA协议操作成功*/
#define GPS_NMEA_ERROR_UNKNOW           0       /*GPS的NMEA协议操作错误 - 原因未知*/
#define GPS_NMEA_ERROR_MSG              -1      /*GPS的NMEA协议操作错误 - 消息异常*/
#define GPS_NMEA_ERROR_CRC              -2      /*GPS的NMEA协议操作错误 - 校验错误*/

#define GPS_NMEA_FRAME_OK               1
#define GPS_NMEA_FRAME_NOCOMP           0
#define GPS_NMEA_FRAME_ERR_HEAD         -1
#define GPS_NMEA_FRAME_ERR_CRC          -2

#define GPS_NMEA_HEAD                   '$'

#define GPS_NMEA_TYPE_GGA               0x01
#define GPS_NMEA_TYPE_RMC               0x02
#define GPS_NMEA_TYPE_GSV               0x03
#define GPS_NMEA_TYPE_GSA               0x04
#define GPS_NMEA_TYPE_VTG               0x05
#define GPS_NMEA_TYPE_GLL               0x06
#define GPS_NMEA_TYPE_ZDA               0x07

#define GPS_NMEA_TYPE_UNKNOW            0
#define GPS_NMEA_HEAD_GP                "GP"
#define GPS_NMEA_TYPE_GP                0x00
#define GPS_NMEA_TYPE_GPGGA             0x01
#define GPS_NMEA_TYPE_GPRMC             0x02
#define GPS_NMEA_TYPE_GPGSV             0x03
#define GPS_NMEA_TYPE_GPGSA             0x04
#define GPS_NMEA_TYPE_GPVTG             0x05
#define GPS_NMEA_TYPE_GPGLL             0x06
#define GPS_NMEA_TYPE_GPZDA             0x07

#define GPS_NMEA_HEAD_GN                "GN"
#define GPS_NMEA_TYPE_GN                0x10
#define GPS_NMEA_TYPE_GNGGA             0x11
#define GPS_NMEA_TYPE_GNRMC             0x12
#define GPS_NMEA_TYPE_GNGSV             0x13
#define GPS_NMEA_TYPE_GNGSA             0x14
#define GPS_NMEA_TYPE_GNVTG             0x15
#define GPS_NMEA_TYPE_GNGLL             0x16
#define GPS_NMEA_TYPE_GNZDA             0x17

#define GPS_NMEA_HEAD_GB                "GB"
#define GPS_NMEA_TYPE_GB                0x20
#define GPS_NMEA_TYPE_GBGGA             0x21
#define GPS_NMEA_TYPE_GBRMC             0x22
#define GPS_NMEA_TYPE_GBGSV             0x23
#define GPS_NMEA_TYPE_GBGSA             0x24
#define GPS_NMEA_TYPE_GBVTG             0x25
#define GPS_NMEA_TYPE_GBGLL             0x26
#define GPS_NMEA_TYPE_GBZDA             0x27

#define GPS_NMEA_HEAD_PTNLR             "PTNLR"
#define GPS_NMEA_TYPE_TB                0x30
#define GPS_NMEA_TYPE_TBGGA             0x31
#define GPS_NMEA_TYPE_TBRMC             0x32
#define GPG_NMEA_TYPE_TBSTART           0x3A
#define GPG_NMEA_TYPE_TBSTAT1           0x3B
#define GPG_NMEA_TYPE_TBSTAT2           0x3C
#define GPG_NMEA_TYPE_TBSENS1           0x3D
#define GPG_NMEA_TYPE_TBSENS2           0x3E
#define GPG_NMEA_TYPE_TBEND             0x3F

#define GPS_NMEA_HEAD_ZB                "ZB"
#define GPS_NMEA_TYPE_ZB                0x40
#define GPS_NMEA_TYPE_ZBBRD             0x49


/*NMEA协议中GPGGA的结构定义*/
typedef struct
{
    Char UTC[12];
    Char latitude[12];    /*度分格式*/
    Char earthNS[2];
    Char longitude[12];   /*度分格式*/
    Char earthEW[2];
    Char qualityInd[2];
    Char satellitesUse[4];
    Char HDOP[8];    
    Char antennaAlt[8];
    Char geoidalSeparation[8];
    Char diffGpsData[8];
    Char diffStaId[8]; 
}S_GpsNmeaCharGpgga;

/*NMEA协议中GNGGA的结构定义*/
typedef struct
{
    Char UTC[12];
    Char latitude[12];    /*度分格式*/
    Char earthNS[2];
    Char longitude[12];   /*度分格式*/
    Char earthEW[2];
    Char qualityInd[2];
    Char satellitesUse[4];
    Char HDOP[8];    
    Char antennaAlt[8];
    Char geoidalSeparation[8];
    Char diffGpsData[8];
    Char diffStaId[8]; 
}S_GpsNmeaCharGngga;

/*NMEA协议中GPRMC的结构定义*/
typedef struct
{
    Char UTC[12];
    Char status[2];
    Char latitude[12];    /*度分格式*/
    Char earthNS[2];
    Char longitude[12];   /*度分格式*/
    Char earthEW[2];
    Char speed[8];
    Char course[8];
    Char dmy[8];
    Char magneticVariation[8];
    Char declination[2];
    Char mode[2];
}S_GpsNmeaCharGprmc;

/*NMEA协议中GNRMC的结构定义*/
typedef struct
{
    Char UTC[12];
    Char status[2];
    Char latitude[12];    /*度分格式*/
    Char earthNS[2];
    Char longitude[12];   /*度分格式*/
    Char earthEW[2];
    Char speed[8];
    Char course[8];
    Char dmy[8];
    Char magneticVariation[8];
    Char declination[2];
    Char mode[2];
}S_GpsNmeaCharGnrmc;

/*NMEA协议中GPVTG的结构定义*/
typedef struct
{
    Char courseTrue[8];
    Char courseMagnetic[8];
    Char speedKnots[8];
    Char speedKm[8];
    Char mode[2];
}S_GpsNmeaCharGpvtg;

/*NMEA协议中GNVTG的结构定义*/
typedef struct
{
    Char courseTrue[8];
    Char courseMagnetic[8];
    Char speedKnots[8];
    Char speedKm[8];
    Char mode[2];
}S_GpsNmeaCharGnvtg;

/*NMEA协议中GPGSA的结构定义*/
typedef struct
{
    Char operatingMode[4];
    Char navigationMode[4];
    Char prn1[4];
    Char prn2[4];
    Char prn3[4];
    Char prn4[4];
    Char prn5[4];
    Char prn6[4];
    Char prn7[4];
    Char prn8[4];
    Char prn9[4];
    Char prn10[4];
    Char prn11[4];
    Char prn12[4];
    Char PDOP[12];
    Char HDOP[12];
    Char VDOP[12];
}S_GpsNmeaCharGpgsa;

/*NMEA协议中GPGSV的结构定义*/
typedef struct
{
    Char totalNum[4];
    Char msgNum[4];
    Char totalSatellites[4];
    Char prn1[4];
    Char elevation1[4];
    Char azimuth1[4];
    Char snr1[4];
    Char prn2[4];
    Char elevation2[4];
    Char azimuth2[4];
    Char snr2[4];
    Char prn3[4];
    Char elevation3[4];
    Char azimuth3[4];
    Char snr3[4];
    Char prn4[4];
    Char elevation4[4];
    Char azimuth4[4];
    Char snr4[4];
}S_GpsNmeaCharGpgsv;

/*NMEA协议中GBGSV的结构定义*/
typedef struct
{
    Char totalNum[4];
    Char msgNum[4];
    Char totalSatellites[4];
    Char prn1[4];
    Char elevation1[4];
    Char azimuth1[4];
    Char snr1[4];
    Char prn2[4];
    Char elevation2[4];
    Char azimuth2[4];
    Char snr2[4];
    Char prn3[4];
    Char elevation3[4];
    Char azimuth3[4];
    Char snr3[4];
    Char prn4[4];
    Char elevation4[4];
    Char azimuth4[4];
    Char snr4[4];
}S_GpsNmeaCharGbgsv;


/******************************************************华丽的分割线*******************************************************/
/*NMEA协议中GNGGA的结构定义*/
typedef struct
{
    Char UTC[12];
    Char latitude[12];    /*度分格式*/
    Char earthNS[2];
    Char longitude[12];   /*度分格式*/
    Char earthEW[2];
    Char qualityInd[2];
    Char satellitesUse[4];
    Char HDOP[8];    
    Char antennaAlt[8];
    Char geoidalSeparation[8];
    Char diffGpsData[8];
    Char diffStaId[8]; 
}S_GpsNmeaCharGga;

/*NMEA协议中GNRMC的结构定义*/
typedef struct
{
    Char UTC[12];
    Char status[2];
    Char latitude[12];    /*度分格式*/
    Char earthNS[2];
    Char longitude[12];   /*度分格式*/
    Char earthEW[2];
    Char speed[8];
    Char course[8];
    Char dmy[8];
    Char magneticVariation[8];
    Char declination[2];
    Char mode[2];
}S_GpsNmeaCharRmc;

/*NMEA协议中VTG的结构定义*/
typedef struct
{
    Char courseTrue[8];
    Char courseMagnetic[8];
    Char speedKnots[8];
    Char speedKm[8];
    Char mode[2];
}S_GpsNmeaCharVtg;

/*NMEA协议中GPGSA的结构定义*/
typedef struct
{
    Char operatingMode[4];
    Char navigationMode[4];
    Char prn1[4];
    Char prn2[4];
    Char prn3[4];
    Char prn4[4];
    Char prn5[4];
    Char prn6[4];
    Char prn7[4];
    Char prn8[4];
    Char prn9[4];
    Char prn10[4];
    Char prn11[4];
    Char prn12[4];
    Char PDOP[12];
    Char HDOP[12];
    Char VDOP[12];
}S_GpsNmeaCharGsa;


/*NMEA协议中GSV的结构定义*/
typedef struct
{
    Char totalNum[4];
    Char msgNum[4];
    Char totalSatellites[4];
    Char prn1[4];
    Char elevation1[4];
    Char azimuth1[4];
    Char snr1[4];
    Char prn2[4];
    Char elevation2[4];
    Char azimuth2[4];
    Char snr2[4];
    Char prn3[4];
    Char elevation3[4];
    Char azimuth3[4];
    Char snr3[4];
    Char prn4[4];
    Char elevation4[4];
    Char azimuth4[4];
    Char snr4[4];
}S_GpsNmeaCharGsv;

Uint8 GpsNmeaCalcCrc(Uint8 *pMsg,Uint16 msgLen);
Int32 GpsNmeaMsgChk(Char *pMsg);
Int32 GpsNmeaMsgParGet(Char *pMsg, Uint8 parNum, Char *pPar);

Int32 GpsNmeaGetMsgFormPacket(Char *pPacketMsg,Char *pCmd,Uint8 cmdNum,Char *pMsg);
Int32 GpsGetGpggaFromPacket(Char *pPacketMsg,S_GpsNmeaCharGpgga *psNmeaGpgga);
Int32 GpsGetGnggaFromPacket(Char *pPacketMsg,S_GpsNmeaCharGngga *psNmeaGngga);
Int32 GpsGetGprmcFromPacket(Char *pPacketMsg,S_GpsNmeaCharGprmc *psNmeaGprmc);
Int32 GpsGetGnrmcFromPacket(Char *pPacketMsg,S_GpsNmeaCharGnrmc *psNmeaGnrmc);
Int32 GpsGetGpvtgFromPacket(Char *pPacketMsg,S_GpsNmeaCharGpvtg *psNmeaGpvtg);
Int32 GpsGetGnvtgFromPacket(Char *pPacketMsg,S_GpsNmeaCharGnvtg *psNmeaGnvtg);
Int32 GpsGetGpgsaFromPacket(Char *pPacketMsg,S_GpsNmeaCharGpgsa *psNmeaGpgsa);
Int32 GpsGetGpgsvFromPacket(Char *pPacketMsg,S_GpsNmeaCharGpgsv *psNmeaGpgsv,Int32 gsvMsgNum);
Int32 GpsGetGbgsvFromPacket(Char *pPacketMsg,S_GpsNmeaCharGbgsv *psNmeaGbgsv,Int32 gsvMsgNum);

/*与2016-10-31新增*/
Int32 GpsNmeaHeadMatch(Uint8 *pInMsg,Uint16 inMsgLen,Uint16 *pInPtr);
Int32 GpsNmeaGetOneFrame(Uint8 *pInMsg,Uint16 inMsgLen,Char *pInNmeaFrame,Uint16 *pInFrameLen);
Int32 GpsNmeaGetTypeFromFrame(Char *pInNmeaFrame,Char **pInDataStart);
Int32 GpsNmeaUnpackGga(Char *pInDataStart,S_GpsNmeaCharGga *psInNmeaGga);
Int32 GpsNmeaUnpackRmc(Char *pInDataStart,S_GpsNmeaCharRmc *psInNmeaRmc);
Int32 GpsNmeaUnpackVtg(Char *pInDataStart,S_GpsNmeaCharVtg *psInNmeaVtg);
Int32 GpsNmeaUnpackGsa(Char *pInDataStart,S_GpsNmeaCharGsa *psInNmeaGsa);
Int32 GpsNmeaUnpackGsv(Char *pInDataStart,S_GpsNmeaCharGsv *psInNmeaGsv);


#endif
