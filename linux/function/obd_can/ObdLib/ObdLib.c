#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/timerfd.h>
#include <sys/reboot.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>
#include <netdb.h>
#include <termio.h>
#include <getopt.h>
#include <setjmp.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/types.h>
#include <math.h>
#include "ObdLib.h"

static int obdComFd;

static int GetTtyUsbDevice(char *ttyUsbDev)
{
	FILE *pPopenFile;
	char lineStr[256],*p;
	printf("\n\n");
	pPopenFile = popen("dmesg", "r");
	if(pPopenFile != NULL)
	{ 
		/*�ӵڶ��п�ʼΪ�����豸*/
		while(!feof(pPopenFile))
		{
			if(fgets(lineStr,512,pPopenFile) != NULL)		 //��ȡһ����Ϣ
			{
				if(fgets(lineStr,512,pPopenFile) != NULL)		 //��ȡһ����Ϣ
				{
					//printf("%s",lineStr);
					if(strstr(lineStr,"ch341-uart converter now attached to ttyUSB") != NULL)
					{
						if((p = strstr(lineStr,"ttyUSB")) != NULL)
						{
							*(p + 7) = '\0';
							strcpy(ttyUsbDev,"/dev/");
							strcat(ttyUsbDev,p);
							pclose(pPopenFile);
							return 1;
						}
					}
				}
			}
		}
		pclose(pPopenFile);
		return -1;
	}
	return -1;

}

/*********************************************************
    Function:       ObdInit
    Description:    ��ʼ��ELF327ģ��
    Param:          obdComDev   ��ELF327ģ��ͨ�ŵĴ����豸�ļ�
    Return:         
    Other:          none
*********************************************************/ 
static int HexCharToInt(char *hexChar)
{
	int n;

	n = strlen(hexChar);
	if(2 == n)
	{
		int8_t temp;
		sscanf(hexChar,"%02hhx",&temp);
		return (int)temp;
	}
	else if(5 == n)
	{
		char temp[2];
		short result;
		sscanf(hexChar,"%02hhx %02hhx",&temp[1],&temp[0]);
		result = (unsigned char)temp[1] << 8 | (unsigned char)temp[0];
		return (int)result;
		
	}
	else
	{
		printf("%s:input error.\n",__func__);
		return -1;
	}
}

/*********************************************************
    Function:       ObdInit
    Description:    ��ʼ��ELF327ģ��
    Param:          obdComDev   ��ELF327ģ��ͨ�ŵĴ����豸�ļ�
    Return:         
    Other:          none
*********************************************************/ 
static int ObdSendMsg(char *sendBuf,int sendLen)
{
	size_t wByte = 0;
	size_t n;
	while(wByte < sendLen)
	{
		if((n = write(obdComFd,sendBuf + wByte,sendLen - wByte)) > 0)
		{
			wByte += n;
		}
		else
		{
			return OBD_ERROR;
		}
	}
#if 0
	char *p = (char *)sendBuf;
	printf("send:%s\n",p);
#endif
	return OBD_OK;
	   

}
/*********************************************************
    Function:       ObdInit
    Description:    ��ʼ��ELF327ģ��
    Param:          obdComDev   ��ELF327ģ��ͨ�ŵĴ����豸�ļ�
    Return:         
    Other:          none
*********************************************************/ 
static int ObdRecvMsg(char *recvBuf,int recvLen,int timeOut)
{
	size_t n;
	int rByte;
	fd_set fds;
	int maxFd;
	struct timeval selectTime;
	int i;
	char *p;

	rByte = 0;
	selectTime.tv_sec = 0;
	selectTime.tv_usec = timeOut * 1000;

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(obdComFd,&fds);
		maxFd = obdComFd + 1;

		select(maxFd,&fds,NULL,NULL,&selectTime);
		if(FD_ISSET(obdComFd,&fds))
		{
			n = read(obdComFd,recvBuf + rByte,recvLen - rByte);
			if(n <= 0)
			{
				return OBD_ERROR;
			}
			else
			{
				rByte += n;

				/*ת�����з������ڴ�ӡ*/
				for(i = 0;i < rByte;i++)
				{
					if(recvBuf[i] == 0x0D)
						recvBuf[i] = '\n';
				}
				
#if 0
				p = (char *)recvBuf;
				for(i = 0;i < rByte;i++)
					printf("%c",p[i]);
				printf("\n");
#endif

				if(rByte > 10 && strstr(recvBuf,"SEARCHING") == NULL)
					return rByte;
				else
					continue;
			}
		}
		else
		{
			if(rByte > 0)
			{
#if 0
				p = (char *)recvBuf;
				for(i = 0;i < rByte;i++)
					printf("%c",p[i]);
				printf("\n");
#endif

				return rByte;
			}
			else
			{
				return OBD_ERROR_TIMEOUT;
			}
		}
	}
	
}

