/*****************************************************************************
  FileName：    RouteFile.h
  Author:       林锴
  Version:      1.0
  Date:         2016/4/11 13:47:06
  Description:  线路文件的应用
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.04.11    V1.0      create
*****************************************************************************/

#ifndef _ROUTE_FILE_H_
#define _ROUTE_FILE_H_

#include "OsPack.h"
#include "OsCommon.h"

#define ROUTE_CITY_MACAO

/*MC的错误定义*/
#define ROUTE_OK               1        //成功
#define ROUTE_ERROR_UNKNOW     0        //失败 - 原因未知
#define ROUTE_ERROR_NO_MEM     -1       //失败 - 内存空间错误
#define ROUTE_ERROR_FILE       -2       //失败 - 文件操作异常
#define ROUTE_ERROR_TOTAL      -3       //失败 - 线路总数错误
#define ROUTE_ERROR_PAR        -4       //失败 - 参数错误
#define ROUTE_ERROR_SEVICE     -5       //失败 - 服务不存在

/*
城市的定义
#define ROUTE_CITY_GUANGZHOU
#define ROUTE_CITY_MACAO
#define ROUTE_CITY_BEITUN
*/

/*
线路索引文件格式
000N3,N3,,000N3.rdf,2012-08-01 00:00:00,2030-12-31 00:00:00
*/

/*
线路统计文件格式
00001,00000001,2015-11-12 10:22:03
*/

#define ROUTE_LOCAL_MAX         256             /*本地保存的最多线路数*/
#define ROUTE_SETVICE_MAX       32              /*每条线路的最大服务器数*/
#define ROUTE_SERVICE_POINT_MAX 128             /*每条线路的最大站点-兴趣点数*/
#define ROUTE_POINT_VOICE_MAX   16              /*每个站点进出站语音的最大值*/

typedef enum
{
    E_CITY_GUANGZHOU            = 1,            /*广州*/
    E_CITY_MACAO                = 2,            /*澳门*/
    E_CITY_BEITUN               = 3             /*北屯*/
}E_ROUTE_CITY;

/*报站的定位方式*/
typedef enum
{
    E_POINT_LOCATE_FENCE        = 1,            /*报站定位模式 - 同心圆*/
    E_POINT_LOCATE_GATE         = 2             /*报站定位模式 - 电子门栏*/
}E_ROUTE_POINT_LOCATE_MODE;

/*定位点的类型*/
typedef enum
{
    E_POING_TYPE_NONE           = 0,            /*未知*/
    E_POING_TYPE_STA            = 1,            /*站点*/
    E_POING_TYPE_INTEREST       = 2             /*兴趣点*/ 
}E_ROUTE_POINT_TYPE;

/*GPS相关定义结构*/
typedef struct
{
    double  longitude;          /*经度,纯度格式*/
    Char    earthEW;
 	double  latitude;           /*纬度,纯度格式*/
    Char    earthNS;
}S_RoutePointGps;

/*蓝牙的相关定义结构*/
typedef struct
{
    Uint32  bleTagId;           /*BLE标签的ID*/
}S_RoutePointBle;

/*电子门栏的相关结构定义*/
typedef struct
{
    S_RoutePointGps sPointGateGpsEnterLeft;
    S_RoutePointGps sPointGateGpsEnterRight;
    S_RoutePointGps sPointGateGpsLeaveLeft;
    S_RoutePointGps sPointGateGpsLeaveRight;
}S_RoutePointGateGps;

#define ROUTE_INVALID_DISTANCE  0xA5A5              /*无效的进出站距离*/
typedef struct
{
    S_RoutePointGps             sPointGpsInfo;      /*站点的GPS坐标信息*/
    E_ROUTE_POINT_LOCATE_MODE   ePointLocateMode;   /*定位模式*/
    Uint32                      enterDistance;      /*进站距离*/
    Uint32                      leaveDistance;      /*离站距离*/
    Int32                       mileage;            /*里程计 - 定义该站点里程计的辅助参数*/
    S_RoutePointGateGps         sPointGateGpsInfo;  /*电子门栏的参数*/
}S_RoutePointGpsLocate;

#define ROUTE_INVALID_RSSI      100                  /*无效的进出站信号强度*/
typedef struct
{
    S_RoutePointBle             sPointBleInfo;       /*站点的BLE信息*/
    Int8                        enterRssi;           /*进站标签信号强度*/
    Int8                        leaveRssi;           /*离站标签信号强度*/
}S_RoutePointBlelocate;



/*站点/关键点的报站定位信息*/
typedef struct
{
    Char    pointCoding[16];      /*站点编码-兴趣点编码*/
    Char    pointName[48];        /*站点名-兴趣点名*/ 
    E_ROUTE_POINT_TYPE          ePointType;             /*类型 - 站点or兴趣点*/
    S_RoutePointGpsLocate       sPointGpsLocateInfo;    /*GPS定位的信息*/
    S_RoutePointBlelocate       sPointBleLocateInfo;    /*BLE定位的信息*/
}S_RoutePointLocateInfo;

/*语音播报相关结构*/
typedef struct
{
    Char file[64];
}S_RouteVoiceFile;

typedef struct
{
    Uint32 voiceTotal;
    void   *pVoiceFile;
}S_RoutePointVoice;

