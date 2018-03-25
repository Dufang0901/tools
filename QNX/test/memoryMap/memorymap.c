#include<stdio.h>
#include<stdlib.h>
#include <sched.h>
#include <stdint.h>
#include <sys/mman.h>
#include <errno.h>
int main(void)
{

	uintptr_t wdt_base;
	unsigned long long cnt = 0;
	while(1)
	{
		usleep(1000);
		cnt++;
		wdt_base = mmap_device_io(100,0x4ae14000);
		if(MAP_DEVICE_FAILED == wdt_base)
		{
			printf("######### mmap_device_io failed,cnt=%lld %s\n",cnt,strerror(errno));
		}
		if(cnt%10000 == 0)
		{
			printf("cnt is %lld\n",cnt);
		}
		if(cnt == 0xefffffffffffffff)
		{
			cnt = 0;
		}
	}
}