/******************************************************************************
˵�������������fwrite��fread������ʹ���з���fwriteÿ�η���ֵ��Ϊ1��fread����ֵ
	Ҳ��Ԥ�ڲ�һ����
���Խ��ۣ�fwrite��fread�ķ���ֵ�ǳɹ�д����ȡ�����ݿ����������������������
	��������һ������������д����ȡ���ֽ�����
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
struct info
{
	int type;
	int sn;
	char msg[128];
};

struct record {  
    char name[10];  
    int age;  
};  
void main()
{
#if 1
	struct info a;
	size_t ret;
	
	a.type = 1;
	a.sn = 1008;
	strcpy(a.msg,"of course you are still loved.");
	
	
	/*д��*/
	int i;
	FILE *fp;
	fp = fopen("1.txt","w");
	if(fp == NULL)
	{
		printf("open file failed.\n");
		return;
	}
    for(i = 0;i < 5;i++)
	{
		ret = fwrite(&a,sizeof(struct info),1,fp);
		printf("write %d bytes,sizeof info is %d.\n",ret,sizeof(struct info));
	}
	fclose(fp);
	sync();
	
	/*��ȡ*/
	struct info b;
	
	fp = fopen("1.txt","r");
	if(fp == NULL)
	{
		printf("open file for read failed.\n");
		return;
	}
	for(i = 0;i < 5;i++)
	{
		ret = fread(&b,sizeof(struct info),1,fp);
	    printf("%d:sn=%d type=%d info=%s\n",i+1,b.sn,b.type,b.msg);
		
	}
	return;
#else

	/*д��*/
	struct record array[2] = {{"Ken", 24}, {"Knuth", 28}};  
    FILE *fp = fopen("recfile", "w"); 
	size_t retSize;
    if (fp == NULL) {  
        perror("Open file recfile");  
        exit(1);  
    }  
    retSize = fwrite(array, sizeof(struct record), 2, fp);  
	printf("%d bytes are written.\n",retSize);
    fclose(fp);  
    
	/*��ȡ*/
    struct record array2[2];  
    fp = fopen("recfile", "r");  
    if (fp == NULL) {  
        perror("Open file recfile");  
        exit(1);  
    }  
    retSize = fread(array2, sizeof(struct record), 2, fp);  
	printf("%d bytes are read.\n",retSize);
    printf("Name1: %s\tAge1: %d\n", array2[0].name, array2[0].age);  
    printf("Name2: %s\tAge2: %d\n", array2[1].name, array2[1].age);  
    fclose(fp);  
 #endif
}