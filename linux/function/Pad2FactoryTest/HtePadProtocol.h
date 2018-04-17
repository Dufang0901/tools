/*****************************************************************************  
FileName:     HtePadProtocol.h  
Author:       �ź�  
Version:      1.0  
Date:         2017/02/17 16:05:43  
Description:  �ڶ���pad����HTEЭ��  
History:      <author>    <time>    <version>    <desc>                 
               duh      2017.02.17     V1.0      create
*****************************************************************************/
#ifndef _HTE_PAD_PROTOCOL_H_
#define _HTE_PAD_PROTOCOL_H_

#include "MyType.h"

/*��Ϣͷ*/
#define HTE_MSG_HEAD0               'H'                 /*��Ϣͷ0*/
#define HTE_MSG_HEAD1               'T'                 /*��Ϣͷ1*/
/*��Ϣ����*/
#define HTE_MSG_TYPE_NONE           0x00                /*һ����Ϣ*/
#define HTE_MSG_TYPE_REQ            0x01                /*������Ϣ*/
#define HTE_MSG_TYPE_RPLY           0x02                /*Ӧ����Ϣ*/
/*�豸����*/
#define HTE_MSG_DEV_UNKNOW          0x00                /*�豸δ֪*/
#define HTE_MSG_DEV_BIC             0x01                /*������Ϣ����*/
#define HTE_MSG_DEV_OBU             0x02                /*���ص�Ԫ*/
#define HTE_MSG_DEV_BGB             0x03                /*���ص�ä�ն�*/
#define HTE_MSG_DEV_PAD1            0x04                /*��һ��������*/
#define HTE_MSG_DEV_PAD2            0x05                /*�ڶ���������*/
#define HTE_MSG_DEV_KB              0x06                /*���ؼ���*/
#define HTE_MSG_DEV_LES             0x60                /*�Ͷ˵���վ��*/
#define HTE_MSG_DEV_HES             0x61                /*�߶˵���վ��*/
#define HTE_MSG_DEV_IES             0x62                /*����վͤ*/
#define HTE_MSG_DEV_RN              0x90                /*·��ڵ�*/
#define HTE_MSG_DEV_CBLE            0xE0                /*ͨ������ģ��*/

#define HTE_MSG_HEAD_LEN            sizeof(S_HteMsgHead)/*��Ϣͷ�ṹ*/

/*Э��֡���Ͷ���*/
/*pad -> other device*/
#define HTE_PAD_HEARTBEAT           0X71        /*ͨ������*/
#define HTE_PAD_POWER_ON            0x01        /*�豸����*/
#define HTE_PAD_SELF_CHECK          0x02        /*�豸�Լ�*/
#define HTE_PAD_KEY_DOWN            0x11        /*���̰���*/
#define HTE_PAD_KEY_HOLD            0x12        /*���̳���*/
#define HTE_PAD_KEY_RELEASE         0x13        /*�����ͷ�*/
#define HTE_PAD_TS_DOWN             0x21        /*����������*/
#define HTE_PAD_TS_RELEASE          0x22        /*�������ͷ�*/
#define HTE_PAD_TS_COORD            0x23        /*����������*/
#define HTE_PAD_MIC_STATE           0x41        /*����״̬*/


/*other device -> pad*/
#define HTE_PAD_BL_SET              0x31        /*��������*/
#define HTE_PAD_MODE_SET            0x32        /*��ʾģʽ����*/
#define HTE_PAD_RESTART             0x3A        /*�豸����*/
#define HTE_PAD_VERSION_QUERY       0x3B        /*�豸�汾��ѯ*/
#define HTE_PAD_SCREEN_REVERSE      0x3E        /*��ʾ����ת*/
#define HTE_PAD_BUZZER_CONTROL      0x35        /*����������*/

/*Э��֡�ṹ�嶨��*/

