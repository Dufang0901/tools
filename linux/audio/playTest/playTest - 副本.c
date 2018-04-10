#include <stdio.h>
#include <string.h>
#include <alsa/asoundlib.h>
//#include "Rtc.h"

typedef struct 
{
	short valprev;	//Previous output value
	char index;		//Index into stepsize table
}AdpcmState;

typedef struct
{
    char rld[4]; 					//riff 标志符号
    unsigned int rLen; 				//文件长度
    char wld[4]; 					//格式类型（wave）
    char fld[4]; 					//"fmt"
    unsigned int fLen; 				//sizeof(wave format matex)
    short wFormatTag; 				//编码格式
    short wChannels; 				//声道数
    unsigned int nSamplesPersec ; 	//采样频率
    unsigned int nAvgBitsPerSample;	//WAVE文件采样大小
    short wBlockAlign; 				//块对齐    
	short wBitsPerSample; 			//WAVE文件采样大小
} WavCommHead;


/* Intel ADPCM step variation table */
static int AdpcmIndexTable[16] = {
								  -1, -1, -1, -1, 2, 4, 6, 8,
								  -1, -1, -1, -1, 2, 4, 6, 8
								 };

static int AdpcmStepSizeTable[89] = {
								     7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
								     19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
								     50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
								     130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
								     337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
								     876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
								     2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
								     5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
							    	 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
									};

snd_pcm_t    *sPcmHandle;		//pcm播放句柄
snd_pcm_hw_params_t* sPcmParams;	//硬件信息和PCM流配置
WavCommHead  sWavCommHead;		//wav公用头格式



#define VOICE_BUFFER_LEN_MAX	1024*10				//解码后最大数据长度



char         sVoiceBuffer[VOICE_BUFFER_LEN_MAX];	//存放解码后的数据
char         sPlayName[256];						//播放的文件名

static char     logProcessFile[128];
static time_t   t_secs;                     /*计数秒数*/
static struct   tm *t_info;                 /*数据元素：年月日时分秒等*/

#define WAV_PCM_COMM_HEAD_LEN	(sizeof(WavCommHead))
#define WAVE_FORMAT_PCM			0x0001
#define WAVE_FORMAT_DVI_ADPCM	0x0011


static void adpcm_decoder(char *inbuff,char *outbuff,int len_of_in,AdpcmState *state);
static int voicePlayPcm(FILE *fp);
static int voicePlayAdpcm(FILE *fp);
static void BicLogPrintf(const char *fmt,...);

/*********************************************************
    Function:       adpcm_decoder
    Description:    将adpcm格式转换为pcm
    Param:          inbuff     adpcm格式数据
    				outbuff    转换后的pcm格式  
    				len_of_in  adpcm数据长度
    				state      上一次转换状态
    Return:         none
    Other:          none
*********************************************************/
void adpcm_decoder(char *inbuff, char *outbuff, int len_of_in, AdpcmState * state )
{
	int  i=0,j=0;
	char tmp_data;
	long step;					 /* Quantizer step size */
	signed long predsample;		 /* Output of ADPCM predictor */
	signed long diffq;			 /* Dequantized predicted difference */
	int nIndex; /* Index into step size table */
	int Samp;
	unsigned char SampH, SampL;
	unsigned char inCode; 
 
    /* Restore previous values of predicted sample and quantizer step size index */
	predsample = state->valprev;
	nIndex = state->index;

	for(i=0;i<len_of_in*2;i++)
	{
		tmp_data=inbuff[i/2];  
		if(i%2)
			inCode=(tmp_data& 0xf0) >> 4;
		else
			inCode=tmp_data & 0x0f;

		step = AdpcmStepSizeTable[nIndex];
		/* Inverse quantize the ADPCM code into a predicted difference using the quantizer step size*/

		diffq = step >> 3;
		if( inCode & 4 )
			diffq += step;
		if( inCode & 2 )
			diffq += step >> 1;
		if( inCode & 1 )
			diffq += step >> 2;

		/* Fixed predictor computes new predicted sample by adding the old predicted sample to predicted difference	*/
		if( inCode & 8 )
			predsample -= diffq;
		else
	   		predsample += diffq;
		
		/* Check for overflow of the new predicted sample */
		if( predsample > 32767 )
			predsample = 32767;
		else if( predsample < -32768 )
			predsample = -32768;

		/* Find new quantizer stepsize index by adding the old index to a table lookup using the ADPCM code */
		nIndex += AdpcmIndexTable[inCode];

		/* Check for overflow of the new quantizer step size index	*/
		if( nIndex < 0 )
			nIndex = 0;
		if( nIndex > 88 )
			nIndex = 88;

		/* Return the new ADPCM code */
		Samp=predsample;
		if (Samp >= 0)
		{
			SampH=Samp / 256;
			SampL = Samp - 256 * SampH;
		}
		else
		{
			Samp= 32768+Samp;
			SampH = Samp / 256;
			SampL = Samp - 256 * SampH;
			SampH += 0x80;
		}
		outbuff[j++]=SampL;
		outbuff[j++]=SampH;   
 	}
	/* Save the predicted sample and quantizer step size index for next iteration */
	state->valprev = (short)predsample;
	state->index = (char)nIndex;
}

