/*********************************************************
    说明：测试使用mktime来获得time_t格式的时间。开发中发现用mktime返回值为0
	结果：正常，因为开发时用Uint64（8字节）类型去接收返回值，但打印时用%ld（只能打印4字节）打印，因此打印出来为0，而不是返回值为0
*********************************************************/

#include<stdio.h>
#include<time.h>
#include<stdint.h>

void main()
{
    
	char yctTime[50] = {"20130506120357"};
    struct tm sYctTime;
    char year[5]   = {0};
    char month[5]  = {0};
    char day[5]    = {0};
    char hour[5]   = {0};
    char minute[5] = {0};
    char second[5] = {0};
    time_t yctTimet,sysTime;
    int timeDiff;

	printf("@@@@@@@@@@@@@@@@@@@@@@@@ YctTimeDiffCalc: yctTime is %s.\n",yctTime);
    memcpy(year,yctTime,4);
    month[0]  = yctTime[4];
    month[1]  = yctTime[5];
    day[0]    = yctTime[6];
    day[1]    = yctTime[7];
    hour[0]   = yctTime[8];
    hour[1]   = yctTime[9];
    minute[0] = yctTime[10];
    minute[1] = yctTime[11];
    second[0] = yctTime[12];
    second[1] = yctTime[13];

    printf("year=%s mon=%s day=%s hour=%s min=%s sec=%s\n",year,month,day,hour,minute,second);

    sYctTime.tm_year = atoi(year) - 1900;
    sYctTime.tm_mon  = atoi(month) - 1;
    sYctTime.tm_mday = atoi(day);
    sYctTime.tm_hour = atoi(hour);
    sYctTime.tm_min  = atoi(minute);
    sYctTime.tm_sec  = atoi(second);
    sYctTime.tm_isdst  = 0;

    printf("year=%d mon=%d day=%d hour=%d min=%d sec=%d\n",sYctTime.tm_year,sYctTime.tm_mon,sYctTime.tm_mday,
        sYctTime.tm_hour,sYctTime.tm_min,sYctTime.tm_sec);

    yctTimet = mktime(&sYctTime);
    time(&sysTime);
    printf("@@@@@@@@@@@@@@@@@@@@@@@@ YctTimeDiffCalc: yctTime = %ld  sysTime = %d.\n",yctTimet,sysTime);

    timeDiff = sysTime - yctTimet;
    return;



	
	
}