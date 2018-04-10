#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

int GpsComOpenCfg(const char *dev,speed_t baud)
{
    int comFd;
    struct termios comTty;
    
    //comFd = open(dev, O_RDWR|O_NOCTTY|O_NDELAY, 0);
    comFd = open(dev, O_RDWR|O_NOCTTY, 0);
    if(comFd < 0)
    {
        return -1;
    }
    
    if (fcntl(comFd, F_SETFL, 0) < 0)
    {
        close(comFd);
        return -1;
    }
    
    bzero(&comTty, sizeof(comTty));
    
    cfsetispeed(&comTty,baud);  /*ÊäÈë*/
    cfsetospeed(&comTty,baud);  /*Êä³ö*/  
    
    comTty.c_cflag  |=  CLOCAL | CREAD;
    comTty.c_cflag &= ~CSIZE;
    comTty.c_cflag |= CS8;
    comTty.c_cflag &= ~PARENB;
    comTty.c_cflag &=  ~CSTOPB;
    
    comTty.c_cc[VTIME]  = 0;    
    comTty.c_cc[VMIN] = 1;
    tcflush(comFd,TCIFLUSH);
    if((tcsetattr(comFd,TCSANOW,&comTty))!=0)
    {
        close(comFd);
        return -1;
    }
    return comFd;
}

int main()
{
	int fd,ret;
	FILE *fp;
	char buf[1024];
	int i;
	
	fd = GpsComOpenCfg("/dev/ttymxc1",B38400);
	if(fd < 0)
		return -1;
	fp = fopen("/home/bic/Code/gps.log","w");
	if(fp == NULL)
		return -1;
	
	while(1)
	{
		memset(buf,0,sizeof(buf));
		ret = read(fd,buf,1023);
		if(ret > 0)
		{
			printf("%s",buf);
			//printf("ret = %d\n",ret);
			//for(i = 0;i < ret;i++)
			//	printf("%02hhx ",buf[i]);
			//printf("\n");
			fprintf(fp,"%s",buf);
		}
	}
	
}