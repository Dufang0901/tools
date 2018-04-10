/*****************************************************************************
  FileName:     VoiceLib.h                                               
  Author:       zhulu                                                          
  Version:      1.0                                                           
  Date:         2016/05/23                               
  Description:  �������ſ�
  History:                                                                    
                <author>    <time>    <version>    <desc>                     
                 zhulu    2016.05.23     V1.0      create                     
*****************************************************************************/

#ifndef _VOICE_LIB_HH_H_
#define _VOICE_LIB_HH_H_


#include "Mytype.h"



#define VOICE_LIB_MAX_H_VOL		9	//��������������ڵȼ�[0-9]
#define VOICE_LIB_MAX_S_VOL		7	//�������������ڵȼ�[0-7]
#define VOICE_LIB_MAX_VOL		(VOICE_LIB_MAX_H_VOL + VOICE_LIB_MAX_S_VOL)	//���������ڵȼ�[0-16]



//���������������
typedef enum
{
   	E_VOICE_LIB_VOLUME_L = 0,    //��������������     
   	E_VOICE_LIB_VOLUME_R = 1,    //��������������
}E_VoiceLibVolumeType;


//���������������
typedef enum
{
	E_VOICE_LIB_CHANNEL_LEFT   = 0,	//������
	E_VOICE_LIB_CHANNEL_RIGHT  = 1,	//������
}E_VoiceLibChannelType;


//������Ĳ���״̬
typedef enum
{
	E_VOICE_LIB_STA_STOP      = 0,	//ֹͣ����
	E_VOICE_LIB_STA_PLAY  	  = 1,	//���ڲ���
	E_VOICE_LIB_STA_PAUSE 	  = 2,	//��ͣ����
}E_VoiceLibSta;


typedef void   (*CallBackVoicePlayStaFun)(E_VoiceLibChannelType lr,E_VoiceLibSta sta);
typedef void   (*CallBackVoiceLibPrintFun)(const char* info,...);


//������ĳ�ʼ������
typedef struct
{
	Uint8  isChannelFlag;		//0��ʾ������,1��ʾ������,2��ʾ������
	Uint8  volumeLValue;		//����������[0-16]   4-��������
	Uint8  volumeRValue;		//����������[0-16]   4-��������
}S_VoiceLibOpenParam;


//��������
int VoiceLibOpen(void);


/*********************************************************
    Function:       VoiceLibGetVer
    Description:    ��ȡ�汾��Ϣ
    Param:          ver	   ��Ű汾��Ϣ
    Return:         1-�ɹ�,0-ʧ��
    Other:          ver="x.x",��������Ϊ8
*********************************************************/
int VoiceLibGetVer(char *ver); 


/*********************************************************
    Function:       VoiceLibVolumeSet
    Description:    ��������
    Param:          type		E_VoiceLibVolumeType
    				value		����ֵ[0-9]
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibVolumeSet(E_VoiceLibVolumeType type,Uint8 value);

/*********************************************************
    Function:       VoiceLibPlayReq
    Description:    ������������
    Param:          type	   E_VoiceLibChannelType
    				fileName   �����ļ���
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibPlayReq(E_VoiceLibChannelType type,char *fileName); 

/*********************************************************
    Function:       VoiceLibPlayStop
    Description:    ����ֹͣ��������
    Param:          type	   E_VoiceLibChannelType
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibPlayStop(E_VoiceLibChannelType type); 

/*********************************************************
    Function:       VoiceLibPlayPause
    Description:    ������ͣ����
    Param:          type	   E_VoiceLibChannelType
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibPlayPause(E_VoiceLibChannelType type); 

/*********************************************************
    Function:       voicePlayResume
    Description:    �����ָ�����
    Param:          type	   E_VoiceLibChannelType
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibPlayResume(E_VoiceLibChannelType type); 

/*********************************************************
    Function:       VoiceLibGetPlaySta
    Description:    ��ȡ��ǰ����״̬
    Param:          lPlaySta	��������������״̬	   
    				RPlaySta	��������������״̬	   
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibGetPlaySta(E_VoiceLibSta *lPlaySta,E_VoiceLibSta *rPlaySta); 


/*********************************************************
    Function:       VoiceLibSwichChannel
    Description:    �����л�
    Param:          value     0-������,1-������,2-������
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibSwichChannel(Uint8 value); 

#endif _VOICE_LIB_HH_H_
