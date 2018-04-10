#include<stdio.h>
#include<string.h>

void main()
{
	FILE *fp;
	char temp[25],lineStr[128];
	char *start,*end;
	
	fp=fopen("./voicehint.lst","r");
	memset(lineStr, 0, sizeof(lineStr));
	fgets(lineStr, sizeof(lineStr), fp);
	start = strchr(lineStr,'=');
	if(start == NULL)
	{
		printf("CenterInitVoiceHintName get dispatchTon failed.\n ");
	}
	else
	{
		printf("%s",start);
		while(*start != '\0')
		{
			printf("%x ",*start);
			start++;
		}
		printf("\n");
			
	}
	
}