/*****************************************************************************
  FileName:     Rtc_RX8025.c
  Author:       林锴
  Version:      1.0
  Date:         2014.08.11
  Description:  RTC操作,芯片对应RX8025
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2014.08.11     V1.0      create
*****************************************************************************/

#include "Rtc_RX8025.h"

/*********************************************************
    Function:       RtcGetWeekFromDate
    Description:    从时间获取星期
    Param:          year
                    month
                    date          
    Return:         星期数 1-7,星期一为1，星期日为7
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
  
      return day + 1;  // 返回星期几(星期一用1表示，而星期天用7表示)
}
#if 0
/*暂未使用,屏蔽警告注销*/
/*********************************************************
    Function:       Rtc8025RdReg
    Description:    RTC_RX8025读寄存器
    Param:          iicFd
                    reg
                    value          
    Return:         读结果 成功返回1
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
    Description:    RTC_RX8025读N字节
    Param:          iicFd
                    reg
                    vaule
                    nBytes          
    Return:         读结果 成功返回1
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
    Description:    RTC_RX8025写寄存器
    Param:          iicFd
                    reg
                    vaule          
    Return:         写结果 成功返回1
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
    Description:    RTC_RX8025初始化
    Param:          iicFd         
    Return:         结果
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
    Description:    RTC_RX8025获取时间
    Param:          iicFd
                    time      
    Return:         结果
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
    Description:    RTC_RX8025设置时间
    Param:          iicFd
                    time      
    Return:         none
    Other:          none
*********************************************************/ 
void Rtc8025SetTime(int iicFd,S_RtcTime *timeSet)
{
    Uint8 setTime;
    
    /*年*/
    setTime = ((((timeSet->year & 0xff) / 10) << 4) & 0xf0) | ((((timeSet->year & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_YEAR,setTime);
    /*月*/
    setTime = ((((timeSet->month & 0xff) / 10) << 4) & 0x10) | ((((timeSet->month & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_MONTH,setTime);
    /*日*/
    setTime = ((((timeSet->date & 0xff) / 10) << 4) & 0x30) | ((((timeSet->date & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_DAY,setTime);
    /*时*/
    setTime = ((((timeSet->hour & 0xff) / 10) << 4) & 0x30) | ((((timeSet->hour & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_HOUR,setTime);
    /*分*/
    setTime = ((((timeSet->minute & 0xff) / 10) << 4) & 0x70) | ((((timeSet->minute & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_MINUTE,setTime);
    /*秒*/
    setTime = ((((timeSet->second & 0xff) / 10) << 4) & 0x70) | ((((timeSet->second & 0xff) % 10)) & 0x0f);
    Rtc8025WrReg(iicFd,RTC_REG_SECOND,setTime);
    
    /*星期*/
    timeSet->week = RtcGetWeekFromDate(timeSet->year,timeSet->month,timeSet->date);
    
    if(timeSet->week == 7)
    {
        timeSet->week = 0;
    }
    setTime = timeSet->week & 0x07;
    Rtc8025WrReg(iicFd,RTC_REG_WEEK,setTime);

}