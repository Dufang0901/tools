#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define UPDATE_RECORD_MAX   128
#define UPDATE_FILE_MAX     3
#define BICID_MAX           10

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
}S_UpdateTime;

typedef struct{
	char fileName[128];
	S_UpdateTime updateTime;
	int flag;		/*下载结果标志:0-失败，1-成功*/
}S_FileDownload;

typedef struct{
	char fileName[128];
	S_UpdateTime updateTime;
}S_FileUpdateRecord;

typedef struct{
	char fileName[128];
	S_UpdateTime updateTime;
	int cmd;		/*下载文件后执行的操作:0-无操作 1-重启...*/
	int bicIdCnt;	/*适用的车机数，0表示适用于所有车机*/
	unsigned int bicId[BICID_MAX];	/*适用于哪些车机*/
}S_FileUpdateInfo;

int main(void)
{
	FILE *local_fp, *fp;
	S_FileUpdateRecord updateRecord[UPDATE_RECORD_MAX];
	S_FileDownload     downloadList[UPDATE_FILE_MAX];
	S_FileUpdateInfo   updateInfo[UPDATE_FILE_MAX];
	int rebootFlag,downloadFlag;
	int updateRecordCnt,updateFileCnt,bicIdCnt,downloadCnt,i,j,k;
	char lineStr[1024];
	char *head,*end;
	char cmd1[] = "wget -c ftp://178.100.254.10/updateInfo.txt";
	char cmd2[512],tempFile[128];
	int downloadTime = 5;
	unsigned int bicId = 0x000000;
	
	/*1 先读取本地localUpdateInfo.txt，获取最近一个月下载的文件和时间*/
	if(access("./updateRecord.txt",F_OK) != 0)
		system("touch updateRecord.txt");
	local_fp = fopen("./updateRecord.txt","r");
	if(NULL == local_fp)
	{
		printf("open updateRecord.txt failed,return.\n");
		return -1;
	}
	
	/*读取历史下载记录*/
	updateRecordCnt = 0;
	memset(updateRecord,0,sizeof(updateRecord));
	memset(lineStr,0,sizeof(lineStr));
	
	while(fgets(lineStr,sizeof(lineStr),local_fp) != NULL)
	{
		head = lineStr;
		if((end = strchr(lineStr,',')) == NULL)
		{
			printf("parameter not found,continue.lineStr is %s\n",lineStr);
			continue;
		}
		*end = '\0'; 
		strncpy(updateRecord[updateRecordCnt].fileName,head,sizeof(updateRecord[updateRecordCnt].fileName) - 1);
		sscanf(end + 1,"%04d-%02d-%02d %02d:%02d"
		,&updateRecord[updateRecordCnt].updateTime.year,&updateRecord[updateRecordCnt].updateTime.month
		,&updateRecord[updateRecordCnt].updateTime.day,&updateRecord[updateRecordCnt].updateTime.hour
		,&updateRecord[updateRecordCnt].updateTime.minute);
		
		updateRecordCnt++;
		memset(lineStr,0,sizeof(lineStr));
		if(updateRecordCnt >= UPDATE_RECORD_MAX)
			break;
		
	}
	fclose(local_fp);
	
	/*删除过期历史下载记录*/
	
	
	/*3 下载updateInfo.txt*/
	while(1)
	{
		/*下载前先删除*/
		int ret;
		ret = system("rm ./updateInfo.txt");
		printf("rm updateInfo.txt before download it, ret is %d\n",ret);
		
		do{
			ret = system(cmd1);
			printf("download updateInfo.txt,ret is %d\n",ret);
			downloadTime--;		/*最多下载5次*/
		}while(access("./updateInfo.txt",F_OK) != 0 && downloadTime > 0);
		
		if(access("./updateInfo.txt",F_OK) != 0)
		{
			sleep(30 * 60);
			continue;
		}
		
		/*4 解析updateInfo.txt，确定需要下载的文件名*/
		fp = fopen("./updateInfo.txt","r");
		if(NULL == fp)
		{
			printf("open updateInfo.txt failed,continue.\n");
			system("rm ./updateInfo.txt");
			sleep(30 * 60);
			continue;
		}

		memset(lineStr,0,sizeof(lineStr));
		if(fgets(lineStr,sizeof(lineStr),fp) == NULL)
		{
			printf("read from updateInfo.txt failed,continue.\n");
			fclose(fp);
			system("rm ./updateInfo.txt");
			sleep(30 * 60);
			continue;
		}

		if(strncmp(lineStr,"[file list]",11) != 0)
		{
			printf("head of updateInfo.txt error,continue. head is %s\n",lineStr);
			fclose(fp);
			system("rm ./updateInfo.txt");
			sleep(30 * 60);
			continue;
		}

		memset(lineStr,0,sizeof(lineStr));
		memset(updateInfo,0,sizeof(updateInfo));
		updateFileCnt = 0;
		while(fgets(lineStr,sizeof(lineStr),fp) != NULL)
		{
			/*获取文件名*/
			head = lineStr;
			if((end = strchr(lineStr,',')) == NULL)
				continue;
			*end = '\0';
			strncpy(updateInfo[updateFileCnt].fileName,head,sizeof(updateInfo[updateFileCnt].fileName) - 1);

			/*获取文件时间*/
			head = end + 1;
			if((end = strchr(head,',')) != NULL)
				*end = '\0';
			
			sscanf(head,"%04d-%02d-%02d %02d:%02d"
			,&updateInfo[updateFileCnt].updateTime.year,&updateInfo[updateFileCnt].updateTime.month
			,&updateInfo[updateFileCnt].updateTime.day,&updateInfo[updateFileCnt].updateTime.hour
			,&updateInfo[updateFileCnt].updateTime.minute);
			
			/*获取文件下载后执行的命令(如有)*/
			int tempFlag = 0;	//用于指示是否获取到文件升级后的命令，影响到后面bicId的获取
			if(end == NULL)
			{
				updateInfo[updateFileCnt].cmd = 0;
				updateInfo[updateFileCnt].bicIdCnt = 0;
				updateFileCnt++;
				if(updateFileCnt >= UPDATE_FILE_MAX)
					break;
				else
					continue;
			}
			
			head = end + 1;
			if(strstr(head,"reboot") != NULL || strstr(head,"Reboot") != NULL || strstr(head,"REBOOT") != NULL)
			{
				updateInfo[updateFileCnt].cmd = 1;
				tempFlag = 1;
			}
			else
				updateInfo[updateFileCnt].cmd = 0;

			/*获取文件适用的车机OBUID列表(如有)*/
			if(tempFlag == 1)
			{
				/*这种情况是只填了bicId,没填cmd的情况*/
				if((end = strchr(head,',')) == NULL)
				{
					updateInfo[updateFileCnt].bicIdCnt = 0;
					updateFileCnt++;
					if(updateFileCnt >= UPDATE_FILE_MAX)
						break;
					else
						continue;
				}
				else
					head = end + 1;
			}
			
			bicIdCnt = 0;
			while((end = strchr(head,':')) != NULL)
			{
				*end = '\0';
				sscanf(head,"%06x",&updateInfo[updateFileCnt].bicId[bicIdCnt]);
				head = end + 1;
				bicIdCnt++;
				if(bicIdCnt >= BICID_MAX)
					break;
			}
			if(bicIdCnt <= BICID_MAX)
			{
				sscanf(head,"%06x",&updateInfo[updateFileCnt].bicId[bicIdCnt]);
				updateInfo[updateFileCnt].bicIdCnt = bicIdCnt + 1;
			}

			updateFileCnt++;
			if(updateFileCnt >= UPDATE_FILE_MAX)
					break;
			
		}

		fclose(fp);

#if 1
		/*打印解析出的updateInfo.txt中的信息*/
		printf("##################### update file list #####################\n");
		for(i = 0;i < updateFileCnt;i++)
		{
			printf("%s,%04d-%04d-%04d %02d:%02d,%01d",updateInfo[i].fileName
				,updateInfo[i].updateTime.year,updateInfo[i].updateTime.month
				,updateInfo[i].updateTime.day,updateInfo[i].updateTime.hour
				,updateInfo[i].updateTime.minute,updateInfo[i].cmd);
			if(updateInfo[i].bicIdCnt > 0)
				for(j = 0;j < updateInfo[i].bicIdCnt;j++)
					printf(":%x",updateInfo[i].bicId[j]);
			printf("\n");
		}
#endif
		
		/*找出需要下载的文件*/
		ret = system("rm ./updateInfo.txt");
		printf("rm updateInfo.txt after parse,ret is %d\n",ret);
		
		rebootFlag = 0;
		downloadCnt = 0;
		
		for(i = 0;i < updateFileCnt;i++)
		{
			downloadFlag = 1;

			/*先判断之前有没有下载过该文件*/
			for(j = 0;j < updateRecordCnt;j++)
			{
				if(strcmp(updateInfo[i].fileName,updateRecord[j].fileName) == 0)
				{
					if(updateInfo[i].updateTime.year   == updateRecord[j].updateTime.year &&
					   updateInfo[i].updateTime.month  == updateRecord[j].updateTime.month &&
					   updateInfo[i].updateTime.day    == updateRecord[j].updateTime.day &&
					   updateInfo[i].updateTime.hour   == updateRecord[j].updateTime.hour &&
					   updateInfo[i].updateTime.minute == updateRecord[j].updateTime.minute)
					{
						downloadFlag = 0;
						break;
					}
					   
				}
			}

			/*如果没有下载过，再检查该文件是否适用于本车机*/
			if(downloadFlag == 1 && updateInfo[i].bicIdCnt > 0)
			{
				downloadFlag = 0;
				for(k = 0;k < updateInfo[i].bicIdCnt;k++)
				{
					if(bicId == updateInfo[i].bicId[k])
					{
						downloadFlag = 1;
						break;
					}
				}
				
			}

			/*检查后确定要下载该文件*/
			if(downloadFlag == 1)
			{
				strcpy(downloadList[downloadCnt].fileName,updateInfo[i].fileName);
				memcpy(&downloadList[downloadCnt].updateTime,&updateInfo[i].updateTime,sizeof(S_UpdateTime));
				downloadList[downloadCnt].flag = 0;
				if(updateInfo[i].cmd == 1)
					rebootFlag = 1;
				downloadCnt++;
				
			}
		}

		if(downloadCnt <= 0)
		{
			printf("no file needs to be downloaded,sleep and continue.\n");
			sleep(12 * 60 * 60);
			continue;
		}

#if 1
		/*打印需要下载的文件信息*/
		printf("##################### download list #####################\n");
		for(i = 0;i < downloadCnt;i++)
		{
			printf("%s,%04d-%02d-%02d %02d:%02d\n",downloadList[i].fileName
				,downloadList[i].updateTime.year,downloadList[i].updateTime.month
				,downloadList[i].updateTime.day,downloadList[i].updateTime.hour
				,downloadList[i].updateTime.minute);
		}
		printf("rebootFlag=%d\n",rebootFlag);
#endif
		
		/*5 下载文件，同时删除updateInfo.txt*/
		/*下载文件*/
		int temp = downloadCnt;
		for(i = 0;i < temp;i++)
		{
			memset(cmd2,0,sizeof(cmd2));
			memset(tempFile,9,sizeof(tempFile));
			
			sprintf(cmd2,"wget -c ftp://178.100.254.10/%s",downloadList[i].fileName);
			sprintf(tempFile,"./%s",downloadList[i].fileName);
			downloadTime = 5;
			do{
				ret = system(cmd2);
				printf("download %s,ret is %d\n",downloadList[i].fileName,ret);
				downloadTime--;		/*最多下载5次*/
			}while(access(tempFile,F_OK) != 0 && downloadTime > 0);

			if(access(tempFile,F_OK) != 0)
			{
				downloadList[i].flag = 0;
				
				/*此处downloadCnt为实际下载成功的升级包个数*/
				downloadCnt--;
			}
			else
				downloadList[i].flag = 1;
			
		}

		/*将下载过的文件加入本地已下载列表*/
		if(downloadCnt > 0)
		{
			/*从最后一条记录开始，把原有记录往后挪，如果数组空间不够，就丢弃掉*/
			for(i = updateRecordCnt - 1;i >= 0;i--)
			{
				j = i + downloadCnt;

				if(j < UPDATE_RECORD_MAX)
				{
					memcpy(&updateRecord[j],&updateRecord[i],sizeof(S_FileUpdateRecord));
				}
			}

			for(i = 0;i < downloadCnt;i++)
			{
				strcpy(updateRecord[i].fileName,downloadList[i].fileName);
				memcpy(&updateRecord[i].updateTime,&downloadList[i].updateTime,sizeof(S_UpdateTime));
			}
				
			updateRecordCnt += downloadCnt;

			/*将更新后的下载历史记录写入文件*/
			local_fp = fopen("./updateRecord.txt","w");
			for(i = 0;i < updateRecordCnt;i++)
			{
				fprintf(local_fp,"%s,%04d-%02d-%02d %02d:%02d\n",updateRecord[i].fileName
					,updateRecord[i].updateTime.year,updateRecord[i].updateTime.month
					,updateRecord[i].updateTime.day,updateRecord[i].updateTime.hour
					,updateRecord[i].updateTime.minute);
			}
			fclose(local_fp);
		}

		/*6 下载完成后执行指定操作*/
		if(rebootFlag == 1)
		{
			sync();
			sync();
			sleep(3);
			system("reboot");
		}
		
		sleep(12 * 60 * 60);
		
	}
	
	
}
