/*****************************************************************************
  FileName��    lbsComm.h
  Author:       �����
  Version:      1.0
  Date:         
  Description:  lbsӦ�õĹ��ýӿ�
  History:  
                <author>    <time>    <version>    <desc>
                    V1.0      create
*****************************************************************************/

#ifndef _LBS_Comm_H_
#define _LBS_Comm_H_

#include "OsPack.h"
#include "OsCommon.h"

/*LBS�Ĵ�����*/
#define LBS_OK                  1                   //�ɹ�
#define LBS_ERROR_UNKNOW        0                   //ʧ�� - ԭ��δ֪
#define LBS_ERROR_NO_MEM        -1                  //ʧ�� - �ڴ�ռ����
#define LBS_ERROR_ROCK          -2                  //ʧ�� - ��д���쳣
#define LBS_ERROR_THREAD        -3                  //ʧ�� - �߳�ʧ��
#define LBS_ERROR_FD            -4                  //ʧ�� - �ļ�������
#define LBS_ERROR_FILE          -5                  //ʧ�� - �ļ������쳣
#define LBS_ERROR_NO_INIT       -6                  //ʧ�� - δ��ʼ��
#define LBS_ERROR_PARAM         -7                  //ʧ�� - ��������
#define LBS_ERROR_NO_SUPPORT    -8                  //ʧ�� - ��֧��

/*LBS���ϴ���ʾλ*/
#define	SCAN_NO_THING		 	    0	    	
#define	SCAN_ERR_FLYSPOT		    (0x01<<8)	   	    //�ɵ�
#define SCAN_ERR_DEVIATE_START      (0x01<<9)    	    //ƫ��
#define SCAN_ERR_DEVIATE_RM         (0x01<<10)    	    //ƫ�����
#define	SCAN_REPORT_POINT_IN	    (0x01<<6)		    //�ϴ�������Ȥ��

#define SCAN_REPORT_STA_MIDDLE	    (0x01<<11)		    //����2վ�м�
#define SCAN_REPORT_STA_INING_DR    (0X01<<12)          //�ϴ���վ��(DR)
#define SCAN_REPORT_STA_OUTING_DR   (0X01<<13)          //�ϴ���վ��(DR)
#define	SCAN_REPORT_STA_IN_DR       (0x01<<14)		    //�ϴ���վ(DR)
#define	SCAN_REPORT_STA_OUT_DR	    (0x01<<15)		    //�ϴ���վ(DR)
#define SCAN_REPORT_STA_INING_GPS   (0X01<<16)          //�ϴ���վ��(GPS)
#define SCAN_REPORT_STA_OUTING_GPS  (0X01<<17)          //�ϴ���վ��(GPS)
#define	SCAN_REPORT_STA_IN_GPS	    (0x01<<18)		    //�ϴ���վ(GPS)
#define	SCAN_REPORT_STA_OUT_GPS	    (0x01<<19)		    //�ϴ���վ(GPS)
#define	SCAN_REPORT_STA_IN_BLE	    (0x01<<20)		    //�ϴ���վ(BLE)
#define	SCAN_REPORT_STA_OUT_BLE	    (0x01<<21)		    //�ϴ���վ(BLE)
#define	SCAN_REPORT_STA_IN_ZB	    (0x01<<22)		    //�ϴ���վ(ZIGBEE)
#define	SCAN_REPORT_STA_OUT_ZB	    (0x01<<23)		    //�ϴ���վ(ZIGBEE)

/*վ������*/
typedef enum
{
    E_ROUTE_SERVICE_NONE,                                  // ��Ч    
    E_ROUTE_SERVICE_POINT,                                 // ��Ȥ��
    E_ROUTE_SERVICE_STA,                                   // ����վ̨
    E_ROUTE_SERVICE_TRACK                                  // �켣�ο���
}E_ROUTE_SERVICE_TYPE;

/*��վ�㷨����*/
typedef enum {
    E_LOCATE_MODE_FENCE,                                   // ͬ��Բ
    E_LOCATE_MODE_GATE,                                    // ��������
}E_ROUTE_LOCATE_MODE;

/*λ�÷������ϸ�ϱ�����*/
typedef struct
{
    double                  longitude;                     // ����,���ȸ�ʽ
    double                  latitude;                      // γ��,���ȸ�ʽ
    float                   course;                        // ��λ��
	Char                    earthNS;    
    Char                    earthEW;
    Uint8                   repMode;                       // ��վ��ʽ
        
    /*վ̨/�ؼ�������*/
    E_ROUTE_SERVICE_TYPE    pointType;                     // վ������
    Char                    staCoding[14];                 // վ�����-��Ȥ�����
    Char                    staName[48];                   // վ����-��Ȥ����
    Uint32                  btTagId;                       // վ�������豸ID(������0)  
}S_LbsRepDetail;

/*Zibee��Ч����*/
typedef struct {
    Uint32                   msgId;                        // ��Ϣ���
    Uint32                   zbbDevId;                     // ·�߱�ǩid
    Uint32                   clientId;                     // �����豸id
    double                   longitude;                    // ����,���ȸ�ʽ
    double                   latitude;                     // γ��
    Char                     earthEW;    
    Char                     earthNS;
}S_ZbbrdStaTagInfo;  

typedef struct
{
#define MAX_ZBBRD_SCAN_CNT   10
    Uint32                   tagCnt;                       // ��ǰɨ�赽��tag����
    S_ZbbrdStaTagInfo        tag[MAX_ZBBRD_SCAN_CNT];      // tagϸ��
}S_ZbbrdScanInfo;

