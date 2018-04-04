#include<stdio.h>
#include<stdlib.h>
#include <sched.h>
#include <mqueue.h>
#include <fcntl.h>

#define RM_MQUEUE_RM "MQ_RM"

int main(void)
{
 int i;
 int prio;
 for(i = 0;i < 66;i++)
 {
	prio = 8500 - i * 100;
    printf("prio = %02d timeout = %d\n",i,prio);	
 }

}