/*澳门通相关结构*/
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

/*站点/关键点的运营信息*/
typedef struct
{
    E_ROUTE_POINT_TYPE          ePointType;             /*类型 - 站点or兴趣点*/

    Char    pointCoding[32];        /*站点编码-兴趣点编码*/
    Char    enterStaName[32];       /*进站报站名称*/
    Char    leaveStaName[32];       /*出站报站名称*/
    Char    enterBroadFile[64];     /*进站报站语音*/
    Char    leaveBroadFile[64];     /*出站报站语音*/
    Char    enterLedCmd[64];        /*进站LED命令*/
    Char    leaveLedCmd[64];        /*出站LED命令*/
    Char    broadMode[8];           /*播音模式*/
    Int32   mileage;                /*里程计*/
    Char    staAdvFile[64];         /*站点广告名*/
    
    S_RoutePointVoice sRouteVoiceEnter;     /*进站的语音信息*/
    S_RoutePointVoice sRouteVoiceLeave;     /*离站语音信息*/
#ifdef ROUTE_CITY_MACAO
    /*澳门*/
    Int32   planTime;               /*计划时间,-1 表示无效*/
    Int32   speedLimit;             /*速度限制,-1 表示无效*/
    Int32   stagnateTime;           /*滞站时间,-1 表示无效*/
    Int32   crossTime;              /*甩站时间,-1 表示无效*/
    
    S_RouteStaAmtInfo sRouteStaAmtInfo;     /*澳门通信息*/
#endif
}S_RoutePointOpeInfo;

/*线路的服务信息*/
typedef struct
{
    Char    serviceCoding[2];                                   /*服务编码*/
    Char    serviceName[14];                                    /*服务名称*/
    Char    direction[48];                                      /*方向*/
    Char    departure[48];                                      /*出发地*/
    Char    destination[48];                                    /*目的地*/
    Uint32  staNums;                                            /*站点的个数*/
    Uint32  intNums;                                            /*兴趣点/关键点的个数*/
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

/*线路的基础信息*/
typedef struct
{
    Char    routeName[16];                                      /*线路名称*/
    Char    routeCoding[8];                                     /*线路编码*/
    Char    routeInfoFile[16];                                  /*线路信息文件*/
    Char    routeBtFile[16];                                    /*线路的蓝牙文件*/
    Char    routeVoiceFile[16];                                 /*线路的语音文件*/
    /*注意文件不带路径*/
    S_RouteTime sRouteEffectTime;                               /*线路生效时间*/
    S_RouteTime sRouteFailTime;                                 /*线路失效时间*/

    Int32   routeOpTimes;                                       /*线路运营次数*/
    S_RouteTime sRouteLastOpTime;                               /*线路最后一次运营的时间*/
    
    Int32   routeFileExist;                                     /*线路文件的存在的状态*/
#ifdef ROUTE_CITY_MACAO
    Char    routeAmtCoding[8];                                  /*澳门通的线路编码*/
    Char    routeFileVer[16];                                   /*线路文件的版本*/
#endif
    Uint32  serviceTotal;                                       /*线路服务总数*/
    void    *pServiceInfo;                                      /*线路服务信息*/
}S_RouteBasicInfo;

/*有效线路的基础信息*/
typedef struct
{
    Char    routeName[16];                                      /*线路名称*/
    Char    routeCoding[8];                                     /*线路编码*/
    Char    routeInfoFile[16];                                  /*线路信息文件*/
    Char    routeBtFile[16];                                    /*线路的蓝牙文件*/
    Char    routeVoiceFile[16];                                 /*线路的语音文件*/
    Char    routeOrbitFile[16];                                 /*线路的轨迹文件*/
    /*注意文件不带路径*/
    
    Int32   routeOpTimes;                                       /*线路运营次数*/
    S_RouteTime sRouteLastOpTime;                               /*线路最后一次运营的时间*/
    
#ifdef ROUTE_CITY_MACAO
    Char    routeAmtCoding[8];                                  /*澳门通的线路编码*/
    Char    routeFileVer[16];                                   /*线路文件的版本*/
#endif
    Uint32  serviceTotal;                                       /*线路服务总数*/
    void    *pServiceInfo;                                      /*线路服务信息*/
}S_RouteValidBasicInfo;

/*线路服务的轨迹信息*/
typedef struct
{
    double  latitude;           /*经度,纯度格式*/
    Char    earthNS;
    double  longitude;          /*纬度,纯度格式*/
    Char    earthEW;
    double  course;             /*方位角*/
}S_RouteOrbitInfo;

typedef struct
{
    Char *pIdxFile;             /*索引文件*/
    Char *pOtxFile;             /*运营文件*/ 
    Char *pRouteDir;            /*线路文件目录*/
}S_RouteBasicRelateFile;

typedef struct
{
    Char *pRouteFile;           /*线路文件*/
    Char *pRouteBleFile;        /*线路蓝牙文件*/ 
}S_RouteLocateRelateFile;

typedef struct
{
    Char *pRouteFile;           /*线路文件*/
    Char *pVoiceIdxFile;        /*音频索引文件*/
}S_RouteOperationRelateFile;

typedef struct
{
    Char *pOrbitFile;           /*轨迹文件*/
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