/*ble��Ч����*/
typedef struct
{
    Uint32                   tagId;                        // �豸ID
    Int8                     rssi;                         // �ź�ֵ
}S_BleStaTagInfo;

typedef struct
{
#define MAX_BLE_SCAN_CNT     10
    Uint32                   tagCnt;                       // ��ǰɨ�赽��tag����
    S_BleStaTagInfo          tag[MAX_BLE_SCAN_CNT];        // tagϸ��
}S_BleScanInfo;

/*gps��Ч����*/
typedef struct
{
    double                   latitude;                     // ����,���ȸ�ʽ
    double                   longitude;                    // γ��
    Char                     earthNS;                    
    Char                     earthEW;
    float                    speed;                        // ���� km/h
    float                    course;                       // �����
}S_GpsLocateInfo;

#define TIMINIG_INFO_ZBBRD_MODE   (0x01<<1)                // zbbrd������Ч
#define TIMINIG_INFO_BLE_MODE     (0x01<<2)                // ble������Ч
#define TIMINIG_INFO_GPS_MODE     (0x01<<3)                // gps������Ч
#define TIMINIG_INFO_DR_MODE      (0x01<<4)                // dr������Ч
/*��ʱ�ɼ��Ķ�λ��Ϣ*/
typedef struct
{
    S_ZbbrdScanInfo          sZbbrdScanInfo;               // Zibee��Ч����                                
    S_BleScanInfo            sBleScanInfo;                 // ble��Ч����
    S_GpsLocateInfo          sGpsLocateInfo;               // gps��Ч����
    S_GpsLocateInfo          sDrLocateInfo;                // dr��Ч����
    Uint32                   mode;                         // ��λ��Ϣ��Ч��������                
    Uint32                   utcTime;                      // UTCʱ��,BCD��
}S_LbsTimingInfo;

/*������������ؽṹ����*/
typedef struct
{
    S_GpsLocateInfo          sGpsLocateInfoEnterLeft;      // ����������
    S_GpsLocateInfo          sGpsLocateInfoEnterRight;     // ���������� 
    S_GpsLocateInfo          sGpsLocateInfoLeaveLeft;      // ����������
    S_GpsLocateInfo          sGpsLocateInfoLeaveRight;     // ����������
}S_GateInfo;

/*վ��/�ؼ���/�켣��λ����Ϣ*/
typedef struct
{
    E_ROUTE_SERVICE_TYPE     eType;                        // վ������
        
    double                   longitude;                    // ����,���ȸ�ʽ
    double                   latitude;                     // γ��,���ȸ�ʽ
    float                    course;                       // ��λ��
	Char                     earthNS;
    Char                     earthEW;
    
    Char                     staCoding[14];                // վ�����-��Ȥ�����
    Char                     staName[48];                  // վ����-��Ȥ����
    E_ROUTE_LOCATE_MODE      locateMode;                   // ��λ��ʽ
    Uint32                   enterDistance;                // ��վ����
    Uint32                   leaveDistance;                // ��վ����
    S_GateInfo               sGateInfo;                    // ���������Ĳ���,����ʱ���ʼ��ΪNULL        
	Uint32                   btTagId;                      // ������ǩID
	Int8                     enterBtRssi;                  // ������ǩ��վǿ��
	Int8                     leaveBtRssi;                  // ������ǩ��վǿ��
}S_LbsRoutePointLocateInfo;



typedef void       (*LbsStaRepResHandler) (Int32 res, S_LbsRepDetail *psLbsRepDetail); /*lbs���ϱ��ؼ���Ϣ*/
typedef void       (*LbsLogPrintfHandler) (const char *info);                          /*lbs���ϱ���ӡ��Ϣ*/

/*λ�÷�������ò���*/
typedef struct
{
    LbsStaRepResHandler      HandlerRepRes;                // λ�÷����ϱ�����ص����
    LbsLogPrintfHandler      HandlerLogPrintf;             // ��ӡ������Ϣ�Ļص����
        
    S_LbsTimingInfo          *pLbsTimingInfo;              // λ����Ϣ
    pthread_rwlock_t         *lbsWRLock;                   // ��д��
    Uint32                    lbsUpdateTime;                // ��ص���ѯ�ٶ�
}S_LbsCfgPar;

/*********************************************************
    Function:       LbsCommInit
    Description:    lbs ��ʼ������
    Param:          psLbsInitPar    ���ò����ṹ��      
    Return:         ���
    Other:          none
*********************************************************/
Int32 LbsCommInit(S_LbsCfgPar *psLbsInitPar);


/*********************************************************
    Function:       LbsCommCreatNewRoute
    Description:    lbs ����վ̨\��Ȥ��\�켣AP �ڵ�����
    Param:          routeAllNums        ����ڵ������   
                    **psLocateInfos     ����ڵ����Ϣ�б�
                    *curStaCoding       ��ʼ������վ��վ̨���
    Return:         ���
    Other:          none
*********************************************************/
Int32 LbsCommCreatNewRoute(Uint32 routeAllNums, 
    S_LbsRoutePointLocateInfo *psLocateInfos,
    const Char  *curStaCoding);


/*********************************************************
    Function:       LbsCommStart
    Description:    lbs ����ȫ�����
    Param:          void
    Return:         ���
    Other:          none
*********************************************************/
Int32 LbsCommStart(void);


/********************************************************
    Function:       LbsCommCfgParCheck
    Description:    ���ò������
    Param:          psLbsCfgPar ���ò���
    Return:         ���
    Other:          none
*********************************************************/
Int32 LbsCommCance(void);


#endif