/*********************************************************
    Function:       voicePlayPcm
    Description:    播放wav格式中的pcm格式的语音文件
    Param:          fp     语音文件描述符
    Return:         none
    Other:          none
*********************************************************/
static int voicePlayPcm(FILE *fp)
{
	int ret = 0;
	int dir = 0;
	snd_pcm_format_t pcmFomat = SND_PCM_FORMAT_UNKNOWN;
	snd_pcm_uframes_t frames = 1;
	int readSize ;
	int nSamplesPersec = sWavCommHead.nSamplesPersec;

	//初始化params
	ret = snd_pcm_hw_params_any(sPcmHandle, sPcmParams);
	if(ret < 0)
	{
		  BicLogPrintf("voicePlayPcm snd_pcm_hw_params_any errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return -1;
	}
	//初始化访问权限
	ret = snd_pcm_hw_params_set_access(sPcmHandle, sPcmParams, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(ret < 0)
	{
		  BicLogPrintf("voicePlayPcm snd_pcm_hw_params_set_access errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return -1;
	}

	//采样位数
	switch(sWavCommHead.wBitsPerSample/8)
	{
	case 1:
		pcmFomat = SND_PCM_FORMAT_U8;
		break ;
	case 2:
		pcmFomat = SND_PCM_FORMAT_S16_LE;
		break ;
	case 3:
		pcmFomat = SND_PCM_FORMAT_S24_LE;
		break ;
	default:
		break;
	}
	if(pcmFomat == SND_PCM_FORMAT_UNKNOWN)
	{
		BicLogPrintf("voicePlayPcm pcmFomat unknown\n");
		return -1;
	}
	ret = snd_pcm_hw_params_set_format(sPcmHandle, sPcmParams, pcmFomat);
	if(ret < 0)
	{
		  BicLogPrintf("voicePlayPcm snd_pcm_hw_params_set_format errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return -1;
	}

	//设置声道,1表示单声>道，2表示立体声
	ret = snd_pcm_hw_params_set_channels(sPcmHandle, sPcmParams, sWavCommHead.wChannels);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayPcm snd_pcm_hw_params_set_channels errNo=%d,%s.\n",ret,snd_strerror(ret));
		return -1;		
	}
	ret = snd_pcm_hw_params_set_rate_near(sPcmHandle, sPcmParams, &nSamplesPersec, &dir);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayPcm snd_pcm_hw_params_set_channels errNo=%d,%s.\n",ret,snd_strerror(ret));
		return -1;		
	}
#if 1
	ret = snd_pcm_hw_params_set_period_size_near(sPcmHandle, sPcmParams, &frames, &dir);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayPcm snd_pcm_hw_params_set_period_size_near errNo=%d,%s.\n",ret,snd_strerror(ret));
		snd_pcm_close(sPcmHandle);
		return -1;		
	}
#endif
	
	ret = snd_pcm_hw_params(sPcmHandle, sPcmParams);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_set_buffer_size errNo=%d,%s \n",ret,snd_strerror(ret));
		return -1;
	}

	snd_pcm_hw_params_get_period_size(sPcmParams, &frames, &dir); 
	//BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_get_period_size:frames=%d\n",(int)frames);	


	memset(sVoiceBuffer, 0, VOICE_BUFFER_LEN_MAX);
	for(ret = 0; ret < 10; ret++)
		snd_pcm_writei(sPcmHandle, sVoiceBuffer, frames);
		
	frames = 256;
	readSize = frames * sWavCommHead.wBlockAlign;	//确定每次读取数据的大小
	
	int i;
	char pcmData[1024*10];
	FILE *file = fopen("temp","w+");
	
	while (1)
	{
		//ret = fread(sVoiceBuffer, 1, 4, fp);
		ret = fread(sVoiceBuffer, 1, readSize, fp);
		if(ret == 0)
		{
			printf("voicePlayPcm read data end\n");
			break;
		}
		#if 1
		int bb = 8;
		for(i=0;i<ret;i+=2)
		{			
			int temp1,temp2;
			short temp;
			//temp1 = (unsigned char)sVoiceBuffer[i];
			//temp2 = (unsigned char)sVoiceBuffer[i+1];
			temp1 = ((unsigned char)sVoiceBuffer[i+1] << 8) | (unsigned char)sVoiceBuffer[i];
			//printf("[%d %04X]",temp1,temp1);
			temp1 &= 0x7FFF;
			
			temp1 = temp1*bb;
			printf("[%02X %02X]",(unsigned char)sVoiceBuffer[i],(unsigned char)sVoiceBuffer[i+1]);
			
			if(temp1>65535)
			{
				temp1 = 65535;
			}
			sVoiceBuffer[i] = (temp1 & 0xff);
			printf("{%04X}",temp1);
			if(sVoiceBuffer[i+1]>=0)
			{
				sVoiceBuffer[i+1] = ((temp1>>8) & 0xff);
				
			}
			else
			{
				sVoiceBuffer[i+1] = ((temp1>>8) & 0xff)|0x80;				
			}
			
			#if 1
			printf("%02X %02X ",(unsigned char)sVoiceBuffer[i],(unsigned char)sVoiceBuffer[i+1]);
			if((i+2)%16==0)
			{
				printf("\n");
			}
			usleep(100*1000);
			#endif
		}
		
		#endif
		//volume_control(sVoiceBuffer, pcmData,readSize,1);
		//fwrite(sVoiceBuffer,1,ret,file);
		#if 0
		// 写音频数据到PCM设备 
		while((ret = snd_pcm_writei(sPcmHandle, sVoiceBuffer, frames)) < 0)
		{
			if (ret == -EPIPE)
			{
				BicLogPrintf("voicePlayPcm snd_pcm_writei return epipe\n");
				//完成硬件参数设置，使设备准备好 
				snd_pcm_prepare(sPcmHandle);
			}
			else if (ret < 0)
			{
				BicLogPrintf("voicePlayPcm snd_pcm_writei return error ret=%d\n",ret);
			}
		}
		#endif
	}
	fclose(file);
	return 0;
}

