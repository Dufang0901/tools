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
	/*打开加速度度传感器设备文件*/
	int fd;
	
	if((fd = open("/dev/i2c-2",O_RDWR)) < 0)
		return 0;
	//printf("fd=%d\n",fd);
	
	/*初始化adxl345*/
	adxl345WrReg(fd,0x31,0x0B);	//设置测量范围为±16g，分辨率为13位
	usleep(5000);
	adxl345WrReg(fd,0x2D,0x08);	//开始测量
	usleep(5000);
	adxl345WrReg(fd,0x2E,0x80);	//使能 DATA_READY 中断
	usleep(5000);
#if 0
	adxl345WrReg(fd,0x1E,0x00);	//X轴初始偏移量
	usleep(5000);
	adxl345WrReg(fd,0x1F,0x00);	//Y轴初始偏移量
	usleep(5000);
	adxl345WrReg(fd,0x20,0x06);	//Y轴初始偏移量
	usleep(5000);
#endif
	
	/*读取x y z轴数据*/
	char              buf[6];		/*存储读回来的x y z三个方向的加速度值*/
	short x,y,z;
	float accX,accY,accZ;
	FILE *fp;
	
	memset(buf,0,sizeof(buf));
	//获取时间
	time_t timet;
	struct tm timeTmp;
	
	time(&timet);
	localtime_r(&timet,&timeTmp);
	year = timeTmp.tm_year + 1900;
	month = timeTmp.tm_mon + 1;
	day   = timeTmp.tm_mday;
	
	//打开日志文件
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
	
		/*对数据进行处理并显示*/
		
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
		
		//滤波
		static float gx = 0.0;
		static float gy = 0.008;
		static float gz = 9.2;
		static float alpha = 0.8;
		float        ax,ay,az;
		
		
		//低通滤波分离出重力加速度
		gx = alpha * gx + (1 - alpha) * accX;
		gy = alpha * gy + (1 - alpha) * accY;
		gz = alpha * gz + (1 - alpha) * accZ;
		//高通滤波剔除重力干扰
		ax = accX - gx;
		ay = accY - gy;
		az = accZ - gz;
		//获取时间
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
		
		//隔天重新保存文件
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