/*********************************************************
    Function:       ObdInit
    Description:    ��ʼ��ELF327ģ��
    Param:          obdComDev   ��ELF327ģ��ͨ�ŵĴ����豸�ļ�
    Return:         
    Other:          none
*********************************************************/ 
int ObdInit(const char *obdComDev)
{
	int comFd;
    struct termios comTty;
	int recvLen,i;
	char ttyDev[50];
	bzero(ttyDev,50);
    
    comFd = open(obdComDev, O_RDWR|O_NOCTTY, 0);
    if(comFd < 0)
    {
		printf("open dev %s failed,return.\n");
		return OBD_ERROR;
#if 0

    	printf("ObuInit:open device %s failed,get obd com device automatically.\n",obdComDev);
		GetTtyUsbDevice(ttyDev);
		comFd = open(ttyDev, O_RDWR|O_NOCTTY, 0);
		if(comFd < 0)
		{
			printf("ObuInit:open automatically got device %s failed,return.\n",ttyDev);
			return OBD_ERROR;
		}
		printf("open %s successful.\n",ttyDev);
#endif

	}
    
    if (fcntl(comFd, F_SETFL, 0) < 0)
    {
        return -1;
    }
    
    bzero( &comTty, sizeof( comTty ) );
    cfsetispeed(&comTty,B38400);  /*����*/
    cfsetospeed(&comTty,B38400);  /*���*/  
    
    comTty.c_cflag  |=  CLOCAL | CREAD;
    comTty.c_cflag &= ~CSIZE;
    comTty.c_cflag |= CS8;
    
    comTty.c_cflag &= ~PARENB;
    
    
    comTty.c_cflag &=  ~CSTOPB;
    /*��ʱ�ȴ�ʱ��200����*/
    comTty.c_cc[VTIME]  = 2;
    comTty.c_cc[VMIN] = 0;
    tcflush(comFd,TCIFLUSH);
    if((tcsetattr(comFd,TCSANOW,&comTty))!=0)
    {
        return OBD_ERROR;
    }
	obdComFd = comFd;

	/*��ʼ��elf327ģ��*/
	char obdMsg[50];
	char obdRecvMsg[50];
	//��λelf327ģ��,Ӧ��20�ֽ�
	bzero(obdMsg,50);
	bzero(obdRecvMsg,50);
	strcpy(obdMsg,"atz\r\n");
	ObdSendMsg(obdMsg,5);
	ObdRecvMsg(obdRecvMsg,50,5000);

	//�Զ����ҿ���Э�鲢��ΪĬ�ϣ�Ӧ��11�ֽ�
	bzero(obdMsg,50);
	bzero(obdRecvMsg,50);

	strcpy(obdMsg,"atsp0\r\n");
	ObdSendMsg(obdMsg,7);
	ObdRecvMsg(obdRecvMsg,50,10000);
	if(strstr(obdRecvMsg,"OK") != NULL)
		return OBD_OK;
#if 0

	//�鿴����Э��,Ӧ��39�ֽ�
	bzero(obdMsg,50);
	bzero(obdRecvMsg,50);
	strcpy(obdMsg,"0100\r\n");
	ObdSendMsg(obdMsg,6);
	printf("%s:%d\n",__func__,__LINE__);
	recvLen = ObdRecvMsg(obdRecvMsg,50,10000);
	printf("%s:%d\n",__func__,__LINE__);
	if(strstr(obdRecvMsg,"41 00") != NULL)
		return OBD_OK;
	else
		return OBD_ERROR;
	
#endif
	
    return OBD_ERROR;
}