/*********************************************************
    Function:       voicePlayAdpcm
    Description:    播放wav格式中的adpcm格式的语音文件
    Param:          fp     语音文件描述符
    Return:         0:成功
					-1:失败
    Other:          none
*********************************************************/
static int voicePlayAdpcm(FILE *fp)
{
	char inBuffer[1024];
	AdpcmState state;
	int adpcSts = 0;
	int readSize = sWavCommHead.wBlockAlign;
	int frames = ((sWavCommHead.wBlockAlign - 4)*4 + 2)/(sWavCommHead.wChannels*2);

	int nSamplesPersec = sWavCommHead.nSamplesPersec;
	int ret = 0;
	int dir = 0;
	
	//初始化params
	ret = snd_pcm_hw_params_any(sPcmHandle, sPcmParams);
	if(ret < 0)
	{
		  BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_any errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return -1;
	}

	//初始化访问权限
	ret = snd_pcm_hw_params_set_access(sPcmHandle, sPcmParams, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(ret < 0)
	{
		  BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_set_access errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return -1;
	}

	//采样位数
	ret = snd_pcm_hw_params_set_format(sPcmHandle, sPcmParams, SND_PCM_FORMAT_S16_LE);
	if(ret < 0)
	{
		  BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_set_format errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return -1;
	}

	//设置声道,1表示单声>道，2表示立体声
	ret = snd_pcm_hw_params_set_channels(sPcmHandle, sPcmParams, sWavCommHead.wChannels);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_set_channels  errNo=%d,%s.\n",ret,snd_strerror(ret));
		return -1;		
	}

	ret = snd_pcm_hw_params_set_rate_near(sPcmHandle, sPcmParams, &nSamplesPersec, &dir);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_set_channels  errNo=%d,%s.\n",ret,snd_strerror(ret));
		return -1;		
	}

	ret = snd_pcm_hw_params(sPcmHandle, sPcmParams);
	if(ret < 0)
	{
		BicLogPrintf("voicePlayAdpcm snd_pcm_hw_params_set_buffer_size errNo=%d,%s \n",ret,snd_strerror(ret));
		return -1;
	}

	while (1)
	{
		memset(inBuffer, 0, readSize);
		memset(sVoiceBuffer, 0, VOICE_BUFFER_LEN_MAX);
		if((ret = fread(inBuffer, 1, readSize, fp)) <= 0)
		{
			//BicLogPrintf("voicePlayAdpcm read data end\n");
			break;
		}
		if(ret != readSize)
		{
			//BicLogPrintf("voicePlayAdpcm fread ret=%d,readSize=%d\n",ret,readSize);
			continue;
		}
		
		if(adpcSts == 0)
		{
			state.index = 0; 
			adpcSts++;
			for(ret = 0; ret < 10; ret++)
			{
				snd_pcm_writei(sPcmHandle, sVoiceBuffer, frames);
			}
		}
		else
		{
			state.index = inBuffer[2];
		}
		state.valprev = (short)inBuffer[0] + ((short)(inBuffer[1]))*256;  
		sVoiceBuffer[0] = inBuffer[0];
		sVoiceBuffer[1] = inBuffer[1];

		//将adpcm格式转换为pcm格式
		adpcm_decoder(&inBuffer[4], &sVoiceBuffer[2], readSize-4, &state);//(readSize-4)*4 + 2

		// 写音频数据到PCM设备 
		while((ret = snd_pcm_writei(sPcmHandle, sVoiceBuffer, frames)) < 0)
		{
			if (ret == -EPIPE)
			{
				BicLogPrintf("voicePlayAdpcm snd_pcm_writei return epipe\n");
				//完成硬件参数设置，使设备准备好 
				snd_pcm_prepare(sPcmHandle);
			}
			else if (ret < 0)
			{
				BicLogPrintf("voicePlayAdpcm snd_pcm_writei return error ret=%d\n",ret);
			}
		}		
	}
	return 0;
}

