#include<stdio.h>
#include<stdlib.h>
#include <sched.h>

int main(void)
{
	system("devmon");
	while(1)
	{
		system("cat /dev/ti/devmon");
		sleep(5);
	}
}