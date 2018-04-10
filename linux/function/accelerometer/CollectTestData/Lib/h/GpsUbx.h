/*****************************************************************************
  FileName��    GpsUbx.h
  Author:       ����
  Version:      1.0
  Date:         2016/10/31 14:50:52
  Description:  UBLOX�Ķ�λģ���Զ���UBXЭ��Ĺ��ýӿ�
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.10.31    V1.0      create
*****************************************************************************/

#ifndef _GPS_UBX_H_
#define _GPS_UBX_H_


#include "OsPack.h"
#include "OsCommon.h"


#define GPS_UBX_OK                      1       /*UBXЭ������ɹ�*/
#define GPS_UBX_ERROR_UNKNOW            0       /*GUBXЭ��������� - ԭ��δ֪*/
#define GPS_UBX_ERROR_MSG               -1      /*UBXЭ��������� - ��Ϣ�쳣*/
#define GPS_UBX_ERROR_CRC               -2      /*UBXЭ��������� - У�����*/

#define GPS_UBX_FRAME_OK                1
#define GPS_UBX_FRAME_NOCOMP            0
#define GPS_UBX_FRAME_ERR_HEAD          -1
#define GPS_UBX_FRAME_ERR_CRC           -2

#define GPS_UBX_SYNC_CHAR1              0xB5
#define GPS_UBX_SYNC_CHAR2              0x62

#define GPS_UBX_ID_STATUS               0x10
#define GPS_UBX_ID_MEAS                 0x02
#define GPS_UBX_ID_RAW                  0x03

/*UBX�Ĺ̶���Ϣͷ*/
typedef struct
{
    Uint8   syncChar1;
    Uint8   syncChar2;
    Uint8   class;
    Uint8   id;
    Uint8   len[2];                     /*С�˵�λ��ǰ*/
}S_GpsUbxHead;

/*STATUS����Ϣ�ṹ*/
/*������״̬*/
typedef struct
{
    Uint8   stat0;
    Uint8   stat1;
    Uint8   freq;
    Uint8   err;
}S_GpsUbxEsfStatusSensor;

typedef struct
{
    Uint8   time[4];
    Uint8   ver;
    Uint8   reserve0[7];
    Uint8   calibStat;
    Uint8   reserve1[2];
    Uint8   sensorNum;
    /*���N����������Ϣ*/
}S_GpsUbxEsfStatus;

/*MEAS����Ϣ�ṹ*/
typedef struct
{
    Uint8   dataType;
    Uint8   dataField[3];
}S_GpsUbxMeasSensor;

typedef struct
{
    Uint8   time[4];
    Uint8   flag[2];
    Uint8   id[2];
    /*���N����������Ϣ*/
}S_GpsUbxMeas;

/*MEAS����Ϣ�ṹ*/
typedef struct
{
    Uint8   data[4];
    Uint8   time[4];
}S_GpsUbxRawSensor;

typedef struct
{
    Uint8   reserve0[8];
    /*���N����������Ϣ*/
}S_GpsUbxRaw;

Int32 GpsUbxCalcCrc(Uint8 *pInMsg,Uint16 inMsgLen,Uint8 *pInCrcA,Uint8 *pInCrcB);
Int32 GpsUbxGetOneFrame(Uint8 *pInMsg,Uint16 inMsgLen,Uint8 *pInUbxFrame,Uint16 *pInFrameLen);

#endif
