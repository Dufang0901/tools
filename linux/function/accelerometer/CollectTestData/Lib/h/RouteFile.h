/*****************************************************************************
  FileName��    RouteFile.h
  Author:       ����
  Version:      1.0
  Date:         2016/4/11 13:47:06
  Description:  ��·�ļ���Ӧ��
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.11    V1.0      create
*****************************************************************************/

#ifndef _ROUTE_FILE_H_
#define _ROUTE_FILE_H_

#include "OsPack.h"
#include "OsCommon.h"

#define ROUTE_CITY_MACAO

/*MC�Ĵ�����*/
#define ROUTE_OK               1        //�ɹ�
#define ROUTE_ERROR_UNKNOW     0        //ʧ�� - ԭ��δ֪
#define ROUTE_ERROR_NO_MEM     -1       //ʧ�� - �ڴ�ռ����
#define ROUTE_ERROR_FILE       -2       //ʧ�� - �ļ������쳣
#define ROUTE_ERROR_TOTAL      -3       //ʧ�� - ��·��������
#define ROUTE_ERROR_PAR        -4       //ʧ�� - ��������
#define ROUTE_ERROR_SEVICE     -5       //ʧ�� - ���񲻴���

/*
���еĶ���
#define ROUTE_CITY_GUANGZHOU
#define ROUTE_CITY_MACAO
#define ROUTE_CITY_BEITUN
*/

/*
��·�����ļ���ʽ
000N3,N3,,000N3.rdf,2012-08-01 00:00:00,2030-12-31 00:00:00
*/

/*
��·ͳ���ļ���ʽ
00001,00000001,2015-11-12 10:22:03
*/

#define ROUTE_LOCAL_MAX         256             /*���ر���������·��*/
#define ROUTE_SETVICE_MAX       32              /*ÿ����·������������*/
#define ROUTE_SERVICE_POINT_MAX 128             /*ÿ����·�����վ��-��Ȥ����*/
#define ROUTE_POINT_VOICE_MAX   16              /*ÿ��վ�����վ���������ֵ*/

typedef enum
{
    E_CITY_GUANGZHOU            = 1,            /*����*/
    E_CITY_MACAO                = 2,            /*����*/
    E_CITY_BEITUN               = 3             /*����*/
}E_ROUTE_CITY;

/*��վ�Ķ�λ��ʽ*/
typedef enum
{
    E_POINT_LOCATE_FENCE        = 1,            /*��վ��λģʽ - ͬ��Բ*/
    E_POINT_LOCATE_GATE         = 2             /*��վ��λģʽ - ��������*/
}E_ROUTE_POINT_LOCATE_MODE;

/*��λ�������*/
typedef enum
{
    E_POING_TYPE_NONE           = 0,            /*δ֪*/
    E_POING_TYPE_STA            = 1,            /*վ��*/
    E_POING_TYPE_INTEREST       = 2             /*��Ȥ��*/ 
}E_ROUTE_POINT_TYPE;

/*GPS��ض���ṹ*/
typedef struct
{
    double  longitude;          /*����,���ȸ�ʽ*/
    Char    earthEW;
 	double  latitude;           /*γ��,���ȸ�ʽ*/
    Char    earthNS;
}S_RoutePointGps;

/*��������ض���ṹ*/
typedef struct
{
    Uint32  bleTagId;           /*BLE��ǩ��ID*/
}S_RoutePointBle;

/*������������ؽṹ����*/
typedef struct
{
    S_RoutePointGps sPointGateGpsEnterLeft;
    S_RoutePointGps sPointGateGpsEnterRight;
    S_RoutePointGps sPointGateGpsLeaveLeft;
    S_RoutePointGps sPointGateGpsLeaveRight;
}S_RoutePointGateGps;

#define ROUTE_INVALID_DISTANCE  0xA5A5              /*��Ч�Ľ���վ����*/
typedef struct
{
    S_RoutePointGps             sPointGpsInfo;      /*վ���GPS������Ϣ*/
    E_ROUTE_POINT_LOCATE_MODE   ePointLocateMode;   /*��λģʽ*/
    Uint32                      enterDistance;      /*��վ����*/
    Uint32                      leaveDistance;      /*��վ����*/
    Int32                       mileage;            /*��̼� - �����վ����̼Ƶĸ�������*/
    S_RoutePointGateGps         sPointGateGpsInfo;  /*���������Ĳ���*/
}S_RoutePointGpsLocate;

#define ROUTE_INVALID_RSSI      100                  /*��Ч�Ľ���վ�ź�ǿ��*/
typedef struct
{
    S_RoutePointBle             sPointBleInfo;       /*վ���BLE��Ϣ*/
    Int8                        enterRssi;           /*��վ��ǩ�ź�ǿ��*/
    Int8                        leaveRssi;           /*��վ��ǩ�ź�ǿ��*/
}S_RoutePointBlelocate;



