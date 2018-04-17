#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
//#include <linux/module.h>
//#include <linux/slab.h>
#include <linux/input.h>
#include <linux/serio.h>
//#include <linux/init.h>

int fd_mouse;


void simulate_syn()
{
    struct input_event event;
    event.type = EV_SYN;
	event.code = SYN_REPORT;//要模拟成什么按键
    event.value = 0;//是按下还是释放按键或者重复
    gettimeofday(&event.time, 0);
    if(write(fd_mouse, &event, sizeof(event)) < 0)
    {
        printf("key error~~~\n");
        return ;
    }
}
void simulate_key(int code,int value)
{
    struct input_event event;
    event.type = EV_KEY;
	event.code = code;//要模拟成什么按键
    event.value = value;//是按下还是释放按键或者重复
    gettimeofday(&event.time, 0);
    if(write(fd_mouse, &event, sizeof(event)) < 0)
    {
        printf("key error~~~\n");
        return ;
    }
	simulate_syn();
}


void simulate_mouse(int x,int y)
{
    static struct input_event event, ev;

    event.type = EV_REL;
    event.code = REL_X;
    event.value = x/2;
    gettimeofday(&event.time, 0);
    if( write(fd_mouse, &event, sizeof(event)) != sizeof(event))
        printf("xxxxxx \n");
    event.code = REL_Y;
    event.value = y/2;
    gettimeofday(&event.time, 0);
    if( write(fd_mouse, &event, sizeof(event)) != sizeof(event))
        printf("yyyy \n");

    //一定要刷新空的
	simulate_syn();
}
void simulate_xy(int x,int y)
{
    static struct input_event event, ev;

    event.type = EV_ABS;
    event.code = ABS_X;
    event.value = x;
    gettimeofday(&event.time, 0);
    if( write(fd_mouse, &event, sizeof(event)) != sizeof(event))
        printf("xxxxxx \n");
	
	simulate_syn();
    event.code = ABS_Y;
    event.value = y;
    gettimeofday(&event.time, 0);
    if( write(fd_mouse, &event, sizeof(event)) != sizeof(event))
        printf("yyyy \n");

	simulate_syn();
	
	
	event.type = EV_ABS;
	event.code = ABS_PRESSURE;
    event.value = 0;
    gettimeofday(&event.time, 0);
	write(fd_mouse, &event, sizeof(event));
	
	simulate_syn();
	
	event.type = EV_KEY;
	event.code = BTN_TOUCH;
    event.value = 1;
    gettimeofday(&event.time, 0);
	write(fd_mouse, &event, sizeof(event));
	
	simulate_syn();
}

int main(int argc,char *argv[])
{

	int a;int b;int c;
	fd_mouse = open("/dev/input/event0", O_RDWR);
	if(fd_mouse < 0 )
	{
		printf("open error.\n");
		return;
	}
	while(1)
	{			
		printf("input[left-0,right-1,mid-2,move-3,xy-4] [x] [y].\n");
		scanf("%d %d %d",&a,&b,&c);
		switch(a)
		{
		case 0:
			simulate_key(BTN_LEFT,1);
			simulate_key(BTN_LEFT,0);
			break;
		case 1:
			simulate_key(BTN_RIGHT,1);
			simulate_key(BTN_RIGHT,0);
			break;	
		case 2:
			simulate_key(BTN_MIDDLE,1);
			simulate_key(BTN_MIDDLE,0);
			break;		
		case 3:
			simulate_mouse(b,c);
			break;		
		case 4:
			simulate_xy(b,c);
			break;	
		default:
			break;
		}
		
	}
	return 0;
}
