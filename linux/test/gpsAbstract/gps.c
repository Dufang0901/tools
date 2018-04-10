#include<stdio.h>
#include<string.h>

void main()
{
	FILE *fp1,*fp2;
	char line[256],line2[256];
	char *start;
	
	
	fp1 = fopen("1.log","r");
	if(NULL == fp1)
	{
		printf("open 1.log failed.\n");
		return;
	}	
	fp2 = fopen("2.log","w");
	if(NULL == fp2)
	{
		printf("open 2.log failed.\n");
		return;
	}	
	while(!feof(fp1))
	{
		if(fgets(line,256,fp1) != NULL)
		{
			if((start = strstr(line,"$GPRMC")) != NULL)
			{
				bzero(line2,256);
				strcpy(line2,start);
				fputs(line2,fp2);
			}
		}
	}
}