/*վ��/�ؼ���ı�վ��λ��Ϣ*/
typedef struct
{
    Char    pointCoding[16];      /*վ�����-��Ȥ�����*/
    Char    pointName[48];        /*վ����-��Ȥ����*/ 
    E_ROUTE_POINT_TYPE          ePointType;             /*���� - վ��or��Ȥ��*/
    S_RoutePointGpsLocate       sPointGpsLocateInfo;    /*GPS��λ����Ϣ*/
    S_RoutePointBlelocate       sPointBleLocateInfo;    /*BLE��λ����Ϣ*/
}S_RoutePointLocateInfo;

/*����������ؽṹ*/
typedef struct
{
    Char file[64];
}S_RouteVoiceFile;

typedef struct
{
    Uint32 voiceTotal;
    void   *pVoiceFile;
}S_RoutePointVoice;

/*����ͨ��ؽṹ*/
typedef struct
{
    Char destStaId[4];
    Char destStaZone;
    Char preStaId[4];
    Char preStaZone;
    Char nextStaId[4];
    Char nextStaZone;
    Char startStaId[4];
    Char routeDiretion;
}S_RouteStaAmtInfo;

/*վ��/�ؼ������Ӫ��Ϣ*/
typedef struct
{
    E_ROUTE_POINT_TYPE          ePointType;             /*���� - վ��or��Ȥ��*/

    Char    pointCoding[32];        /*վ�����-��Ȥ�����*/
    Char    enterStaName[32];       /*��վ��վ����*/
    Char    leaveStaName[32];       /*��վ��վ����*/
    Char    enterBroadFile[64];     /*��վ��վ����*/
    Char    leaveBroadFile[64];     /*��վ��վ����*/
    Char    enterLedCmd[64];        /*��վLED����*/
    Char    leaveLedCmd[64];        /*��վLED����*/
    Char    broadMode[8];           /*����ģʽ*/
    Int32   mileage;                /*��̼�*/
    Char    staAdvFile[64];         /*վ������*/
    
    S_RoutePointVoice sRouteVoiceEnter;     /*��վ��������Ϣ*/
    S_RoutePointVoice sRouteVoiceLeave;     /*��վ������Ϣ*/
#ifdef ROUTE_CITY_MACAO
    /*����*/
    Int32   planTime;               /*�ƻ�ʱ��,-1 ��ʾ��Ч*/
    Int32   speedLimit;             /*�ٶ�����,-1 ��ʾ��Ч*/
    Int32   stagnateTime;           /*��վʱ��,-1 ��ʾ��Ч*/
    Int32   crossTime;              /*˦վʱ��,-1 ��ʾ��Ч*/
    
    S_RouteStaAmtInfo sRouteStaAmtInfo;     /*����ͨ��Ϣ*/
#endif
}S_RoutePointOpeInfo;

/*��·�ķ�����Ϣ*/
typedef struct
{
    Char    serviceCoding[2];                                   /*�������*/
    Char    serviceName[14];                                    /*��������*/
    Char    direction[48];                                      /*����*/
    Char    departure[48];                                      /*������*/
    Char    destination[48];                                    /*Ŀ�ĵ�*/
    Uint32  staNums;                                            /*վ��ĸ���*/
    Uint32  intNums;                                            /*��Ȥ��/�ؼ���ĸ���*/
}S_RouteServiceInfo;

typedef struct
{
    Uint32 year;
    Uint32 month;
    Uint32 date;
    Uint32 hour;
    Uint32 minute;
    Uint32 second;
}S_RouteTime;

/*��·�Ļ�����Ϣ*/
typedef struct
{
    Char    routeName[16];                                      /*��·����*/
    Char    routeCoding[8];                                     /*��·����*/
    Char    routeInfoFile[16];                                  /*��·��Ϣ�ļ�*/
    Char    routeBtFile[16];                                    /*��·�������ļ�*/
    Char    routeVoiceFile[16];                                 /*��·�������ļ�*/
    /*ע���ļ�����·��*/
    S_RouteTime sRouteEffectTime;                               /*��·��Чʱ��*/
    S_RouteTime sRouteFailTime;                                 /*��·ʧЧʱ��*/

    Int32   routeOpTimes;                                       /*��·��Ӫ����*/
    S_RouteTime sRouteLastOpTime;                               /*��·���һ����Ӫ��ʱ��*/
    
    Int32   routeFileExist;                                     /*��·�ļ��Ĵ��ڵ�״̬*/
#ifdef ROUTE_CITY_MACAO
    Char    routeAmtCoding[8];                                  /*����ͨ����·����*/
    Char    routeFileVer[16];                                   /*��·�ļ��İ汾*/
#endif
    Uint32  serviceTotal;                                       /*��·��������*/
    void    *pServiceInfo;                                      /*��·������Ϣ*/
}S_RouteBasicInfo;

