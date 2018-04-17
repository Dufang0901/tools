/*****************************************************************************
  FileName:     Rtc_RX8025.c
  Author:       ����
  Version:      1.0
  Date:         2014.08.11
  Description:  RTC����,оƬ��ӦRX8025
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2014.08.11     V1.0      create
*****************************************************************************/

#include "Rtc_RX8025.h"

/*********************************************************
    Function:       RtcGetWeekFromDate
    Description:    ��ʱ���ȡ����
    Param:          year
                    month
                    date          
    Return:         ������ 1-7,����һΪ1��������Ϊ7
    Other:          none
*********************************************************/ 
static Uint8 RtcGetWeekFromDate(Uint8 year,Uint8 month,Uint8 date)
{
    Uint8 day;
    if(month < 3) 
    {
        month += 12;
        if(year > 0)
        {
            year--;
        }
        else
        {
            year = 4;
        }
    }
  
      day = date + (month<<1) + (Uint8)(((month+1u)*154u)>>8) + year + (year>>2); 
  
      if(day >= 112) day -= 112; //day%7
      if(day >= 56)  day -= 56; 
      if(day >= 28)  day -= 28; 
      if(day >= 14)  day -= 14; 
      if(day >= 7)   day -= 7;
  
      return day + 1;  // �������ڼ�(����һ��1��ʾ������������7��ʾ)
}
#if 0
/*��δʹ��,���ξ���ע��*/
/*********************************************************
    Function:       Rtc8025RdReg
    Description:    RTC_RX8025���Ĵ���
    Param:          iicFd
                    reg
                    value          
    Return:         ����� �ɹ�����1
    Other:          none
*********************************************************/ 
static int Rtc8025RdReg(int iicFd, Uint8 reg, Uint8 *value)
{
    Uint8  outBuf;  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[2];  
  

    outBuf = reg << 4;  
    messages[0].addr  = RTC_IIC_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outBuf);  
    messages[0].buf   = &outBuf;  
  
    messages[1].addr  = RTC_IIC_ADDR;  
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;  
    messages[1].len   = 1;  
    messages[1].buf   = value;  
   
    packets.msgs      = messages;  
    packets.nmsgs     = 2;  
    if(ioctl(iicFd, I2C_RDWR, &packets) < 0) 
    {  
        perror("RX8025 Unable to send data");  
        return 0;  
    }   
  
    return 1;
}
#endif

/*********************************************************
    Function:       Rtc8025RdNBytes
    Description:    RTC_RX8025��N�ֽ�
    Param:          iicFd
                    reg
                    vaule
                    nBytes          
    Return:         ����� �ɹ�����1
    Other:          none
*********************************************************/ 
static int Rtc8025RdNBytes(int iicFd, Uint8 reg, Uint8 *value, Uint8 nBytes)
{
    Uint8 outBuf;  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[2];  
  

    outBuf = reg << 4;  
    messages[0].addr  = RTC_IIC_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outBuf);  
    messages[0].buf   = &outBuf;  
  
    messages[1].addr  = RTC_IIC_ADDR;  
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;  
    messages[1].len   = nBytes;  
    messages[1].buf   = value;  
   
    packets.msgs      = messages;  
    packets.nmsgs     = 2;  
    if(ioctl(iicFd, I2C_RDWR, &packets) < 0) 
    {  
        perror("RX8025 Unable to send data");  
        return 0;  
    }

    return 1;
}

/*********************************************************
    Function:       Rtc8025WrReg
    Description:    RTC_RX8025д�Ĵ���
    Param:          iicFd
                    reg
                    vaule          
    Return:         д��� �ɹ�����1
    Other:          none
*********************************************************/ 
static int Rtc8025WrReg(int iicFd, Uint8 reg, Uint8 value)
{
    Uint8 outBuf[2];  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[1];  
  
    messages[0].addr  = RTC_IIC_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outBuf);  
    messages[0].buf   = outBuf;  
  
    outBuf[0] = reg << 4;
    outBuf[1] = value;  
  
    packets.msgs  = messages;  
    packets.nmsgs = 1;  
    if(ioctl(iicFd, I2C_RDWR, &packets) < 0) 
    {  
        perror("Unable to send data");  
        return 0;  
    }  
  
    return 1;
}

