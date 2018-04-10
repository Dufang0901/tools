#include  "adpcm.h"

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <unistd.h>

#define CFG_BlkSize     256   
char ch[CFG_BlkSize];  //用来存储wav文件的头信息
char savedata[CFG_BlkSize*4];
 
#if 0
unsigned char RiffHeader[] = {
      'R' , 'I' , 'F' , 'F' , // Chunk ID (RIFF)
      0x70, 0x70, 0x70, 0x70, // Chunk payload size (calculate after rec!)
      'W' , 'A' , 'V' , 'E' , // RIFF resource format type  
   
      'f' , 'm' , 't' , ' ' , // Chunk ID (fmt )
      0x10, 0x00, 0x00, 0x00, // Chunk payload size (0x14 = 20 bytes)
      0x01, 0x00,             // Format Tag ()
      0x01, 0x00,             // Channels (1)
      0x40, 0x1f, 0x00, 0x00, // Sample Rate,  = 16.0kHz
      0x80, 0x3e, 0x00, 0x00, // Byte rate       32.0K
		0x02, 0x00,             // BlockAlign == NumChannels * BitsPerSample/8 
		0x10, 0x00     // BitsPerSample 
    };
 unsigned char RIFFHeader504[] = {
  'd' , 'a' , 't' , 'a' , // Chunk ID (data)
  0x70, 0x70, 0x70, 0x70  // Chunk payload size (calculate after rec!)
};

#else
unsigned char RiffHeader[] = {
      'R' , 'I' , 'F' , 'F' , // Chunk ID (RIFF)
      0x70, 0x70, 0x70, 0x70, // Chunk payload size (calculate after rec!)
      'W' , 'A' , 'V' , 'E' , // RIFF resource format type  
   
      'f' , 'm' , 't' , ' ' , // Chunk ID (fmt )
      0x14, 0x00, 0x00, 0x00, // Chunk payload size (0x14 = 20 bytes)
      0x01, 0x00,             // Format Tag ()
      0x01, 0x00,             // Channels (1)
      0x11, 0x2b, 0x00, 0x00, // Sample Rate,  = 16.0kHz
      0x80, 0x3e, 0x00, 0x00, // Byte rate       32.0K
		0x02, 0x00,             // BlockAlign == NumChannels * BitsPerSample/8 
		0x10, 0x00     // BitsPerSample 
    };
 unsigned char RIFFHeader504[] = {
  'd' , 'a' , 't' , 'a' , // Chunk ID (data)
  0x70, 0x70, 0x70, 0x70  // Chunk payload size (calculate after rec!)
};
#endif

/****************************************************************
函数名称：    main
功能描述：   
输入参数：    none
输出参数：    none
****************************************************************/
void main(void)
{
	FILE *fpi,*fpo;
	unsigned long iLen,temp;
	struct adpcm_state ADPCMstate;
	unsigned long i = 0;
	unsigned long j;
	
	int rc;
	int ret;
	int size;

	if((fpi=fopen("K31011.mp3","rb"))==NULL)  //若打开文件失败，退出
	{
		printf("can't open this file\n");
		printf("\nread error!\n");
		printf("\n%d\n",i);
		exit(0);
	}
	fseek(fpi,0,SEEK_END);   
	iLen=ftell(fpi); 
	printf("\n======================================================\n");
	printf("\n========================%d========================\n",iLen);
	printf("\n======================================================\n");
	if((iLen-44)%CFG_BlkSize)
		iLen = (iLen-44)/CFG_BlkSize+1;
	else
		iLen = (iLen-44)/CFG_BlkSize;

	if((fpo=fopen("new.pcm","wb+"))==NULL)          //若打开文件失败，退出
	{
		printf("can't open this file\n");
		printf("\nwrite error!\n");
		exit(0);
	}
	fseek(fpo,0,SEEK_SET);
	fwrite(RiffHeader,sizeof(RiffHeader),1,fpo);    //写文件riff
	fwrite(RIFFHeader504,sizeof(RIFFHeader504),1,fpo);   //写 data块头
	while(i<iLen)
	{
		fseek(fpi,48+i*CFG_BlkSize,SEEK_SET);
		fread(ch,1,CFG_BlkSize,fpi); 
		////////////////////////添加读取BlockHeader部分开始////////////////////////////////
		if(i == 0)
		{
		
			ADPCMstate.index = 0; //第一个block的index为 0     当前的BlockSize为 256 即采样点数为 （256-4）*2+1 = 505
		}
		else
		{
			ADPCMstate.index = ch[2];
		}
		ADPCMstate.valprev = (short)ch[0] + ((short)(ch[1]))*256;   //每一个block里面帧头有一个未压缩的数据 存储时 先低后高
		savedata[0] = ch[0];     //存储第一个没有被压缩的数据
		savedata[1] = ch[1];     //存储第一个没有被压缩的数据
		////////////////////////添加读取BlockHeader部分结束////////////////////////////////
		adpcm_decoder(&ch[4], &savedata[2], CFG_BlkSize-4, &ADPCMstate);//解码出来了   （256-4）*4 个字节   
		temp = (CFG_BlkSize-4)*4 + 2;
		fseek(fpo,44+i*temp,SEEK_SET);       //开始写声音数据
		fwrite(savedata,temp,1,fpo);
		i++;
	}
	temp *= i;
	RiffHeader[4] = (unsigned char)((40 + temp)&0x000000ff);
	RiffHeader[5] = (unsigned char)(((40 + temp)&0x0000ff00)>>8);
	RiffHeader[6] = (unsigned char)(((40 + temp)&0x00ff0000)>>16);
	RiffHeader[7] = (unsigned char)(((40 + temp)&0xff000000)>>24);
	fseek(fpo,4,SEEK_SET);
	fwrite(&RiffHeader[4],4,1,fpo);
	RiffHeader[40] = (unsigned char)(temp&0x000000ff);
	RiffHeader[41] = (unsigned char)((temp&0x0000ff00)>>8);
	RiffHeader[42] = (unsigned char)((temp&0x00ff0000)>>16);
	RiffHeader[43] = (unsigned char)((temp&0xff000000)>>24);
	fseek(fpo,40,SEEK_SET);
	fwrite(&RiffHeader[40],4,1,fpo);
	
	fseek(fpo,0,SEEK_SET);
	char buf[100]={0};
	fread(buf,1,100,fpo);
	printf("==========================================\n");
	int aaaa;
	for(aaaa=0;aaaa<44;aaaa++)
	{
		if(aaaa%16 == 0)
		{
			printf("\n");
		}
		printf("%02X ",buf[aaaa]);
	}
	printf("\n==========================================\n");
	fclose(fpi);    
	fclose(fpo);  

	printf("\n==========================OK!=========================\n");
}




