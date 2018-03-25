#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "imagesDisplay.h"
int main(int argc,char *argv[])
{
	int status = -1;
	int ret = -1;
	img_display_t displayCmd;
	int fd = -1;
	int startFlag = 0;    /*1-发了start命令   2-发了stop命令*/
	
	while(1)
	{
		printf("stressTest: open device.\n");
		fd = open("/dev/imgdisp",O_RDWR);
		if(fd <=0)
		{
			printf("open /dev/imgdisp failed.\n");
			break;
		}
		
		/*get images_display status*/
		printf("stressTest: get device status.\n");
		ret = devctl(fd, DCMD_IMG_DISPLAY_STATUS, &status, sizeof(status), NULL);
		printf("images_display status is %d, ret is %d\n",status,ret);
		
		if(IMGDISP_STATUS_DONE == status && startFlag != 2)
		{
			startFlag = 2;
			devctl(fd, DCMD_IMG_DISPLAY_STOP, &displayCmd, sizeof(displayCmd), NULL);
		}
		else if(IMGDISP_STATUS_STOP == status && startFlag != 1)
		{
			startFlag = 1;
			displayCmd.fps = 60;
			memset(displayCmd.source,0,sizeof(displayCmd.source));
			strcpy(displayCmd.source,"/appfs/StartUpFile/");
			printf("stressTest: start animation.\n");
			devctl(fd, DCMD_IMG_DISPLAY_START_FILE, &displayCmd, sizeof(img_display_t), NULL);
		}
		else if(IMGDISP_STATUS_RUNNING != status && startFlag == 1)
		{
			/*已经发了start命令但是没有start*/
			printf("stressTest: ################################start command issued but animation not running...\n");
			
		}
		else if(IMGDISP_STATUS_STOP != status && startFlag == 2)
		{
			printf("stressTest: ################################stop command issued but animation not stopped...\n");
		}

		

		
		close(fd);
		
		usleep(50 * 1000);
	}
	
	
}