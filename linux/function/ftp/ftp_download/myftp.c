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
	int flag;		/*���ؽ����־:0-ʧ�ܣ�1-�ɹ�*/
}S_FileDownload;

typedef struct{
	char fileName[128];
	S_UpdateTime updateTime;
}S_FileUpdateRecord;

typedef struct{
	char fileName[128];
	S_UpdateTime updateTime;
	int cmd;		/*�����ļ���ִ�еĲ���:0-�޲��� 1-����...*/
	int bicIdCnt;	/*���õĳ�������0��ʾ���������г���*/
	unsigned int bicId[BICID_MAX];	/*��������Щ����*/
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
	
	/*1 �ȶ�ȡ����localUpdateInfo.txt����ȡ���һ�������ص��ļ���ʱ��*/
	if(access("./updateRecord.txt",F_OK) != 0)
		system("touch updateRecord.txt");
	local_fp = fopen("./updateRecord.txt","r");
	if(NULL == local_fp)
	{
		printf("open updateRecord.txt failed,return.\n");
		return -1;
	}
	
	/*��ȡ��ʷ���ؼ�¼*/
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
	
	/*ɾ��������ʷ���ؼ�¼*/
	
	
	/*3 ����updateInfo.txt*/
	while(1)
	{
		/*����ǰ��ɾ��*/
		int ret;
		ret = system("rm ./updateInfo.txt");
		printf("rm updateInfo.txt before download it, ret is %d\n",ret);
		
		do{
			ret = system(cmd1);
			printf("download updateInfo.txt,ret is %d\n",ret);
			downloadTime--;		/*�������5��*/
		}while(access("./updateInfo.txt",F_OK) != 0 && downloadTime > 0);
		
		if(access("./updateInfo.txt",F_OK) != 0)
		{
			sleep(30 * 60);
			continue;
		}
		
		/*4 ����updateInfo.txt��ȷ����Ҫ���ص��ļ���*/
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
			/*��ȡ�ļ���*/
			head = lineStr;
			if((end = strchr(lineStr,',')) == NULL)
				continue;
			*end = '\0';
			strncpy(updateInfo[updateFileCnt].fileName,head,sizeof(updateInfo[updateFileCnt].fileName) - 1);

			/*��ȡ�ļ�ʱ��*/
			head = end + 1;
			if((end = strchr(head,',')) != NULL)
				*end = '\0';
			
			sscanf(head,"%04d-%02d-%02d %02d:%02d"
			,&updateInfo[updateFileCnt].updateTime.year,&updateInfo[updateFileCnt].updateTime.month
			,&updateInfo[updateFileCnt].updateTime.day,&updateInfo[updateFileCnt].updateTime.hour
			,&updateInfo[updateFileCnt].updateTime.minute);
			
			/*��ȡ�ļ����غ�ִ�е�����(����)*/
			int tempFlag = 0;	//����ָʾ�Ƿ��ȡ���ļ�����������Ӱ�쵽����bicId�Ļ�ȡ
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

			/*��ȡ�ļ����õĳ���OBUID�б�(����)*/
			if(tempFlag == 1)
			{
				/*���������ֻ����bicId,û��cmd�����*/
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
		/*��ӡ��������updateInfo.txt�е���Ϣ*/
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
		
		/*�ҳ���Ҫ���ص��ļ�*/
		ret = system("rm ./updateInfo.txt");
		printf("rm updateInfo.txt after parse,ret is %d\n",ret);
		
		rebootFlag = 0;
		downloadCnt = 0;
		
		for(i = 0;i < updateFileCnt;i++)
		{
			downloadFlag = 1;

			/*���ж�֮ǰ��û�����ع����ļ�*/
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

			/*���û�����ع����ټ����ļ��Ƿ������ڱ�����*/
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

			/*����ȷ��Ҫ���ظ��ļ�*/
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
		/*��ӡ��Ҫ���ص��ļ���Ϣ*/
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
		
		/*5 �����ļ���ͬʱɾ��updateInfo.txt*/
		/*�����ļ�*/
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
				downloadTime--;		/*�������5��*/
			}while(access(tempFile,F_OK) != 0 && downloadTime > 0);

			if(access(tempFile,F_OK) != 0)
			{
				downloadList[i].flag = 0;
				
				/*�˴�downloadCntΪʵ�����سɹ�������������*/
				downloadCnt--;
			}
			else
				downloadList[i].flag = 1;
			
		}

		/*�����ع����ļ����뱾���������б�*/
		if(downloadCnt > 0)
		{
			/*�����һ����¼��ʼ����ԭ�м�¼����Ų���������ռ䲻�����Ͷ�����*/
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

			/*�����º��������ʷ��¼д���ļ�*/
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

		/*6 ������ɺ�ִ��ָ������*/
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
