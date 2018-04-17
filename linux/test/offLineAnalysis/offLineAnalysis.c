/*********************************************************************/
/*从online进程日志中分析出断网次数和每次断网的时间。当一次断网时间过 */
/*长（如超过2分钟）时，认为是有问题的。                              */
/*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>

static int cnt,cnt0,cnt1,cnt2,cnt3,cnt4,cnt5;

time_t GetTimetFromTm(int year,int month,int day,int hour,int minute,int second)
{
	struct tm sTime;
	
	sTime.tm_year = year - 1900;
	sTime.tm_mon  = month - 1;
	sTime.tm_mday = day;
	sTime.tm_hour = hour;
	sTime.tm_min  = minute;
	sTime.tm_sec  = second;
	
	return mktime(&sTime);
	
}

int CountTime(int time)
{
	if(time < 0)
	{
		printf("offLine time error, time is %d.\n",time);
		return -1;
	}
	if(0 == time)
	{
		cnt0++;
	}
	else if(time > 0 && time <= 60)
	{
		//小于1分钟的次数
		cnt1++;
	}
	else if(time > 60 && time <=120)
	{
		//1到2分钟的次数
		cnt2++;
	}
	else if(time > 120 && time <= 180)
	{
		//2到3分钟的次数
		cnt3++;
	}
	else if(time > 180 && time <= 240)
	{
		//3到4分钟的次数
		cnt4++;
	}
	else if(time > 240)
	{
		//大于4分钟的次数
		cnt5++;
	}
	cnt++;
	return 1;
}

int main(int argc, const char *argv[])
{
	FILE *fp;
	char lineStr[1024] = {0};
	int year,month,day,hour,minute,second,msecond,state;
	time_t start_time,end_time;
	int offLineTime;
	
	int onlineFlag = 0;
	
	//参数检查
	if(argc != 2)
	{
		printf("argument number error.\n");
		return -1;
	}
	if(argv[1] == NULL)
	{
		printf("invalid argument.\n");
		return -1;
	}
	
	//打开日志文件
	if((fp = fopen(argv[1],"r")) == NULL)
	{
		printf("open log file failed.\n");
		return -1;
	}
	
	start_time = end_time = 0;
	//读取日志文件内容
	while(fgets(lineStr,1023,fp) != NULL)
	{
		if(strstr(lineStr,"Ol U7500:Pppd stat change to") != NULL)
		{
			sscanf(lineStr,"Date:%04d-%02d-%02d %02d:%02d:%02d.%d <  INFO>,[Online]--Ol U7500:Pppd stat change to %d."
			,&year,&month,&day,&hour,&minute,&second,&msecond,&state);
			
			//printf("state = %d\n",state);
			if(1 == state)
			{
				if(0 == onlineFlag)
				{
					/*一次掉线结束*/
					end_time = GetTimetFromTm(year,month,day,hour,minute,second);
					//printf("offline\n");
					
					if(start_time != 0)
					{
						//printf("offline confirm\n");
						//计算掉线时间
						offLineTime = end_time - start_time;
						
						//计数
						CountTime(offLineTime);
					}
					start_time = end_time = 0;
				}
				onlineFlag = 1;
			}
			else
			{
				if(1 == onlineFlag)
				{
					/*一次掉线开始*/
					start_time = GetTimetFromTm(year,month,day,hour,minute,second);
					end_time   = 0;
				}
				onlineFlag = 0;
			}
			
		}
	}
	
	printf("############### %04d年%02d月%02d日  result ##################\n",year,month,day - 1);
	printf("总掉线次数：%d，其中：\n",cnt);
	printf("马上恢复           %d 次\n",cnt0);
	printf("小于1分钟          %d 次\n",cnt1);
	printf("大于1分钟小于2分钟 %d 次\n",cnt2);
	printf("大于2分钟小于3分钟 %d 次\n",cnt3);
	printf("大于3分钟小于4分钟 %d 次\n",cnt4);
	printf("大于5分钟          %d 次\n",cnt5);
	
}