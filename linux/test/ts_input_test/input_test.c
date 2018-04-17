#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/input.h>



int main(int argc, char *argv[])
{
	int fd;
	char buf[32];
	int ret;
	int on;

	struct input_event event;

	fd = open("/dev/input/mouse0", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}


	while(1)
	{
		ret = read(fd, &event, sizeof(struct input_event));
		if(ret < 0)
		{
			perror("read");
			exit(1);
		}
		printf("input event(type=%02x code=%02x):",event.type,event.code);
		if(event.type == EV_ABS)
		{
			if(event.code == ABS_X)
			{
				printf(" x = %04d ",event.value);
			}
			else if(event.code == ABS_Y)
			{
				printf(" y = %04d ",event.value);
			}
		}
		
		printf("\n");
	}
	
	close(fd);

	return 0;
}