/*HTEЭ��֡��Ϣͷ*/
typedef struct{    
Uint8 head[2];                  /*��Ϣͷ*/    
Uint8 srcDevType;               /*Դ�豸����*/    
Uint8 srcDevId[3];              /*Դ�豸ID*/    
Uint8 dstDevType;               /*Ŀ���豸����*/    
Uint8 dstDevId[3];              /*Ŀ���豸ID*/    
Uint8 sn[4];                    /*��ˮ��*/    
Uint8 type;                     /*��Ϣ����*/    
Uint8 cmd;                      /*����*/    
Uint8 msgLen[2];                /*��Ϣ����*/    
Uint8 crc[2];                   /*У��ֵ*/
}S_HteMsgHead;

/*�豸������Ϣ*/
typedef struct{
    Char hwVer[16];             /*pad��Ӳ���汾��Ϣ*/
    Char swVer[16];             /*pad������汾��Ϣ*/
}S_HtePadPowerOn;

/*�豸����*/
typedef struct{
    Int32 devState;             /*�豸״̬*/
}S_HetPadHb;

/*���̰�����Ŷ���*/
#define KEY_WEISHENG     0x05       /*����*/
#define KEY_HOUZOU       0x06       /*����*/
#define KEY_GUZHANG      0x07       /*����*/
#define KEY_BAOJING      0x08       /*����*/
#define KEY_JINZHAN      0x00       /*��վ*/
#define KEY_QUXIAO       0x01       /*ȡ��*/
#define KEY_UP           0x02       /*��*/
#define KEY_QUEREN       0x04       /*ȷ��*/
#define KEY_FN           0x0A       /*FN*/
#define KEY_RANGZUO      0x0B       /*����*/
#define KEY_ZHUANWAN     0x0C       /*ת��*/
#define KEY_CAIDAN       0x0D       /*�˵�*/
#define KEY_CHUZHAN      0x0F       /*��վ*/
#define KEY_LEFT         0x10       /*��*/
#define KEY_DOWN         0x11       /*��*/
#define KEY_RIGHT        0x13       /*��*/

/*���̰��¡��������ͷŵİ������*/
typedef struct{
    Uint8 keyId;                /*�������*/
}S_HtePadKeyId;

/*����������*/
typedef struct{
    Uint16 xVoltage;            /*x���ѹ����ֵ*/
    Uint16 yVoltage;            /*y���ѹ����ֵ*/
}S_HtePadTsDown;

/*������������Ϣ*/
typedef struct{
    Uint16 xVoltage;            /*x���ѹ����ֵ*/
    Uint16 yVoltage;            /*y���ѹ����ֵ*/
}S_HtePadTsCoord;

/*��˷�״̬*/
typedef struct{
    Int32 micState;             /*����״̬ 0-�ͷ� 1-����*/
}S_HtePadMicState;

/*��������*/
typedef struct{
    Uint8 blValue;              /*��������ֵ,ȡֵ0-10,0-�رձ��� 10-����*/
}S_HtePadBlSet;

/*��ʾģʽ����*/
typedef struct{
    Uint8 displayMode;          /*��ʾģʽ*/
}S_HtePadModeSet;

/*��ʾģʽ����Ӧ��*/
typedef struct{
    Uint8 result;               /*���ý�� 0x10-�л��ɹ� 0x01-�л�ʧ��*/
}S_HtePadModeSetRply;

/*�豸����*/
typedef struct{
    Uint8 type;                 /*�������� 0-��������  1-��ʾģ������*/
}S_HtePadRestart;

/*�汾�Ų�ѯ*/
typedef struct{
    Uint8 devType;              /*��ѯ�豸 0-stm32 1-tw8836*/
}S_HtePadVerQuery;

/*�汾��ѯӦ��*/
typedef struct{
    Char hwVer[16];             /*��ѯ�豸��Ӳ���汾��Ϣ*/
    Char swVer[16];             /*��ѯ�豸������汾��Ϣ*/
}S_HtePadVerion;

/*��Ļ��ת*/
typedef struct{
    Uint8 reverseType;          /*��ת���� 0-���·�ת 1-���ҷ�ת*/
}S_HtePadReverse;

/*����������*/
typedef struct{
    Uint8 cmd;                  /*�������� 0-�� 1-��*/
}S_HtePadBuzCtrl;




#endif


