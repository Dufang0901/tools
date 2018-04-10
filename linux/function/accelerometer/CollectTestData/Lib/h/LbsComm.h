/*****************************************************************************
  FileName：    lbsComm.h
  Author:       佘旭佳
  Version:      1.0
  Date:         
  Description:  lbs应用的共用接口
  History:  
                <author>    <time>    <version>    <desc>
                    V1.0      create
*****************************************************************************/

#ifndef _LBS_Comm_H_
#define _LBS_Comm_H_

#include "OsPack.h"
#include "OsCommon.h"

/*LBS的错误定义*/
#define LBS_OK                  1                   //成功
#define LBS_ERROR_UNKNOW        0                   //失败 - 原因未知
#define LBS_ERROR_NO_MEM        -1                  //失败 - 内存空间错误
#define LBS_ERROR_ROCK          -2                  //失败 - 读写锁异常
#define LBS_ERROR_THREAD        -3                  //失败 - 线程失败
#define LBS_ERROR_FD            -4                  //失败 - 文件描述符
#define LBS_ERROR_FILE          -5                  //失败 - 文件操作异常
#define LBS_ERROR_NO_INIT       -6                  //失败 - 未初始化
#define LBS_ERROR_PARAM         -7                  //失败 - 参数错误
#define LBS_ERROR_NO_SUPPORT    -8                  //失败 - 不支持

/*LBS的上传标示位*/
#define	SCAN_NO_THING		 	    0	    	
#define	SCAN_ERR_FLYSPOT		    (0x01<<8)	   	    //飞点
#define SCAN_ERR_DEVIATE_START      (0x01<<9)    	    //偏离
#define SCAN_ERR_DEVIATE_RM         (0x01<<10)    	    //偏离结束
#define	SCAN_REPORT_POINT_IN	    (0x01<<6)		    //上传进入兴趣点

#define SCAN_REPORT_STA_MIDDLE	    (0x01<<11)		    //进入2站中间
#define SCAN_REPORT_STA_INING_DR    (0X01<<12)          //上传进站中(DR)
#define SCAN_REPORT_STA_OUTING_DR   (0X01<<13)          //上传出站中(DR)
#define	SCAN_REPORT_STA_IN_DR       (0x01<<14)		    //上传进站(DR)
#define	SCAN_REPORT_STA_OUT_DR	    (0x01<<15)		    //上传出站(DR)
#define SCAN_REPORT_STA_INING_GPS   (0X01<<16)          //上传进站中(GPS)
#define SCAN_REPORT_STA_OUTING_GPS  (0X01<<17)          //上传出站中(GPS)
#define	SCAN_REPORT_STA_IN_GPS	    (0x01<<18)		    //上传进站(GPS)
#define	SCAN_REPORT_STA_OUT_GPS	    (0x01<<19)		    //上传出站(GPS)
#define	SCAN_REPORT_STA_IN_BLE	    (0x01<<20)		    //上传进站(BLE)
#define	SCAN_REPORT_STA_OUT_BLE	    (0x01<<21)		    //上传出站(BLE)
#define	SCAN_REPORT_STA_IN_ZB	    (0x01<<22)		    //上传进站(ZIGBEE)
#define	SCAN_REPORT_STA_OUT_ZB	    (0x01<<23)		    //上传出站(ZIGBEE)

/*站点类型*/
typedef enum
{
    E_ROUTE_SERVICE_NONE,                                  // 无效    
    E_ROUTE_SERVICE_POINT,                                 // 兴趣点
    E_ROUTE_SERVICE_STA,                                   // 公交站台
    E_ROUTE_SERVICE_TRACK                                  // 轨迹参考点
}E_ROUTE_SERVICE_TYPE;

/*报站算法类型*/
typedef enum {
    E_LOCATE_MODE_FENCE,                                   // 同心圆
    E_LOCATE_MODE_GATE,                                    // 电子门栏
}E_ROUTE_LOCATE_MODE;

/*位置服务的详细上报内容*/
typedef struct
{
    double                  longitude;                     // 经度,纯度格式
    double                  latitude;                      // 纬度,纯度格式
    float                   course;                        // 方位角
	Char                    earthNS;    
    Char                    earthEW;
    Uint8                   repMode;                       // 报站方式
        
    /*站台/关键点名称*/
    E_ROUTE_SERVICE_TYPE    pointType;                     // 站点类型
    Char                    staCoding[14];                 // 站点编码-兴趣点编码
    Char                    staName[48];                   // 站点名-兴趣点名
    Uint32                  btTagId;                       // 站点蓝牙设备ID(空则填0)  
}S_LbsRepDetail;

/*Zibee有效数据*/
typedef struct {
    Uint32                   msgId;                        // 消息序号
    Uint32                   zbbDevId;                     // 路边标签id
    Uint32                   clientId;                     // 接收设备id
    double                   longitude;                    // 经度,纯度格式
    double                   latitude;                     // 纬度
    Char                     earthEW;    
    Char                     earthNS;
}S_ZbbrdStaTagInfo;  

typedef struct
{
#define MAX_ZBBRD_SCAN_CNT   10
    Uint32                   tagCnt;                       // 当前扫描到的tag总数
    S_ZbbrdStaTagInfo        tag[MAX_ZBBRD_SCAN_CNT];      // tag细节
}S_ZbbrdScanInfo;