/*********************************************************
    Function:       Rtc8025Init
    Description:    RTC_RX8025��ʼ��
    Param:          iicFd         
    Return:         ���
    Other:          none
*********************************************************/ 
int Rtc8025Init(int iicFd)
{
    if(Rtc8025WrReg(iicFd,RTC_REG_CTRL1,0x20) == 0)
    {
        return 0;
    }
    if(Rtc8025WrReg(iicFd,RTC_REG_CTRL2,0x20) == 0)
    {
        return 0;
    }
    
    return 1;
}

/*********************************************************
    Function:       Rtc8025GetTime
    Description:    RTC_RX8025��ȡʱ��
    Param:          iicFd
                    time      
    Return:         ���
    Other:          none
*********************************************************/ 
int Rtc8025GetTime(int iicFd,S_RtcTime *timeGet)
{
    Uint8 timeRd[7];
    
    if(Rtc8025RdNBytes(iicFd,RTC_REG_SECOND,timeRd,7) == 0)
    {
        return 0;
    }
    
    timeGet->year = (((timeRd[6] >> 4) & 0x0f) * 10) + (timeRd[6] & 0x0f);
    timeGet->month = (((timeRd[5] >> 4) & 0x01) * 10) + (timeRd[5] & 0x0f);
    timeGet->date = (((timeRd[4] >> 4) & 0x03) * 10) + (timeRd[4] & 0x0f);
    timeGet->week = timeRd[3] & 0x07;
    timeGet->hour = (((timeRd[2] >> 4) & 0x03) * 10) + (timeRd[2] & 0x0f);
    timeGet->minute = (((timeRd[1] >> 4) & 0x07) * 10) + (timeRd[1] & 0x0f);
    timeGet->second = (((timeRd[0] >> 4) & 0x07) * 10) + (timeRd[0] & 0x0f);
    if(timeGet->week == 0)
    {
        timeGet->week = 7;
    }
    
    return 1;
}

/*********************************************************
    Function:       Rtc8025SetTime
    Description:    RTC_RX8025����ʱ��
    Param:          iicFd
                    time      
    Return:         none
    Other:          none
*********************************************************/ 
void Rtc8025SetTime(int iicFd,S_RtcTime *timeSet)
{
    Uint8 setTime;
    
    /*��*/
    setTime = ((((timeSet->year & 0xff) / 10) << 4) & 0xf0) | ((((timeSet->year & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_YEAR,setTime);
    /*��*/
    setTime = ((((timeSet->month & 0xff) / 10) << 4) & 0x10) | ((((timeSet->month & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_MONTH,setTime);
    /*��*/
    setTime = ((((timeSet->date & 0xff) / 10) << 4) & 0x30) | ((((timeSet->date & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_DAY,setTime);
    /*ʱ*/
    setTime = ((((timeSet->hour & 0xff) / 10) << 4) & 0x30) | ((((timeSet->hour & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_HOUR,setTime);
    /*��*/
    setTime = ((((timeSet->minute & 0xff) / 10) << 4) & 0x70) | ((((timeSet->minute & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_MINUTE,setTime);
    /*��*/
    setTime = ((((timeSet->second & 0xff) / 10) << 4) & 0x70) | ((((timeSet->second & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_SECOND,setTime);
    
    /*����*/
    timeSet->week = RtcGetWeekFromDate(timeSet->year,timeSet->month,timeSet->date);
    
    if(timeSet->week == 7)
    {
        timeSet->week = 0;
    }
    setTime = timeSet->week & 0x07;
    Rtc8025WrReg(iicFd,RTC_REG_WEEK,setTime);

}