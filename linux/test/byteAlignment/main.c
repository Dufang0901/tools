#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <string.h>

typedef struct{
	char a;
	int  b;
	char c;
}S_Test;

int comCfg(const char * comDev)
{
	int comFd;
    struct termios comTty;
    
    comFd = open(comDev, O_RDWR|O_NOCTTY, 0);

    if(comFd < 0)
    {
        return comFd;
    }
    
    if (fcntl(comFd, F_SETFL, 0) < 0)
    {
        return -1;
    }
    
    bzero( &comTty, sizeof( comTty ) );
    cfsetispeed(&comTty,B19200);  /*输入*/
    cfsetospeed(&comTty,B19200);  /*输出*/  
    
    comTty.c_cflag  |=  CLOCAL | CREAD;
    comTty.c_cflag &= ~CSIZE;
    comTty.c_cflag |= CS8;
    
    comTty.c_cflag &= ~PARENB;
    
    
    comTty.c_cflag &=  ~CSTOPB;
    /*超时等待时间200毫秒*/
    comTty.c_cc[VTIME]  = 2;
    comTty.c_cc[VMIN] = 0;
    tcflush(comFd,TCIFLUSH);
    if((tcsetattr(comFd,TCSANOW,&comTty))!=0)
    {
        return -1;
    }
    return comFd;
}
int main()
{
	int comFd = -1;
	S_Test sTest;
	ssize_t ret;
	char   buf[1024];
	
	comFd = comCfg("/dev/ttymxc1");
	if(comFd <= 0)
	{
		printf("open serial port failed.\n");
		return -1;
	}
	printf("comFd is %d\n",comFd);
	
	sTest.a = '1';
	sTest.b = 1;
	sTest.c = '2';
	
	ret = write(comFd,&sTest,sizeof(sTest));
	if(ret != sizeof(sTest))
		printf("send data error.\n");
	else
		printf("%d bytes are sent.\n",ret);
	
	while(1)
	{
		bzero(buf,sizeof(buf));
		ret = read(comFd,buf,1023);
		if(ret <= 0)
			continue;
		else
			printf("ret is %d,receiv msg:%s\n",ret,buf);
	}
	close(comFd);
	
	return 1;
	
	
	
	
	
	
	
}