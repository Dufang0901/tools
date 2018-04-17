#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <netdb.h>
#include <dirent.h>
#include <time.h>
#include <math.h>
#include "staRepAnalysis.h"

S_Analysis sAnalysis;

int main()
{
	struct dirent ** namelist,**subNamelist;
	int dirCnt,subDirCnt;
	int i,j;
	char tempDir[128];

	
	dirCnt = scandir(LOG_FILE_DIR,&namelist,NULL,alphasort);
	for(i = 0;i < dirCnt;i++)
	{
		printf("%s,type is %hhx\n",namelist[i]->d_name,namelist[i]->d_type);
		bzero(tempDir,sizeof(tempDir));
		sprintf(tempDir,"%s/%s",LOG_FILE_DIR,namelist[i]->d_name);
		if(4 == namelist[i]->d_type && namelist[i]->d_name[0] != '.')
		{
			subDirCnt = scandir(tempDir,&subNamelist,NULL,alphasort);
			for(j = 0;j < subDirCnt;j++)
			{
				printf("%s,type is %hhx\n",subNamelist[j]->d_name,subNamelist[j]->d_type);
				free(subNamelist[j]);
			}
			printf("\n\n");
		}
		free(namelist[i]);
	}
}
