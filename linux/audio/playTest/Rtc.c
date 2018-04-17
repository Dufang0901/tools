/*****************************************************************************
  FileName:     Rtc.c
  Author:       ����
  Version:      1.0
  Date:         2015/9/9 15:14:07
  Description:  RTC����
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2015.09.09     V1.0      create
*****************************************************************************/

#include "Rtc.h"
#include "Rtc_RX8025.h"

/*********************************************************
    Function:       RtcInit
    Description:    RTC��ʼ��
    Param:          none        
    Return:         ���
    Other:          none
*********************************************************/ 
int RtcInit(void)
{
    int rtcIicFd;
    int ret;
    
    if((rtcIicFd = open(RTC_IIC_DEV, O_RDWR)) < 0) 
    {  
        return 0;
    }
    
    if(ioctl(rtcIicFd, I2C_TIMEOUT, 2) < 0) //���ó�ʱʱ��
    {
        perror("Est Unable to set i2c timeout");
        close(rtcIicFd);
        return 0;
    }
    
    if(ioctl(rtcIicFd, I2C_RETRIES, 1) < 0)//�����ط����� 
    {
        perror("Est Unable to set i2c retries");
        close(rtcIicFd);
        return 0;
    }
    
     ret = Rtc8025Init(rtcIicFd);
     
     close(rtcIicFd);
     
     return ret;
     
}

/*********************************************************
    Function:       RtcGetTimeToSys
    Description:    RTC��RTCоƬ��ȡʱ�����õ�ϵͳ
    Param:          none        
    Return:         ���
    Other:          none
*********************************************************/ 
int RtcGetTimeToSys(void)
{
    int rtcIicFd;
    S_RtcTime sRtcTime;
    time_t timeSet;
    struct tm timeSetTm;
    int ret;
    
    if((rtcIicFd = open(RTC_IIC_DEV, O_RDWR)) < 0) 
    {  
        return 0;
    }
    
    ret = Rtc8025GetTime(rtcIicFd,&sRtcTime);
    close(rtcIicFd);
    if(ret)
    {
        timeSetTm.tm_year = sRtcTime.year + 100;
        timeSetTm.tm_mon = sRtcTime.month - 1;
        timeSetTm.tm_mday = sRtcTime.date;
        timeSetTm.tm_hour = sRtcTime.hour;
        timeSetTm.tm_min = sRtcTime.minute;
        timeSetTm.tm_sec = sRtcTime.second;
        
        timeSet = mktime(&timeSetTm);
        
        stime(&timeSet);
    }
    return ret;
}

/*********************************************************
    Function:       RtcSetTimeFromSys
    Description:    RTC��ϵͳ��ȡʱ�����õ�RTCоƬ
    Param:          none        
    Return:         ���
    Other:          none
*********************************************************/ 
int RtcSetTimeFromSys(void)
{
    int rtcIicFd;
    S_RtcTime sRtcTime;
    time_t timeGet;
    struct tm timeGetTm;
    
    if((rtcIicFd = open(RTC_IIC_DEV, O_RDWR)) < 0) 
    {  
        return 0;
    }
    
    time(&timeGet);
    localtime_r(&timeGet,&timeGetTm);
    
    sRtcTime.year = (timeGetTm.tm_year >= 100)?(timeGetTm.tm_year - 100) & 0xff:0;
    sRtcTime.month = timeGetTm.tm_mon + 1;
    sRtcTime.date = timeGetTm.tm_mday;
    sRtcTime.hour = timeGetTm.tm_hour;
    sRtcTime.minute = timeGetTm.tm_min;
    sRtcTime.second = timeGetTm.tm_sec;
    
    Rtc8025SetTime(rtcIicFd,&sRtcTime);
    close(rtcIicFd);
    
    return 1;
}
