#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/timerfd.h>
#include <sys/reboot.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <fcntl.h>
//#include <iostream>
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

#include <linux/if.h>
#include <linux/i2c.h>  
#include <linux/i2c-dev.h> 
#include <linux/reboot.h>
#include <linux/watchdog.h>
#include <linux/types.h>

typedef char        Char;
typedef int8_t      Int8;
typedef uint8_t     Uint8;
typedef int16_t     Int16;
typedef uint16_t    Uint16;
typedef int32_t     Int32;
typedef uint32_t    Uint32;
typedef int64_t     Int64;
typedef uint64_t    Uint64;

/*线路服务信息*/
typedef struct
{
	Char serviceCoding[2];						/*服务编码*/
	Char direction;
	Uint8 siteNum;								/*站点数*/
	Char siteStartName[48];		/*起点站*/
	Char siteEndName[48];		/*终点站*/
	Char siteStartCoding[16];		/*终点站*/
	Char siteEndCoding[16];		/*终点站*/
}S_RouteServiceInfo;

static int RouteInfoParGet(const Char *lineStr,Char *par,Uint32 parNum,Uint32 parMax)
{
    char *parStart;
    char *parEnd;

    Uint32 parPtr = 0;

    if(parNum > parMax)
    {
        return -1;
    }
    if((parStart = strchr(lineStr,'=')) != NULL)
    {
        do 
        {
            parStart += 1;
            if(++parPtr == parNum)
            {
                if(parPtr == parMax)
                {
                    if((parEnd = strchr(parStart,'\n')) != NULL)
                    {
                        strncpy(par,parStart,parEnd - parStart);
                        par[parEnd - parStart] = '\0';
                        return parEnd - parStart;
                    }
                    else
                    {
                        return -1;
                    }
                }
                else
                {
                    if((parEnd = strchr(parStart,',')) != NULL)
                    {
                        strncpy(par,parStart,parEnd - parStart);
                        par[parEnd - parStart] = '\0';
                        return parEnd - parStart;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
            else
            {
                if((parStart = strchr(parStart,',')) == NULL)
                {
                    return -1;
                }
            }
        } while (parPtr < parNum);
    }
    else
    {
        return -1;
    }
    return 0;
}


int RouteServiceInfoGet(void *pRouteBasicInfo,const char *routeInfoFile,const char *routeCoding)
{
 	FILE    *routeFilePtr;
    Char    lineStr[512];           /*用于保存每行的字符串*/
	Char    siteName[48];
	Char    siteCoding[16];
	Char    *end;
	Uint32  startFlag = 0; 
	S_RouteServiceInfo *serviceInfo = (S_RouteServiceInfo *)pRouteBasicInfo;
	Uint32  startPtr = 0;

	routeFilePtr = fopen(routeInfoFile,"r");
    if(NULL == routeFilePtr)
    {
        printf("RouteServiceInfoGet RouteInfoFileOpen error  %s\n",routeInfoFile);
		return -1;
    }
    
    do
    {
    	memset(lineStr,0,512);
        if(fgets(lineStr,512,routeFilePtr) != NULL)         //读取一行信息
        {
        	if((lineStr[0] == '[') && (lineStr[3] == ']'))
            {
        		if (2 == startFlag)
				{
					memcpy(serviceInfo[startPtr].siteEndName,siteName,sizeof(siteName));
					memcpy(serviceInfo[startPtr].siteEndCoding,siteCoding,sizeof(siteCoding));
					startPtr++;							
					startFlag = 0;
				}							
            	serviceInfo[startPtr].siteNum = 0;
            	serviceInfo[startPtr].serviceCoding[0] = lineStr[1];
				serviceInfo[startPtr].serviceCoding[1] = lineStr[2];
			    startFlag = 1;
            }
			else if(strncmp(lineStr,routeCoding,5) == 0)
			{
				memset(siteName,0,64);
				serviceInfo[startPtr].siteNum ++;
				serviceInfo[startPtr].direction = lineStr[5];
				RouteInfoParGet(lineStr, siteName,1,6);

				end = strchr(lineStr,'=');
				memcpy(siteCoding,lineStr,end-lineStr);
				siteCoding[end-lineStr]='\0';
					
				if(1 == startFlag)
				{
					startFlag = 2;

					memcpy(serviceInfo[startPtr].siteStartName,siteName,sizeof(siteName));
					memcpy(serviceInfo[startPtr].siteStartCoding,siteCoding,sizeof(siteCoding));
				}
			}
			else if(strncmp(lineStr,"\n",1) == 0)
			{
				if(1 == startFlag)
				{
					startPtr++;							
					startFlag = 0;
				}
				else if (2 == startFlag)
				{
					memcpy(serviceInfo[startPtr].siteEndName,siteName,sizeof(siteName));
					memcpy(serviceInfo[startPtr].siteEndCoding,siteCoding,sizeof(siteCoding));
					startPtr++;							
					startFlag = 0;
				}
			}
			else
			{

			}

			
        }
		else
		{
			if (2 == startFlag)
			{
				memcpy(serviceInfo[startPtr].siteEndName,siteName,strlen(siteName));
				memcpy(serviceInfo[startPtr].siteEndCoding,siteCoding,sizeof(siteCoding));
				startPtr++;
				startFlag = 0;
			}
		}
		
    } while (!feof(routeFilePtr));
	if (2 == startFlag)
	{
		memcpy(serviceInfo[startPtr].siteEndName,siteName,strlen(siteName));
		memcpy(serviceInfo[startPtr].siteEndCoding,siteCoding,sizeof(siteCoding));
		startPtr++;
		startFlag = 0;
	}
	

	fclose(routeFilePtr);

	return 1;
}

void main()
{

	S_RouteServiceInfo routeServiceInfo[10];
	int ret;
	int i;

	ret = RouteServiceInfoGet(routeServiceInfo, "./1.rdf", "99");
	if(ret<=0)
	{
		printf("route service information get error!\n");
		return;
	}
	for(i=0;i<2;i++)
	{
		printf("NO.%d",i+1);
		printf("serviceCode=%c%c siteNum=%d startSta:%s %s endSta:%s %s\n",
			routeServiceInfo[i].serviceCoding[0],routeServiceInfo[i].serviceCoding[1],
			routeServiceInfo[i].siteNum, routeServiceInfo[i].siteStartCoding,
			routeServiceInfo[i].siteStartName, routeServiceInfo[i].siteEndCoding,
			routeServiceInfo[i].siteEndName);
	}


	
}