/*ble有效数据*/
typedef struct
{
    Uint32                   tagId;                        // 设备ID
    Int8                     rssi;                         // 信号值
}S_BleStaTagInfo;

typedef struct
{
#define MAX_BLE_SCAN_CNT     10
    Uint32                   tagCnt;                       // 当前扫描到的tag总数
    S_BleStaTagInfo          tag[MAX_BLE_SCAN_CNT];        // tag细节
}S_BleScanInfo;

/*gps有效数据*/
typedef struct
{
    double                   latitude;                     // 经度,纯度格式
    double                   longitude;                    // 纬度
    Char                     earthNS;                    
    Char                     earthEW;
    float                    speed;                        // 车速 km/h
    float                    course;                       // 航向角
}S_GpsLocateInfo;

#define TIMINIG_INFO_ZBBRD_MODE   (0x01<<1)                // zbbrd数据有效
#define TIMINIG_INFO_BLE_MODE     (0x01<<2)                // ble数据有效
#define TIMINIG_INFO_GPS_MODE     (0x01<<3)                // gps数据有效
#define TIMINIG_INFO_DR_MODE      (0x01<<4)                // dr数据有效
/*定时采集的定位信息*/
typedef struct
{
    S_ZbbrdScanInfo          sZbbrdScanInfo;               // Zibee有效数据                                
    S_BleScanInfo            sBleScanInfo;                 // ble有效数据
    S_GpsLocateInfo          sGpsLocateInfo;               // gps有效数据
    S_GpsLocateInfo          sDrLocateInfo;                // dr有效数据
    Uint32                   mode;                         // 定位信息有效数据类型                
    Uint32                   utcTime;                      // UTC时间,BCD码
}S_LbsTimingInfo;

/*电子门栏的相关结构定义*/
typedef struct
{
    S_GpsLocateInfo          sGpsLocateInfoEnterLeft;      // 进栏左坐标
    S_GpsLocateInfo          sGpsLocateInfoEnterRight;     // 进栏右坐标 
    S_GpsLocateInfo          sGpsLocateInfoLeaveLeft;      // 出栏左坐标
    S_GpsLocateInfo          sGpsLocateInfoLeaveRight;     // 出栏右坐标
}S_GateInfo;

/*站点/关键点/轨迹的位置信息*/
typedef struct
{
    E_ROUTE_SERVICE_TYPE     eType;                        // 站点类型
        
    double                   longitude;                    // 经度,纯度格式
    double                   latitude;                     // 纬度,纯度格式
    float                    course;                       // 方位角
	Char                     earthNS;
    Char                     earthEW;
    
    Char                     staCoding[14];                // 站点编码-兴趣点编码
    Char                     staName[48];                  // 站点名-兴趣点名
    E_ROUTE_LOCATE_MODE      locateMode;                   // 定位方式
    Uint32                   enterDistance;                // 进站距离
    Uint32                   leaveDistance;                // 离站距离
    S_GateInfo               sGateInfo;                    // 电子门栏的参数,创建时请初始化为NULL        
	Uint32                   btTagId;                      // 蓝牙标签ID
	Int8                     enterBtRssi;                  // 蓝牙标签进站强度
	Int8                     leaveBtRssi;                  // 蓝牙标签离站强度
}S_LbsRoutePointLocateInfo;



typedef void       (*LbsStaRepResHandler) (Int32 res, S_LbsRepDetail *psLbsRepDetail); /*lbs库上报关键信息*/
typedef void       (*LbsLogPrintfHandler) (const char *info);                          /*lbs库上报打印信息*/

/*位置服务的配置参数*/
typedef struct
{
    LbsStaRepResHandler      HandlerRepRes;                // 位置服务上报需求回调句柄
    LbsLogPrintfHandler      HandlerLogPrintf;             // 打印调试信息的回调句柄
        
    S_LbsTimingInfo          *pLbsTimingInfo;              // 位置信息
    pthread_rwlock_t         *lbsWRLock;                   // 读写锁
    Uint32                    lbsUpdateTime;                // 监控的轮询速度
}S_LbsCfgPar;

/*********************************************************
    Function:       LbsCommInit
    Description:    lbs 初始化函数
    Param:          psLbsInitPar    配置参数结构体      
    Return:         结果
    Other:          none
*********************************************************/
Int32 LbsCommInit(S_LbsCfgPar *psLbsInitPar);


/*********************************************************
    Function:       LbsCommCreatNewRoute
    Description:    lbs 创建站台\兴趣点\轨迹AP 节点链表
    Param:          routeAllNums        输入节点的数量   
                    **psLocateInfos     输入节点的信息列表
                    *curStaCoding       开始搜索报站的站台编号
    Return:         结果
    Other:          none
*********************************************************/
Int32 LbsCommCreatNewRoute(Uint32 routeAllNums, 
    S_LbsRoutePointLocateInfo *psLocateInfos,
    const Char  *curStaCoding);


/*********************************************************
    Function:       LbsCommStart
    Description:    lbs 启动全部监控
    Param:          void
    Return:         结果
    Other:          none
*********************************************************/
Int32 LbsCommStart(void);


/********************************************************
    Function:       LbsCommCfgParCheck
    Description:    配置参数检查
    Param:          psLbsCfgPar 配置参数
    Return:         结果
    Other:          none
*********************************************************/
Int32 LbsCommCance(void);


#endif








