#include<stdio.h>
#include<stdlib.h>
#include <sched.h>
#include <mqueue.h>
#include <fcntl.h>

#define RM_MQUEUE_RM "MQ_RM"

int main(void)
{
	mqd_t id = -1;
	int cnt = 0;
	while(1)
	{
		if(id < 0)
		{
			id = mq_open(RM_MQUEUE_RM,O_WRONLY);
		}
		cnt ++;
		if(id < 0 && cnt >= 5000)
		{
			cnt = 0;
			printf("********** get id failed.\n");
		}
		if(id > 0)
			printf("######### id is OK\n");
		
		usleep(1000);
	}

}