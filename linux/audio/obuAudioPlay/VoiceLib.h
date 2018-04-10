/*****************************************************************************
  FileName:     VoiceLib.h                                               
  Author:       zhulu                                                          
  Version:      1.0                                                           
  Date:         2016/05/23                               
  Description:  语音播放库
  History:                                                                    
                <author>    <time>    <version>    <desc>                     
                 zhulu    2016.05.23     V1.0      create                     
*****************************************************************************/

#ifndef _VOICE_LIB_HH_H_
#define _VOICE_LIB_HH_H_


#include "Mytype.h"



#define VOICE_LIB_MAX_H_VOL		9	//驱动最大音量调节等级[0-9]
#define VOICE_LIB_MAX_S_VOL		7	//软件最大音量调节等级[0-7]
#define VOICE_LIB_MAX_VOL		(VOICE_LIB_MAX_H_VOL + VOICE_LIB_MAX_S_VOL)	//音量最大调节等级[0-16]



//语音库的音量类型
typedef enum
{
   	E_VOICE_LIB_VOLUME_L = 0,    //音量左声道类型     
   	E_VOICE_LIB_VOLUME_R = 1,    //音量右声道类型
}E_VoiceLibVolumeType;


//语音库的声道类型
typedef enum
{
	E_VOICE_LIB_CHANNEL_LEFT   = 0,	//左声道
	E_VOICE_LIB_CHANNEL_RIGHT  = 1,	//右声道
}E_VoiceLibChannelType;


//语音库的播放状态
typedef enum
{
	E_VOICE_LIB_STA_STOP      = 0,	//停止播放
	E_VOICE_LIB_STA_PLAY  	  = 1,	//正在播放
	E_VOICE_LIB_STA_PAUSE 	  = 2,	//暂停播放
}E_VoiceLibSta;


typedef void   (*CallBackVoicePlayStaFun)(E_VoiceLibChannelType lr,E_VoiceLibSta sta);
typedef void   (*CallBackVoiceLibPrintFun)(const char* info,...);


//语音库的初始化参数
typedef struct
{
	Uint8  isChannelFlag;		//0表示左声道,1表示右声道,2表示立体音
	Uint8  volumeLValue;		//左声道音量[0-16]   4-音量不变
	Uint8  volumeRValue;		//右声道音量[0-16]   4-音量不变
}S_VoiceLibOpenParam;


//打开语音库
int VoiceLibOpen(void);


/*********************************************************
    Function:       VoiceLibGetVer
    Description:    获取版本信息
    Param:          ver	   存放版本信息
    Return:         1-成功,0-失败
    Other:          ver="x.x",长度至少为8
*********************************************************/
int VoiceLibGetVer(char *ver); 


/*********************************************************
    Function:       VoiceLibVolumeSet
    Description:    设置音量
    Param:          type		E_VoiceLibVolumeType
    				value		音量值[0-9]
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibVolumeSet(E_VoiceLibVolumeType type,Uint8 value);

/*********************************************************
    Function:       VoiceLibPlayReq
    Description:    语音播放请求
    Param:          type	   E_VoiceLibChannelType
    				fileName   播放文件名
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibPlayReq(E_VoiceLibChannelType type,char *fileName); 

/*********************************************************
    Function:       VoiceLibPlayStop
    Description:    语音停止播放请求
    Param:          type	   E_VoiceLibChannelType
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibPlayStop(E_VoiceLibChannelType type); 

/*********************************************************
    Function:       VoiceLibPlayPause
    Description:    语音暂停播放
    Param:          type	   E_VoiceLibChannelType
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibPlayPause(E_VoiceLibChannelType type); 

/*********************************************************
    Function:       voicePlayResume
    Description:    语音恢复播放
    Param:          type	   E_VoiceLibChannelType
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibPlayResume(E_VoiceLibChannelType type); 

/*********************************************************
    Function:       VoiceLibGetPlaySta
    Description:    获取当前播放状态
    Param:          lPlaySta	保存左声道播放状态	   
    				RPlaySta	保存右声道播放状态	   
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibGetPlaySta(E_VoiceLibSta *lPlaySta,E_VoiceLibSta *rPlaySta); 


/*********************************************************
    Function:       VoiceLibSwichChannel
    Description:    声道切换
    Param:          value     0-左声道,1-右声道,2-立体声
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibSwichChannel(Uint8 value); 

#endif _VOICE_LIB_HH_H_
