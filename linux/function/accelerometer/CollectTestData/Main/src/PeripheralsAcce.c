/*****************************************************************************
  FileName:     PeripheralsAcce.h
  Author:       林锴
  Version:      1.0
  Date:         2017/2/28 14:35:49
  Description:  外设进程的加速度传感器操作
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2017.02.28    V1.0       create
*****************************************************************************/

#include "PeripheralsAcce.h"

#define ADXL_DEV_ADDR		    0x53		/*ADXL的IIC设备地址*/



pthread_t threadAcce;
    
/*********************************************************
    Function:       PhAcceAdxl345DevOpen
    Description:    打开用于Adxl345交互的i2c描述符
    Param:          none
    Return:         描述符
    Other:          执行对I2C的初始化
*********************************************************/
static int PhAcceAdxl345DevOpen(void)
{
    #define PH_ADX1345_IIC_DEV                     "/dev/i2c-2"
    
    Int32 devFd;
    if((devFd = open(PH_ADX1345_IIC_DEV, O_RDWR)) <0) 
    {  
        return -1;
    }
    
    if(ioctl(devFd, I2C_TIMEOUT, 2) < 0) //设置超时时间
    {
        perror("Est Unable to set i2c timeout");
        close(devFd);
        return -1;
    }
    
    if(ioctl(devFd, I2C_RETRIES, 1) < 0)//设置重发次数 
    {
        perror("Est Unable to set i2c retries");
        close(devFd);
        return -1;
    }

    return devFd;
}

/*********************************************************
    Function:       PhAcceAdxl345WrReg
    Description:    Adxl345写寄存器
    Param:          inRegAddr       寄存器地址
                    inData          寄存器数据
    Return:         结果
    Other:          none
*********************************************************/
static Int32 PhAcceAdxl345WrReg(Int32 inDevFd, Uint8 inRegAddr,Uint8 inData)
{
    Uint8 outBuf[2];  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[1];  
  
    messages[0].addr  = ADXL_DEV_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outBuf);  
    messages[0].buf   = outBuf;  
  
    //outBuf[0] = inRegAddr << 4;
    outBuf[0] = inRegAddr;
    outBuf[1] = inData;  
  
    packets.msgs  = messages;  
    packets.nmsgs = 1;

    
    if(ioctl(inDevFd, I2C_RDWR, &packets) < 0) 
    {  
        perror("I2C: Unable to send data"); 
        return 0; 
    }  

    return 1;
}

/*********************************************************
    Function:       PhAcceAdxl345RdReg
    Description:    Adxl345读寄存器
    Param:          inRegAddr            寄存器地址
    Return:         寄存器数据
    Other:          none
*********************************************************/
static Int32 PhAcceAdxl345RdReg(Int32 inDevFd, Uint8 inRegAddr, Uint8 *pInValue, Uint8 inNBytes)
{   
    Uint8 outBuf;  
    struct i2c_rdwr_ioctl_data packets;  
    struct i2c_msg messages[2];  

    //outBuf = inRegAddr << 4; 
    outBuf = inRegAddr;
    messages[0].addr  = ADXL_DEV_ADDR;  
    messages[0].flags = 0;  
    messages[0].len   = sizeof(outBuf);  
    messages[0].buf   = &outBuf;  
  
    messages[1].addr  = ADXL_DEV_ADDR;  
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;  
    messages[1].len   = inNBytes;  
    messages[1].buf   = pInValue;  
   
    packets.msgs      = messages;  
    packets.nmsgs     = 2;  
    if(ioctl(inDevFd, I2C_RDWR, &packets) < 0) 
    {  
        perror("I2C: Unable to send data");  
        return 0;  
    }

    return 1;
}

/*********************************************************
    Function:       PhAdx1345Init
    Description:    Adxl345初始化操作
    Param:          inDevFd
    Return:         none
    Other:          none
*********************************************************/
static Int32 PhAdx1345Init(int inDevFd)
{
    Int32 ret;
    ret = PhAcceAdxl345WrReg(inDevFd,0x31,0x0b);   //测量范围,正负16g，13位模式
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x2C,0x08);   //速率设定为12.5Hz 参考pdf13页
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x2D,0x08);   //选择电源模式   参考pdf24页
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x2E,0x80);   //使能 DATA_READY 中断
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x20,0x06);   //Z 偏移量 根据测试传感器的状态写入pdf29页
    usleep(5000);

    //分辨率 = (16g*2)/(2^13) = 3.9mg/LSB = 0.038 m/s*s
    
    return ret;
}

/*********************************************************
    Function:       PerpheralAcceThread
    Description:    外设温度获取的线程
    Param:          arg 参数
    Return:         none
    Other:          none
*********************************************************/
static void *PerpheralAcceThread(void *arg)
{
    S_PhAcce       *psPhAcce = (S_PhAcce *)arg;

    int     adxDevFd;
    
    Uint8   acceValue[6];
    Int16   acceIntTemp;
    float   acceX,acceY,acceZ;
    
    Int32   i;
   
    
    adxDevFd = PhAcceAdxl345DevOpen();
    
    if(adxDevFd < 0)
    {
        /*设备都异常了直接退出*/
        printf("Acceerature fd open error and over thread.\n");
        return NULL;
    }
    
    if(PhAdx1345Init(adxDevFd) == 0)
    {
        printf("Acceerature adx1345 init failed and over thread.\n");
        return NULL;
    }
    
    while(1)
    {
        if(PhAcceAdxl345RdReg(adxDevFd, 0x32, acceValue, 6) == 1)
        {
            acceIntTemp = (acceValue[1] << 8) | acceValue[0];
            acceX = ((float)acceIntTemp) * 0.039;
        
            acceIntTemp = (acceValue[3] << 8) | acceValue[2];
            acceY = ((float)acceIntTemp) * 0.039;
        
            acceIntTemp = (acceValue[5] << 8) | acceValue[4];
            acceZ = ((float)acceIntTemp) * 0.039;

            //转化为 m/(s*s)
            psPhAcce->acceX = acceX;
            psPhAcce->acceY = acceY;
            psPhAcce->acceZ = acceZ;
 
        }
        /*相隔N毫秒获取一次*/
        usleep(PH_ACCE_GET_INTERVAL_MS * 1000);
        
    }
    return NULL;
}

/*********************************************************
    Function:       PhAccePthreadStart
    Description:    外设温度线程启动
    Param:          psInPhProcessRes
    Return:         none
    Other:          none
*********************************************************/
void PhAccePthreadStart(S_PhAcce *psPhacce)
{
    /*Io状态获取线程*/
    if(PthreadCreate(&threadAcce,NULL,PerpheralAcceThread,(void*)psPhacce) == -1)
    {
        printf("Create peripherals acceleration pthread failed.\n");
    }
    sleep(1);
}