int main()
{
	char fileName[256]={0};
	char playName[128]={0};
	char temp[128] = {0};
	int count = 0;
	FILE *fp = NULL;
	int ret = 0;

	//strcpy(playName,"/home/BicCode/0B0011.mp3");
	strcpy(playName,"test.wav");
	//RtcInit();
    //RtcGetTimeToSys();
	
	sPcmHandle = NULL;
	ret = snd_pcm_open(&sPcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(ret < 0)
	{
		  BicLogPrintf("VoicePlayThread snd_pcm_open errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return 0;
	}
	//分配params结构体
	sPcmParams = NULL;
	snd_pcm_hw_params_malloc(&sPcmParams);
	if(ret < 0)
	{
		  BicLogPrintf("VoicePlayThread snd_pcm_hw_params_malloc errNo=%d,%s.\n",ret,snd_strerror(ret));	  
		  snd_pcm_close(sPcmHandle);
		  sPcmHandle = NULL;
		  return 0;
	}
	BicLogPrintf("main start.\n");
	while(1)
	{
		count++;
		BicLogPrintf("play start count=%d.\n",count);
		memset(&sWavCommHead,0,sizeof(sWavCommHead));
		
		fp = fopen(playName,"rb");
		if(fp == NULL)
		{
			BicLogPrintf("VoicePlayThread open file failed. playName=%s\n",playName);
			sleep(1);
			continue;
		}
		//BicLogPrintf("VoicePlayThread open file success. playName=%s\n",playName);
		if(fread(&sWavCommHead,1,WAV_PCM_COMM_HEAD_LEN,fp) != WAV_PCM_COMM_HEAD_LEN)
		{
			BicLogPrintf("VoicePlayThread fread size!=sizeof(sWavCommHead)\n");
			fclose(fp);
			continue;			
		}

		//只需要获取24字节判断是否有data字段
		if(fread(temp,1,24,fp) != 24)
		{
			BicLogPrintf("VoicePlayThread fread size!=24\n");
			fclose(fp);
			continue;
		}
		for(ret = 0; ret < 20; ret++)
		{
			if(memcmp(temp+ret,"data",4) == 0)
			{
				break;
			}
		}
		if(ret >= 20)
		{
			BicLogPrintf("VoicePlayThread not found data\n");
			fclose(fp);
			continue;
		}
		fseek(fp,WAV_PCM_COMM_HEAD_LEN+ret+8,SEEK_SET); //定位歌曲到数据区(8:"data"(4Byte)+dataSize(4Byte))
#if 1
		printf("===============================================================\r\n");
		printf("sizeof(wav_header)=%d\r\n",sizeof(sWavCommHead));
		printf("(4)资源文件标志 rld=%c%c%c%c.\n",sWavCommHead.rld[0],sWavCommHead.rld[1],sWavCommHead.rld[2],sWavCommHead.rld[3]);
		printf("(4)文件长度     rLen=0x%X.\n",sWavCommHead.rLen);
		printf("(4)WAVw文件标志 wld=%c%c%c%c.\n",sWavCommHead.wld[0],sWavCommHead.wld[1],sWavCommHead.wld[2],sWavCommHead.wld[3]);
		printf("(4)波形文件标志 fld=%c%c%c%c.\n",sWavCommHead.fld[0],sWavCommHead.fld[1],sWavCommHead.fld[2],sWavCommHead.fld[3]);    
		printf("(4)块长度       fLen=%d.\n",sWavCommHead.fLen);    
		printf("(2)格式种类     wFormatTag=%d.\n",sWavCommHead.wFormatTag);
		printf("(2)通道数       wChannels=%d.\n",sWavCommHead.wChannels);
		printf("(4)采集频率     nSamplesPersec=%d.\n",sWavCommHead.nSamplesPersec);
		printf("(4)传输速率     nAvgBitsPerSample=%d.\n",sWavCommHead.nAvgBitsPerSample);
		printf("(2)数据块       wBlockAlign=%d.\n",sWavCommHead.wBlockAlign);    
		printf("(2)PCM位宽       wBitsPerSample=%d.\n",sWavCommHead.wBitsPerSample); 
		printf("fseek data       ptr=%d.\n",ret); 
		printf("===============================================================\r\n");
#endif				
		if(WAVE_FORMAT_PCM == sWavCommHead.wFormatTag)
		{
			voicePlayPcm(fp);
		}
		else if(WAVE_FORMAT_DVI_ADPCM == sWavCommHead.wFormatTag)
		{	
			voicePlayAdpcm(fp);
		}
		else
		{	
			BicLogPrintf("VoicePlayThread wFormatTag=0x%02X unknown \n",sWavCommHead.wFormatTag);
		}			
		BicLogPrintf("play end count=%d.\n",count);
		
		fclose(fp);		
		snd_pcm_drain(sPcmHandle);
		break;
	}
	snd_pcm_hw_params_free(sPcmParams);
	sPcmParams = NULL;
	snd_pcm_close(sPcmHandle);
	sPcmHandle = NULL;

	return 0;
}


void BicLogPrintf(const char *fmt,...)
{
#if 0
    FILE    *pLog;
    char    argsPrint[1024];
    va_list args;         //定义一个va_list类型的变量，用来储存单个参数
    
	time(&t_secs);
	t_info = localtime(&t_secs);
 
    sprintf(logProcessFile,"/home/BicCode/playTest_%04d-%02d-%02d.log",t_info->tm_year+1900, t_info->tm_mon+1,t_info->tm_mday);
 
     if((pLog = fopen(logProcessFile, "a+")) != NULL)
    {
		va_start(args, fmt);  //使args指向可变参数的第一个参数
		vsprintf(argsPrint,fmt,args);
		printf("Log voice:%02d:%02d:%02d----%s",t_info->tm_hour, t_info->tm_min, t_info->tm_sec,argsPrint);   //直接传递给printf
		fprintf(pLog,"Date:%d-%02d-%02d %02d:%02d:%02d----%s",t_info->tm_year+1900, t_info->tm_mon+1, t_info->tm_mday,t_info->tm_hour, t_info->tm_min, t_info->tm_sec,argsPrint);
		va_end(args);         //结束可变参数的获取
		fflush(pLog);
        fclose(pLog);
    }
#endif
}
