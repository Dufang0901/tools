#include<stdio.h>
#include<stdlib.h>
#include <sched.h>
#include <mqueue.h>
#include <fcntl.h>

char *deleteLine[] = 
{
	"[HMIS]",
	"LOG: fps:",
	"IMC task:",
	"IMC State:",
	"Header CRC Error on GPU:",
	"Data CRC Error on GPU:",
	"Header CRC Error on MCU:",
	"Data CRC Error on MCU:",
	"====IMC Task on MCU:",
	"device open:"
	
};

int main(int argc, const char *argv[])
{
 int i;
 int deleteCnt;
 FILE *fp = NULL;
 FILE *fp1 = NULL;
 char lineStr[1024];
 char tmp[256];
 int deleteFlag = 0;
 
 deleteCnt = sizeof(deleteLine) / sizeof(deleteLine[0]);
 
 /*open original file and outcome file*/
 fp = fopen(argv[1],"r");
 if(NULL == fp)
 {
	 printf("open original file %s error, exit.\n",argv[1]);
	 return 1;
 }
 
 bzero(tmp,sizeof(tmp));
 snprintf(tmp,sizeof(tmp) - 1,"%s_after",argv[1]);
 fp1 = fopen(tmp,"w");
 if(NULL == fp1)
 {
	printf("open result file %s error, exit.\n",tmp);
	return 1;
 }
 
 /*read one line of original file*/
 bzero(lineStr,sizeof(lineStr));
 while(NULL != fgets(lineStr,sizeof(lineStr) - 1,fp))
 {
	 /*check if it is the line to be deleted*/
	 deleteFlag = 0;
	 for(i = 0;i < deleteCnt;i++)
	 {
		if(0 == strncmp(lineStr,deleteLine[i],strlen(deleteLine[i])))
		{
			deleteFlag = 0;
			break;
		}
	 }
	 
	 if(!deleteFlag)
	 {
		 fputs(lineStr,fp1);
	 }
 }
 
 fclose(fp);
 fclose(fp1);
 system("sync");
 system("sync");
 

}