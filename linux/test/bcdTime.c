#include<stdio.h>
#include<string.h>

/*时间:13点24分12秒*/

static unsigned int CalBCDTimerInter(long actTime, long preTime)
{
	unsigned int inter = 0;
	long act;
	long pre;
	char utcTemp[10];
	sprintf(utcTemp, "%lx", actTime);
	act = atoi(utcTemp);
	sprintf(utcTemp, "%lx", preTime);
	pre = atoi(utcTemp);

	act = act/10000 * 3600 + act%10000/100* 60 + act%100;
	pre = pre/10000 * 3600 + pre%10000/100 * 60 + pre%100;

	printf("time = %ld\n",act);
	
	if(act < pre) {
		act += 86400;
	}
	inter = act - pre;
	return inter;
}

int main(void)
{
	unsigned int hour = 13;
	unsigned int minute = 24;
	unsigned int second = 12;

	unsigned int time;

	unsigned int bcdTime;
	bcdTime = (((hour/10) & 0x0f) << 20) | (((hour % 10) & 0x0f) << 16) |
				(((minute/10) & 0x0f) << 12) | (((minute%10) & 0x0f) << 8) |
				(((second/10) & 0x0f) << 4) | ((second%10) & 0x0f);

	/*获取秒数的第一种方法*/
#if 1
	char tmp[10];
	memset(tmp,1,sizeof(tmp));
	sprintf(tmp,"%x",bcdTime);
	printf("%s\n",tmp);
	time = atoi(tmp);
	time = time/10000 * 3600 + time%10000/100* 60 + time%100;

	printf("first method, time=%d\n",time);

#else
	CalBCDTimerInter(bcdTime,0);
#endif
	/*获取秒数的第二种方法*/
	printf("hour:%d\n",((bcdTime >> 20) & 0x0f) * 10 + ((bcdTime >> 16) & 0x0f));
	printf("minute:%d\n",((bcdTime >> 12) & 0x0f) * 10 + ((bcdTime >> 8) & 0x0f));
	printf("second:%d\n",((bcdTime >> 4) & 0x0f) * 10 + (bcdTime & 0x0f));
	time = (((bcdTime >> 20) & 0x0f) * 10 + ((bcdTime >> 16) & 0x0f))*3600 +
			(((bcdTime >> 12) & 0x0f) *10 + ((bcdTime >> 8) & 0x0f)) *60 +
			(((bcdTime >> 4) & 0x0f) * 10 + (bcdTime & 0x0f));
	printf("second method, time=%d\n",time);
	
}
