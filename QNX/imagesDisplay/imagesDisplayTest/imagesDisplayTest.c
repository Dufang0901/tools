#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "imagesDisplay.h"
int main(int argc,char *argv[])
{
	int cmd = -1;
	int fps = 0;
	int fd = -1;
	img_display_t displayCmd;
	int flag = 0;
	int ret;
		
	if(argc >= 2)
	{
		cmd = atoi(argv[1]);
		if(cmd >= 0)
		{
			flag = 1;
			if(1 == cmd || 2 == cmd)
			{
				if(3 == argc)
				{
					fps = atoi(argv[2]);
					if(fps > 0)
					{
						flag = 1;
						printf("set fps to %d\n",fps);
					}
					else
					{
						printf("invalid fps.\n");
					}
				}
				else
				{
					flag = 1;
					fps = 30;
					printf("set fps to 30(default)\n");
				}
			}
		}


	}
	
	if(!flag)
	{
		printf("usage: imgdispTest [cmd] [fps]    cmd: 0-stop 1-FILE_SW 2-FILE_HW 3-BUFFER 4-status\n");
		printf("                                  fps: positive integer, set only when cmd is 1 or 2\n");
		return 1;
	}
	
	fd = open("/dev/imgdisp",O_RDWR);
	if(fd <=0)
	{
		printf("open /dev/imgdisp failed.\n");
		return 1;
	}

	if(1 == cmd)
	{
		displayCmd.fps = fps;
		memset(displayCmd.source,0,sizeof(displayCmd.source));
		strcpy(displayCmd.source,"/appfs/StartUpFile/");
        displayCmd.decoderType = IMGDISP_DECODER_TYPE_SW;
		ret = devctl(fd, DCMD_IMG_DISPLAY_START_FILE, &displayCmd, sizeof(img_display_t), NULL);
		printf("imgdispTest: start animation with SW decoder returns %d\n",ret);
		
	}
    else if(2 == cmd)
    {
        displayCmd.fps = fps;
		memset(displayCmd.source,0,sizeof(displayCmd.source));
		strcpy(displayCmd.source,"/appfs/StartUpFile/");
        displayCmd.decoderType = IMGDISP_DECODER_TYPE_HW;
		ret = devctl(fd, DCMD_IMG_DISPLAY_START_FILE, &displayCmd, sizeof(img_display_t), NULL);
		printf("imgdispTest: start animation with HW decoder returns %d\n",ret);
    }
	else if(0 == cmd)
	{
		ret = -1;
		ret = devctl(fd, DCMD_IMG_DISPLAY_STOP, &displayCmd, sizeof(displayCmd), NULL);
		printf("imgdispTest: stop animation returns %d\n",ret);
	}
	else if(4 == cmd)
	{
		int status = -1;
		int ret = 0;
		ret = devctl(fd, DCMD_IMG_DISPLAY_STATUS, &status, sizeof(status), NULL);
		printf("images_display status is %d, ret is %d\n",status,ret);
	}
	else
	{
		printf("cmd not supported.\n");
	}
	close(fd);
	
}