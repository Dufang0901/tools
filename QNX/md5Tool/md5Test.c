/******************************************************************************/
/* @F_NAME:           main.c                                             */
/* @F_PURPOSE:                                                                */
/* @F_CREATED_BY:       jun.ma                                                  */
/* @F_CREATION_DATE: May 10, 2017                                               */
/* @F_LANGUAGE:      C                                                        */
/* @F_MPROC_TYPE:    processor independent                                    */
/************************************** (C) Copyright 2016 Magneti Marelli ****/

#ifdef __USAGE


#endif

/*______ I N C L U D E - F I L E S ___________________________________________*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "md5.h"

static void print_help()
{
	printf("*********************************************\n");
	printf("** USAGE: rmTest cmd \n");
	printf("**        cmd: 1(session)  - set boot session as client.\n");
	printf("**        cmd: 2(notify)   - shutdown notify.\n");
	printf("**        cmd: 3(status)   - get shutdown status.\n");
	printf("**        cmd: 4(off)      - power off.\n");
	printf("**        cmd: 5(shutdown) - shutdown.\n");
	printf("*********************************************\n");
}


void main(int argc,char *argv[])  
{  
      unsigned char digest[16];  //存放结果  
      printf("######## file name is %s\n",argv[1]);
      bzero(digest,sizeof(digest));
#if 0
      //第一种用法:  
  
     MD5_CTX md5c;  
     MD5Init(&md5c); //初始化  
     MD5UpdaterString(&md5c,"你要测试的字符串");  
     MD5FileUpdateFile(&md5c,"你要测试的文件路径");  
     MD5Final(digest,&md5c);  
#endif
#if 0
     //第二种用法:  
     MDString("你要测试的字符串",digest); //直接输入字符串并得出结果 
#endif
#if 1
    //第三种用法:  
    system("timestamp before");
    MD5File(argv[1],digest); //直接输入文件路径并得出结果  
    system("timestamp after");
#endif

    int i;
    printf("md5 value is: ");
    for(i = 0;i < sizeof(digest);i++)
    {
        printf("0x%hhx  ",digest[i]);
    }
    printf("\n");
}  






