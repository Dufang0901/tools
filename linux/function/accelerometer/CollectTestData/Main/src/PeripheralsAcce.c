/*****************************************************************************
  FileName:     PeripheralsAcce.h
  Author:       ����
  Version:      1.0
  Date:         2017/2/28 14:35:49
  Description:  ������̵ļ��ٶȴ���������
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2017.02.28    V1.0       create
*****************************************************************************/

#include "PeripheralsAcce.h"

#define ADXL_DEV_ADDR		    0x53		/*ADXL��IIC�豸��ַ*/



pthread_t threadAcce;
    
/*********************************************************
    Function:       PhAcceAdxl345DevOpen
    Description:    ������Adxl345������i2c������
    Param:          none
    Return:         ������
    Other:          ִ�ж�I2C�ĳ�ʼ��
*********************************************************/
static int PhAcceAdxl345DevOpen(void)
{
    #define PH_ADX1345_IIC_DEV                     "/dev/i2c-2"
    
    Int32 devFd;
    if((devFd = open(PH_ADX1345_IIC_DEV, O_RDWR)) <0) 
    {  
        return -1;
    }
    
    if(ioctl(devFd, I2C_TIMEOUT, 2) < 0) //���ó�ʱʱ��
    {
        perror("Est Unable to set i2c timeout");
        close(devFd);
        return -1;
    }
    
    if(ioctl(devFd, I2C_RETRIES, 1) < 0)//�����ط����� 
    {
        perror("Est Unable to set i2c retries");
        close(devFd);
        return -1;
    }

    return devFd;
}

/*********************************************************
    Function:       PhAcceAdxl345WrReg
    Description:    Adxl345д�Ĵ���
    Param:          inRegAddr       �Ĵ�����ַ
                    inData          �Ĵ�������
    Return:         ���
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
    Description:    Adxl345���Ĵ���
    Param:          inRegAddr            �Ĵ�����ַ
    Return:         �Ĵ�������
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
    Description:    Adxl345��ʼ������
    Param:          inDevFd
    Return:         none
    Other:          none
*********************************************************/
static Int32 PhAdx1345Init(int inDevFd)
{
    Int32 ret;
    ret = PhAcceAdxl345WrReg(inDevFd,0x31,0x0b);   //������Χ,����16g��13λģʽ
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x2C,0x08);   //�����趨Ϊ12.5Hz �ο�pdf13ҳ
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x2E,0x80);   //ʹ�� DATA_READY �ж�
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    usleep(5000);
    ret = PhAcceAdxl345WrReg(inDevFd,0x20,0x06);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    usleep(5000);

    //�ֱ��� = (16g*2)/(2^13) = 3.9mg/LSB = 0.038 m/s*s
    
    return ret;
}

/*********************************************************
    Function:       PerpheralAcceThread
    Description:    �����¶Ȼ�ȡ���߳�
    Param:          arg ����
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
        /*�豸���쳣��ֱ���˳�*/
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

            //ת��Ϊ m/(s*s)
            psPhAcce->acceX = acceX;
            psPhAcce->acceY = acceY;
            psPhAcce->acceZ = acceZ;
 
        }
        /*���N�����ȡһ��*/
        usleep(PH_ACCE_GET_INTERVAL_MS * 1000);
        
    }
    return NULL;
}

/*********************************************************
    Function:       PhAccePthreadStart
    Description:    �����¶��߳�����
    Param:          psInPhProcessRes
    Return:         none
    Other:          none
*********************************************************/
void PhAccePthreadStart(S_PhAcce *psPhacce)
{
    /*Io״̬��ȡ�߳�*/
    if(PthreadCreate(&threadAcce,NULL,PerpheralAcceThread,(void*)psPhacce) == -1)
    {
        printf("Create peripherals acceleration pthread failed.\n");
    }
    sleep(1);
}