/*��Ч��·�Ļ�����Ϣ*/
typedef struct
{
    Char    routeName[16];                                      /*��·����*/
    Char    routeCoding[8];                                     /*��·����*/
    Char    routeInfoFile[16];                                  /*��·��Ϣ�ļ�*/
    Char    routeBtFile[16];                                    /*��·�������ļ�*/
    Char    routeVoiceFile[16];                                 /*��·�������ļ�*/
    Char    routeOrbitFile[16];                                 /*��·�Ĺ켣�ļ�*/
    /*ע���ļ�����·��*/
    
    Int32   routeOpTimes;                                       /*��·��Ӫ����*/
    S_RouteTime sRouteLastOpTime;                               /*��·���һ����Ӫ��ʱ��*/
    
#ifdef ROUTE_CITY_MACAO
    Char    routeAmtCoding[8];                                  /*����ͨ����·����*/
    Char    routeFileVer[16];                                   /*��·�ļ��İ汾*/
#endif
    Uint32  serviceTotal;                                       /*��·��������*/
    void    *pServiceInfo;                                      /*��·������Ϣ*/
}S_RouteValidBasicInfo;

/*��·����Ĺ켣��Ϣ*/
typedef struct
{
    double  latitude;           /*����,���ȸ�ʽ*/
    Char    earthNS;
    double  longitude;          /*γ��,���ȸ�ʽ*/
    Char    earthEW;
    double  course;             /*��λ��*/
}S_RouteOrbitInfo;

typedef struct
{
    Char *pIdxFile;             /*�����ļ�*/
    Char *pOtxFile;             /*��Ӫ�ļ�*/ 
    Char *pRouteDir;            /*��·�ļ�Ŀ¼*/
}S_RouteBasicRelateFile;

typedef struct
{
    Char *pRouteFile;           /*��·�ļ�*/
    Char *pRouteBleFile;        /*��·�����ļ�*/ 
}S_RouteLocateRelateFile;

typedef struct
{
    Char *pRouteFile;           /*��·�ļ�*/
    Char *pVoiceIdxFile;        /*��Ƶ�����ļ�*/
}S_RouteOperationRelateFile;

typedef struct
{
    Char *pOrbitFile;           /*�켣�ļ�*/
}S_RoutOrbitRelateFile;

Int32 RouteBasicInfoGet(S_RouteBasicRelateFile *psRelateFile,S_RouteBasicInfo *psBasicInfo[],Uint32 *pRouteTotal);
Int32 RouteBasicInfoRelease(S_RouteBasicInfo *psBasicInfo,Uint32 routeTotal);
Int32 RouteValidBasicInfoGet(S_RouteBasicInfo *psBasicInfo,Uint32 routeTotal,S_RouteValidBasicInfo *psValidBasicInfo[],Uint32 *pVRouteTotal,S_RouteTime *psNowTime);
Int32 RouteValidBasicInfoRelease(S_RouteValidBasicInfo *psVBasicInfo,Uint32 vRouteTotal);
Int32 RouteLocateInfoGet(S_RouteLocateRelateFile *psRelateFile,const Char *pRouteCoding,const Char *pServiceCoding,S_RoutePointLocateInfo *psLocateInfo[],Uint32 *pPointTotal);
Int32 RouteLocateInfoRelease(S_RoutePointLocateInfo *psLocateInfo);
Int32 RouteLocateDefDistSet(S_RoutePointLocateInfo *psInLocateInfo,Uint32 inPointTotal,Uint32 inEnterDist,Uint32 inLeaveDist);
Int32 RouteLocateDefBleRssiSet(S_RoutePointLocateInfo *psInLocateInfo,Uint32 inPointTotal,Int8 inEnterRssi,Int8 inLeaveRssi);
Int32 RouteOperationInfoGet(S_RouteOperationRelateFile *psRelateFile,const Char *pRouteCoding,const Char *pServiceCoding,S_RoutePointOpeInfo *psOpeInfo[],Uint32 *pPointTotal);
Int32 RouteOperationInfoRelease(S_RoutePointOpeInfo *psOpeInfo,Uint32 pointTotal);
Int32 RouteOrbitInfoGet(S_RoutOrbitRelateFile *psRelateFile,double pointDistance,S_RouteOrbitInfo *psOrbitInfo[],Uint32 *pPointTotal);
Int32 RouteOrbitInfoRelease(S_RouteOrbitInfo *psOrbitInfo);
Int32 RouteOpInfoUpdate(Char *pOpFile,S_RouteValidBasicInfo *psVBasicInfo,Uint32 vRouteTotal);
Int32 RouteIdxFileChk(Char *pInRouteIdxFile);

#endif