/*********************************************************
    Function:       ObdDataGet
    Description:    ��ȡobd����
    Param:          type    ��ȡ����������
    				value   �����ȡ�������ݵ�ָ��
    Return:         ���
    Other:          none
*********************************************************/ 
int ObdDataGet(int type,int *value)
{
	char obdSendMsg[10],obdRecvMsg[128];
	int recvLen;
	char *p;

	memset(obdSendMsg,0,sizeof(obdSendMsg));
	memset(obdRecvMsg,0,sizeof(obdRecvMsg));
	*value = 0;
	switch(type)
	{
		case OBU_DATA_TEMP:	//Ӧ��17�ֽ�
		{
			strcpy(obdSendMsg,"0105\r\n");

			ObdSendMsg(obdSendMsg,strlen(obdSendMsg));
			if((recvLen = ObdRecvMsg(obdRecvMsg,sizeof(obdRecvMsg),10000)) > 0)
			{
				if((p = strstr(obdRecvMsg,"41 05")) != NULL)
				{
					p = p + 6;
					*(p + 2) = '\0';
					*value = HexCharToInt(p) - 40;
					return OBD_OK;
				}
				else
					return OBD_ERROR;
			}
			else
				return OBD_ERROR;
		}
		break;
		case OBU_DATA_RPM:	//Ӧ��20�ֽ�
		{
			strcpy(obdSendMsg,"010c\r\n");

			ObdSendMsg(obdSendMsg,strlen(obdSendMsg));
			if((recvLen = ObdRecvMsg(obdRecvMsg,sizeof(obdRecvMsg),10000)) > 0)
			{
				if((p = strstr(obdRecvMsg,"41 0C")) != NULL)
				{
					p = p + 6;
					*(p + 5) = '\0';
					*value = HexCharToInt(p)/4;
					return OBD_OK;
				}
				else
					return OBD_ERROR;
			}
			else
				return OBD_ERROR;
		}
		break;
		case OBU_DATA_PRESSURE:
		{
			strcpy(obdSendMsg,"010a\r\n");

			ObdSendMsg(obdSendMsg,strlen(obdSendMsg));
			if((recvLen = ObdRecvMsg(obdRecvMsg,sizeof(obdRecvMsg),10000)) > 0)
			{
				if((p = (strstr(obdRecvMsg,"41 0A"))) != NULL)
				{
					p = p + 6;
					*(p + 2) = '\0';
					*value = HexCharToInt(p);
					return OBD_OK;
				}
				else
					return OBD_ERROR;
			}
			else
				return OBD_ERROR;
			

		}
		break;
		case OBU_DATA_SPEED:	//Ӧ��17�ֽ�
		{
			strcpy(obdSendMsg,"010d\r\n");

			ObdSendMsg(obdSendMsg,strlen(obdSendMsg));
			if((recvLen = ObdRecvMsg(obdRecvMsg,sizeof(obdRecvMsg),10000)) > 0)
			{
				if((p = (strstr(obdRecvMsg,"41 0D"))) != NULL)
				{
					p = p + 6;
					*(p + 2) = '\0';
					*value = HexCharToInt(p);
					return OBD_OK;
				}
				else
					return OBD_ERROR;
			}
			else
				return OBD_ERROR;
			
		}
		break;
		case OBU_DATA_INLET_TEMP:	//Ӧ��17�ֽ�
		{

			strcpy(obdSendMsg,"010f\r\n");

			ObdSendMsg(obdSendMsg,strlen(obdSendMsg));
			if((recvLen = ObdRecvMsg(obdRecvMsg,sizeof(obdRecvMsg),10000)) > 0)
			{
				if((p = (strstr(obdRecvMsg,"41 0F"))) != NULL)
				{
					p = p + 6;
					*(p + 2) = '\0';
					*value = HexCharToInt(p) - 40;
					return OBD_OK;
				}
				else
					return OBD_ERROR;
			}
			else
				return OBD_ERROR;
			
		}
		break;
		case OBU_DATA_DUR_TIME:	//Ӧ��20�ֽ�
		{
			strcpy(obdSendMsg,"011f\r\n");

			ObdSendMsg(obdSendMsg,strlen(obdSendMsg));
			if((recvLen = ObdRecvMsg(obdRecvMsg,sizeof(obdRecvMsg),10000)) > 0)
			{
				if((p = (strstr(obdRecvMsg,"41 1F"))) != NULL)
				{
					p = p + 6;
					*(p + 5) = '\0';
					*value = HexCharToInt(p);
					return OBD_OK;
				}
				else
					return OBD_ERROR;
			}
			else
				return OBD_ERROR;
			
		}
		break;
		default:
		break;

	}

	return OBD_ERROR;

}

/*********************************************************
    Function:       ObdClose
    Description:    �ر�obd
    Param:          none
    Return:         ���
    Other:          none
*********************************************************/ 
void ObdClose(void)
{
	close(obdComFd);
}




