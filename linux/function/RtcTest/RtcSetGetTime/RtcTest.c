#include <stdio.h>  
#include <linux/types.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/ioctl.h>  
#include <errno.h>  
#include <assert.h>  
#include <string.h>  
#include <linux/i2c.h>  
#include <linux/i2c-dev.h> 
#include <stdint.h>


#define RTC_IIC_DEV     "/dev/i2c-2"

#define RTC_IIC_ADDR	0x32

#define RTC_REG_SECOND		0x00
#define	RTC_REG_MINUTE		0x01
#define	RTC_REG_HOUR		0x02
#define RTC_REG_WEEK		0x03
#define RTC_REG_DAY         0x04
#define RTC_REG_MONTH		0x05
#define RTC_REG_YEAR		0x06
#define RTC_REG_DIG_OFFSET  0x07
#define RTC_REG_AW_MINUTE	0x08
#define RTC_REG_AW_HOUR		0x09
#define RTC_REG_AW_WEEK		0x0A
#define RTC_REG_AD_MINUTE	0x0B
#define RTC_REG_AD_HOUR		0x0C
#define RTC_REG_CTRL1		0x0E
#define RTC_REG_CTRL2		0x0F 

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}S_RtcTime;


static int RtcWrReg(int i2cFile,uint8_t reg, uint8_t value)
{
    uint8_t outbuf[2];  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[1];  
  
    messages[0].addr  = RTC_IIC_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outbuf);  
    messages[0].buf   = outbuf;  
  
    /* The first byte indicates which register we'll write */  
    outbuf[0] = reg;  
  
    /*  
     * The second byte indicates the value to write.  Note that for many 
     * devices, we can write multiple, sequential registers at once by 
     * simply making outbuf bigger. 
     */  
    outbuf[1] = value;  
  
    /* Transfer the i2c packets to the kernel and verify it worked */  
    packets.msgs  = messages;  
    packets.nmsgs = 1;  
    if(ioctl(i2cFile, I2C_RDWR, &packets) < 0) 
    {  
        perror("Unable to send data");  
        return 1;  
    }  
  
    return 0; 
}

static int RtcRdReg(int i2cFile,uint8_t reg, uint8_t *value)
{
    unsigned char inbuf, outbuf;  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[2];  
  
    /* 
     * In order to read a register, we first do a "dummy write" by writing 
     * 0 bytes to the register we want to read from.  This is similar to 
     * the packet in set_i2c_register, except it's 1 byte rather than 2. 
     */  
    outbuf = reg;  
    messages[0].addr  = RTC_IIC_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outbuf);  
    messages[0].buf   = &outbuf;  
  
    /* The data will get returned in this structure */  
    messages[1].addr  = RTC_IIC_ADDR;  
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;  
    messages[1].len   = sizeof(inbuf);  
    messages[1].buf   = &inbuf;  
  
    /* Send the request to the kernel and get the result back */  
    packets.msgs      = messages;  
    packets.nmsgs     = 2;  
    if(ioctl(i2cFile, I2C_RDWR, &packets) < 0) 
    {  
        perror("Unable to send data");  
        return 1;  
    }  
    *value = inbuf;  
  
    return 0;
}


