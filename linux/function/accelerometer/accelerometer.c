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
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define SLAVE_ADDR		0x53
int year,month,day;

int adxl345WrReg(int fd,unsigned char reg,unsigned char value)
{
	struct i2c_rdwr_ioctl_data regData;
	struct i2c_msg             data;
	unsigned char              buf[2];
	buf[0] = reg;
	buf[1] = value;
	
	regData.msgs = &data;
	regData.nmsgs = 1;
	
	data.addr = SLAVE_ADDR;
	data.flags = 0;
	data.len = 2;
	data.buf = buf;
	
	if(ioctl(fd,I2C_RDWR,&regData) < 0)
	{
		perror("send i2c data error.\n");
		return -1;
	}
	
	return 1;
}

int adxl345RdReg(int fd,unsigned char reg,unsigned char *buf,int rdByte)
{
	struct i2c_rdwr_ioctl_data regData;
	struct i2c_msg             data[2];
	
	data[0].addr = SLAVE_ADDR;
	data[0].flags = 0;
	data[0].len = 1;
	data[0].buf = &reg;
	
	data[1].addr = SLAVE_ADDR;
	data[1].flags = I2C_M_RD;
	data[1].len = rdByte;
	data[1].buf = buf;
	
	regData.nmsgs = 2;
	regData.msgs  = data;

	if(ioctl(fd,I2C_RDWR,&regData) < 0)
	{
		perror("send i2c data error.\n");
		return -1;
	}
	return 1;
}

int main()
{
	/*�򿪼��ٶȶȴ������豸�ļ�*/
	int fd;
	
	if((fd = open("/dev/i2c-2",O_RDWR)) < 0)
		return 0;
	//printf("fd=%d\n",fd);
	
	/*��ʼ��adxl345*/
	adxl345WrReg(fd,0x31,0x0B);	//���ò�����ΧΪ��16g���ֱ���Ϊ13λ
	usleep(5000);
	adxl345WrReg(fd,0x2D,0x08);	//��ʼ����
	usleep(5000);
	adxl345WrReg(fd,0x2E,0x80);	//ʹ�� DATA_READY �ж�
	usleep(5000);
#if 0
	adxl345WrReg(fd,0x1E,0x00);	//X���ʼƫ����
	usleep(5000);
	adxl345WrReg(fd,0x1F,0x00);	//Y���ʼƫ����
	usleep(5000);
	adxl345WrReg(fd,0x20,0x06);	//Y���ʼƫ����
	usleep(5000);
#endif
	
	/*��ȡx y z������*/
	char              buf[6];		/*�洢��������x y z��������ļ��ٶ�ֵ*/
	short x,y,z;
	float accX,accY,accZ;
	FILE *fp;
	
	memset(buf,0,sizeof(buf));
	//��ȡʱ��
	time_t timet;
	struct tm timeTmp;
	
	time(&timet);
	localtime_r(&timet,&timeTmp);
	year = timeTmp.tm_year + 1900;
	month = timeTmp.tm_mon + 1;
	day   = timeTmp.tm_mday;
	
	//����־�ļ�
	char fileName[128];
	bzero(fileName,128);
	sprintf(fileName,"/root/%04d-%02d-%02daccLog.csv",year,month,day);
	fp = fopen(fileName,"w");
	if(fp == NULL)
		return -1;
	
	fprintf(fp,"time,accx,accy,accz,gx,gy,gz,g,ax,ay,az,a\n");
	fclose(fp);
	
	fp = fopen(fileName,"a");
	if(fp == NULL)
		return -1;
	
	int n = 0;
	
	while(1)
	{
		adxl345RdReg(fd,0x32,buf,6);
	
		/*�����ݽ��д�����ʾ*/
		
		x = y = z = 0;
		int i;
#if 0
		for(i = 0;i < 6;i++)
			printf("%02hhx ",buf[i]);
		printf("    ");
#endif
		
		x = buf[0] | buf[1] << 8;
		y = buf[2] | buf[3] << 8;
		z = buf[4] | buf[5] << 8;
		
		accX = x * 0.039;
		accY = y * 0.039;
		accZ = z * 0.039;
		
		//�˲�
		static float gx = 0.0;
		static float gy = 0.008;
		static float gz = 9.2;
		static float alpha = 0.8;
		float        ax,ay,az;
		
		
		//��ͨ�˲�������������ٶ�
		gx = alpha * gx + (1 - alpha) * accX;
		gy = alpha * gy + (1 - alpha) * accY;
		gz = alpha * gz + (1 - alpha) * accZ;
		//��ͨ�˲��޳���������
		ax = accX - gx;
		ay = accY - gy;
		az = accZ - gz;
		//��ȡʱ��
		struct timeval tv;
		struct tm sTime;
		gettimeofday(&tv,NULL);
		localtime_r(&tv.tv_sec,&sTime);
		fprintf(fp,"%02d:%02d:%02d.%03d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n"
		,sTime.tm_hour,sTime.tm_min,sTime.tm_sec,(int)tv.tv_usec/1000
		,accX,accY,accZ,gx,gy,gz,sqrt(gx*gx + gy*gy + gz*gz),ax,ay,az,sqrt(ax*ax + ay*ay + az*az));
		
		//printf("x=%.2f y=%.2f z=%.2f    %.2f %.2f %.2f\n",x*0.004,y*0.004,z*0.004,accX,accY,accZ);
		
		if(++n % 300 == 0)
		{
			sync();
		}
		
		//�������±����ļ�
		if(sTime.tm_year + 1900 != year || sTime.tm_mon + 1 != month || sTime.tm_mday != day)
		{
			year = sTime.tm_year;
			month = sTime.tm_mon + 1;
			day = sTime.tm_mday;
			
			fclose(fp);
			
			bzero(fileName,128);
			sprintf(fileName,"/root/%04d-%02d-%02daccLog.csv",year,month,day);
			fp = fopen(fileName,"w");
			if(fp == NULL)
				return -1;
			
			fprintf(fp,"time,accx,accy,accz,gx,gy,gz,g,ax,ay,az,a\n");
			fclose(fp);
			
			fp = fopen("./accLog.csv","a");
			if(fp == NULL)
				return -1;
			
			
		}
		
		usleep(250 * 1000);
		
	}
	fclose(fp);
	return 1;
	
	
}