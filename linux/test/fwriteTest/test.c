/******************************************************************************
说明：本程序测试fwrite和fread函数。使用中发现fwrite每次返回值都为1，fread返回值
	也跟预期不一样。
测试结论：fwrite和fread的返回值是成功写入或读取的数据块的数量（即正常情况下与第
	三个参数一样），而不是写入或读取的字节数。
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
	
	
	/*写入*/
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
	
	/*读取*/
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

	/*写入*/
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
    
	/*读取*/
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