int main(int argc,char *argv[])
{
    int i2cFile;
    
    S_RtcTime sRtcTimeSet;
    S_RtcTime sRtcTimeGet;
    S_RtcTime *time;
    uint8_t setTime;
    uint8_t getTime;
	char    timeStr[128] = {0};
	char    timeStr2[128] = {0};
	int year,month,day,hour,minute,second;
	
    // Open a connection to the I2C userspace control file.  
    if ((i2cFile = open(RTC_IIC_DEV, O_RDWR)) < 0) 
    {  
        perror("Unable to open i2c control file");  
        exit(1);  
    } 
    
    if(ioctl(i2cFile, I2C_TIMEOUT, 2) < 0) //设置超时时间
    {
        perror("Unable to set i2c timeout");
    }  
    if(ioctl(i2cFile, I2C_RETRIES, 1) < 0)//设置重发次数 
    {
        perror("Unable to set i2c retries");
    }
   
    RtcWrReg(i2cFile,RTC_REG_CTRL1 << 4,0x20);
    RtcWrReg(i2cFile,RTC_REG_CTRL2 << 4,0x20);
	
	/*检查参数个数*/
	if(argc != 3)
	{
		printf("input argument error,return.\n");
		return -1;
	}
	
	/*检查输入的时间格式是否正确*/
	strncpy(timeStr, argv[1],sizeof(timeStr) - 1);
	strncpy(timeStr2, argv[2],sizeof(timeStr2) - 1);
	if(timeStr[4] != '-')
	{
		printf("input time format error,return.\n");
		return -1;
	}
	else if(timeStr[7] != '-')
	{
		printf("input time format error,return.\n");
		return -1;
	}
	else if(timeStr[10] != '\0')
	{
		printf("input time format error,return.\n");
		return -1;
	}
	else if(timeStr2[2] != ':')
	{
		printf("input time format error,return.\n");
		return -1;
	}
	else if(timeStr2[5] != ':')
	{
		printf("input time format error,return.\n");
		return -1;
	}
	else if(timeStr2[8] != '\0')
	{
		printf("input time format error,return.\n");
		return -1;
	}

	/*格式正确*/
	sscanf(timeStr,"%04d-%02d-%02d",&year,&month,&day);
	sscanf(timeStr2,"%02d:%02d:%02d",&hour,&minute,&second);
	printf("input time:%04d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,minute,second);
	if(year <= 2000)
	{
		printf("input year error,return.\n");
		return -1;
	}

	
#if 1    
    /*写时间*/
    sRtcTimeSet.year = year - 2000;
    sRtcTimeSet.month = month;
    sRtcTimeSet.day = day;
    sRtcTimeSet.hour = hour;
    sRtcTimeSet.minute = minute;
    sRtcTimeSet.second = second;
    
    time = &sRtcTimeSet;
    
    
    setTime = ((((time->year & 0xff) / 10) << 4) & 0xf0) | ((((time->year & 0xff) % 10)) & 0x0f);
    RtcWrReg(i2cFile,RTC_REG_YEAR << 4,setTime);
    setTime = ((((time->month & 0xff) / 10) << 4) & 0x10) | ((((time->month & 0xff) % 10)) & 0x0f);
    RtcWrReg(i2cFile,RTC_REG_MONTH << 4,setTime);
    setTime = ((((time->day & 0xff) / 10) << 4) & 0x30) | ((((time->day & 0xff) % 10)) & 0x0f);
    RtcWrReg(i2cFile,RTC_REG_DAY << 4,setTime);
    setTime = ((((time->hour & 0xff) / 10) << 4) & 0x30) | ((((time->hour & 0xff) % 10)) & 0x0f);
    RtcWrReg(i2cFile,RTC_REG_HOUR << 4,setTime);
    setTime = ((((time->minute & 0xff) / 10) << 4) & 0x70) | ((((time->minute & 0xff) % 10)) & 0x0f);
    RtcWrReg(i2cFile,RTC_REG_MINUTE << 4,setTime);
    setTime = ((((time->second & 0xff) / 10) << 4) & 0x70) | ((((time->second & 0xff) % 10)) & 0x0f);
    RtcWrReg(i2cFile,RTC_REG_SECOND << 4,setTime);
    
    
    
    sleep(2);
    
    /*读回时间*/
    RtcRdReg(i2cFile,RTC_REG_YEAR << 4, &getTime);
    sRtcTimeGet.year = (((getTime >> 4) & 0x0f) * 10) + (getTime & 0x0f);
    RtcRdReg(i2cFile,RTC_REG_MONTH << 4, &getTime);
    sRtcTimeGet.month = (((getTime >> 4) & 0x01) * 10) + (getTime & 0x0f);
    RtcRdReg(i2cFile,RTC_REG_DAY << 4, &getTime);
    sRtcTimeGet.day = (((getTime >> 4) & 0x03) * 10) + (getTime & 0x0f);
    RtcRdReg(i2cFile,RTC_REG_HOUR << 4, &getTime);
    sRtcTimeGet.hour = (((getTime >> 4) & 0x03) * 10) + (getTime & 0x0f);
    RtcRdReg(i2cFile,RTC_REG_MINUTE << 4, &getTime);
    sRtcTimeGet.minute = (((getTime >> 4) & 0x07) * 10) + (getTime & 0x0f);
    RtcRdReg(i2cFile,RTC_REG_SECOND << 4, &getTime);
    sRtcTimeGet.second = (((getTime >> 4) & 0x07) * 10) + (getTime & 0x0f);
    
    
    printf("Now time is %d-%02d-%02d %02d:%02d:%02d.\n",sRtcTimeGet.year + 2000,sRtcTimeGet.month,sRtcTimeGet.day,sRtcTimeGet.hour,sRtcTimeGet.minute,sRtcTimeGet.second);
    
 #endif   
 
    close(i2cFile);
    return 0;
}