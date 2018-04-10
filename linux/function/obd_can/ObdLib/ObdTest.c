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

int main(int argc, const char *argv[])
{
	char cmd;
	int type,value;
	/*获取OBD数据步骤如下*/

	/*1 初始化*/
	printf("open device %s\n",argv[1]);
	if(ObdInit(argv[1]) < 0)
	{
		printf("obd init error,return.\n");
		//return -1;
	}

	printf("obd init successful.\n");
	while(1)
	{
		cmd = getchar();
		
		if(cmd > '0' && cmd < '9')
		{
			printf("cmd = %c\n",cmd);
			type = cmd - '0';
			printf("type is %d\n",type);

			if(ObdDataGet(type,&value) < 0)
			{
				printf("obd get date error.\n");
				continue;
			}
			switch(type)
			{
				case 1:
					printf("cooler temperature is %d℃\n",value);
				break;
				case 2:
					printf("engine rotation speed is %d rpm\n",value);
				break;
				case 3:
					printf("oil pipe pressure is %d KPa\n",value);
				break;
				case 4:
					printf("vehicle speed is %d Km/h\n",value);
				break;
				case 5:
					printf("inlet temperature is %d ℃\n",value);
				break;
				case 6:
					printf("engine operating time is %d s\n",value);
				break;
				default:
					break;
			}
		}
		
	}
}

