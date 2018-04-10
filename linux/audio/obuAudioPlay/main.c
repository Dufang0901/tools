#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/timerfd.h>
#include <sys/reboot.h>
#include "VoiceLib.h"
#include <alsa/asoundlib.h>
#include "Mytype.h"
#include <pthread.h>


//目前只考虑一下wav格式的信息
//详细内容请参考http://www.icculus.org/SDL_sound/downloads/external_documentation/wavecomp.htm
#define WAVE_FORMAT_PCM			0x0001
#define WAVE_FORMAT_DVI_ADPCM	0x0011

#define	VOICE_LIB_DEV_WM8962                  1  /*音频设备-WM8962*/
#define VOICE_LIB_DEV_SGTL5000                2  /*支持硬件调节音量*/	

//voiceLib公用资源池
typedef struct
{
	snd_pcm_t 				*pPcmHandle;		//当前线程的声卡驱动句柄
	snd_pcm_hw_params_t 	*pPcmParams;		//当前线程的声卡驱动参数
	
	Uint8 						indexL; 		//左声道,默认不扩音
	Uint8 						indexR; 		//右声道,默认不扩音
	Uint8		        		channelFlag;	//0表示左右声道正常,1表示左右声道交互,2表示立体音	
	Uint8						playStaL;		//播放的状态
	Uint8						playStaR;		//播放的状态
	Char 						playNameL[256];					//播放的文件名
	Char 						playNameR[256];					//播放的文件名

}S_VoiceLibRes;

//adpcm解码状态
typedef struct 
{
	short valprev;	//Previous output value
	Char index;		//Index into stepsize table
}AdpcmState;

//wav公共头结构体定位
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

typedef struct
{
    Char    dataId[4];               //固定为"data"
    Uint32  dataSize;
}S_WaveDataChunk;



//各自线程的变量
typedef struct
{
	//入参
	Char 					*pPlayName;		//播放的文件名
	char                    threadName[24];
	E_VoiceLibChannelType	lr;			    //声道类型(即哪个线程)
	
	//内部参数
	snd_pcm_t 				*pPcmHandle;		//当前线程的声卡驱动句柄
	snd_pcm_hw_params_t 	*pPcmParams;		//当前线程的声卡驱动参数
	WavCommHead 			wavHead;		//当前播放语音文件头
	S_WaveDataChunk 		dataChunk;		//数据头
	FILE 					*fp;		    //当前播放语音文件的描述符

	Uint8 					*LoopFlag;				//当前线程命令
}S_VoiceThreadPar;


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

//硬件调节数组(前10个等级由硬件调节,所以为1)
static float sVolumeHard[VOICE_LIB_MAX_VOL+1] = {1.0, 1.0,    1.0,   1.0,  1.0,  1.0,  1.0,  1.0, 1.0, 1.0,
											     1.5, 2.0,    3.0,   4.0,  5.0,  6.0, 7.0};
//软件调节数组
#if 0
static float sVolumeSoft[VOICE_LIB_MAX_VOL+1] = {0.0, 0.001,  0.004, 0.008,0.01, 0.04, 0.08, 0.1, 0.4, 0.8,
												1.0, 2.0,   3.0,  4.0,   5.0, 6.0, 7.0};
#else
static float sVolumeSoft[VOICE_LIB_MAX_VOL+1] = {0.0, 0.03,  0.06, 0.09,0.1,0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
												0.8, 0.9,  1.0,  1.3,  1.6,2.0};
#endif

static S_VoiceLibRes  gsVoiceLibRes;	//voiceLib公用资源池



//函数定义
void voiceLibAdpcmDec(char *inbuff, char *outbuff, int len_of_in, AdpcmState * state );
void voiceLibPcmDataLR(Char*inBuff,Char *outBuff,int inLen,E_VoiceLibChannelType lr,Uint8 channel);
int voiceLibThreadComm(S_VoiceThreadPar *psThreadPar);
int voiceLibRunRecovery(snd_pcm_t *pcmHandle, int err);  


#if 0
//分隔线
#endif


/*********************************************************
    Function:       voiceLibAdpcmDec
    Description:    将adpcm格式转换为pcm
    Param:          inbuff     adpcm格式数据
    				outbuff    转换后的pcm格式  
    				len_of_in  adpcm数据长度
    				state      上一次转换状态
    Return:         none
    Other:          none
*********************************************************/
void voiceLibAdpcmDec(char *inbuff, char *outbuff, int len_of_in, AdpcmState * state )
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
    Function:       voiceLibPcmDataLR
    Description:    将adpcm格式转换为pcm
    Param:          inbuff     pcm格式数据
    				outBuff    处理后的数据
    				inLen      数据大小
    				lr	  	   要播放声道类型
    				channel    pcm数据的声道数
    Return:         none
    Other:          none
*********************************************************/
void voiceLibPcmDataLR(Char*inBuff,Char *outBuff,int inLen,E_VoiceLibChannelType lr,Uint8 channel)
{
	int i = 0;
	int tempInt;
	short tempShort;
	Uint8 tempVolumeIndex;
	float volumeValue = 1.0;

	if(lr == E_VOICE_LIB_CHANNEL_LEFT)
	{
		tempVolumeIndex = gsVoiceLibRes.indexL;
	}
	else
	{
		tempVolumeIndex = gsVoiceLibRes.indexR;

	}
	
	if(1 == gsVoiceLibRes.channelFlag)
	{
		if(lr == E_VOICE_LIB_CHANNEL_LEFT)
		{
			lr = E_VOICE_LIB_CHANNEL_RIGHT;
			tempVolumeIndex = gsVoiceLibRes.indexR;
		}
		else
		{
			lr = E_VOICE_LIB_CHANNEL_LEFT;
			tempVolumeIndex = gsVoiceLibRes.indexL;;
		}
	}

	/*从音量表中获取音量大小*/
	volumeValue = sVolumeSoft[tempVolumeIndex];
		
	//控制左右声道播放,其实是将所有数据转换为立体声,然后在按照PCM格式将不播放的声道数据分别置为0
	if(channel == 1)
	{
		//单声道处理
		if(2 == gsVoiceLibRes.channelFlag)
		{
			//左右声道不切换,左声道输出
			for(i = 0; i <= inLen - 2; i+=2)
			{
				tempShort = ((unsigned char)inBuff[i+1] << 8) | (unsigned char)inBuff[i];			
				tempInt = tempShort*volumeValue;
				if(tempInt > 32767)
				{
					tempInt = 32767;
				}	
				if(tempInt < -32768)
				{
					tempInt = -32768;
				}					
				outBuff[i*2] = (tempInt & 0xff);
				outBuff[i*2+1] = ((tempInt>>8) & 0xff);
				outBuff[i*2+2] = outBuff[i*2];
				outBuff[i*2+3] = outBuff[i*2+1];
			}	

		}
		else if(lr == E_VOICE_LIB_CHANNEL_LEFT)
		{	
			//左右声道不切换,左声道输出
			for(i = 0; i <= inLen - 2; i+=2)
			{
				tempShort = ((unsigned char)inBuff[i+1] << 8) | (unsigned char)inBuff[i];			
				tempInt = tempShort*volumeValue;
				if(tempInt > 32767)
				{
					tempInt = 32767;
				}	
				if(tempInt < -32768)
				{
					tempInt = -32768;
				}					
				outBuff[i*2] = (tempInt & 0xff);
				outBuff[i*2+1] = ((tempInt>>8) & 0xff);
				outBuff[i*2+2] = 0;
				outBuff[i*2+3] = 0;		
			}	
		}		
		else
		{
			//左右声道切换,右声道输出
			for(i = 0; i <= inLen -2; i+=2)
			{
				tempShort = ((unsigned char)inBuff[i+1] << 8) | (unsigned char)inBuff[i];			
				tempInt = tempShort*volumeValue; 
				if(tempInt > 32767)
				{
					tempInt = 32767;
				}	
				if(tempInt < -32768)
				{
					tempInt = -32768;
				}	
				outBuff[i*2] = 0;
				outBuff[i*2+1] = 0;				
				outBuff[i*2+2] = (tempInt & 0xff);
				outBuff[i*2+3] = ((tempInt>>8) & 0xff);
			}		
		}
	
	}
	else
	{
		//立体声处理
		if(2 == gsVoiceLibRes.channelFlag)
		{
			for(i = 0; i <= inLen - 4; i+=4)
			{
				tempShort = ((unsigned char)inBuff[i+1] << 8) | (unsigned char)inBuff[i];			
				tempInt = tempShort*volumeValue;
				if(tempInt > 32767)
				{
					tempInt = 32767;
				}	
				if(tempInt < -32768)
				{
					tempInt = -32768;
				}	
				outBuff[i] = (tempInt & 0xff);
				outBuff[i+1] = ((tempInt>>8) & 0xff);
				outBuff[i+2] = outBuff[i];
				outBuff[i+3] = outBuff[i+1];
			}	

		}
		if(lr == E_VOICE_LIB_CHANNEL_LEFT)
		{
			//左右声道不切换,左声道输出
			for(i = 0; i <= inLen - 4; i+=4)
			{
				tempShort = ((unsigned char)inBuff[i+1] << 8) | (unsigned char)inBuff[i];			
				tempInt = tempShort*volumeValue;
				if(tempInt > 32767)
				{
					tempInt = 32767;
				}	
				if(tempInt < -32768)
				{
					tempInt = -32768;
				}	
				outBuff[i] = (tempInt & 0xff);
				outBuff[i+1] = ((tempInt>>8) & 0xff);
				outBuff[i+2] = 0;
				outBuff[i+3] = 0;
			}		
		}
		else
		{
			//左右声道不切换,右声道输出
			for(i = 0; i <= inLen - 4; i+=4)
			{
				tempShort = ((unsigned char)inBuff[i+3] << 8) | (unsigned char)inBuff[i+2];			
				tempInt = tempShort*volumeValue;
				if(tempInt > 32767)
				{
					tempInt = 32767;
				}	
				if(tempInt < -32768)
				{
					tempInt = -32768;
				}	
				outBuff[i] = 0;
				outBuff[i+1] = 0;
				outBuff[i+2] = (tempInt & 0xff);
				outBuff[i+3] = ((tempInt>>8) & 0xff);;
			}		
		}
	}
}

/*********************************************************
    Function:       voiceLibRunRecovery
    Description:    writei失败后,恢复
    Param:          pcmHandle  播放音频的设备句柄
		    		err	   错误码
    Return:         none
    Other:          none
*********************************************************/
int voiceLibRunRecovery(snd_pcm_t *pcmHandle, int err)  
{  
    if (err == -EPIPE) 
    {
        err = snd_pcm_prepare(pcmHandle);  
	    if (err < 0) 
	    {
	      printf("Can't recovery from underrun, prepare failed: %s\n",snd_strerror(err));  	    
	      return 0;  
	    }
    } 
	else if (err == -ESTRPIPE) 
	{   
		while ((err = snd_pcm_resume(pcmHandle)) == -EAGAIN)  
		{
		 	usleep(10000);
		}
		if (err < 0) 
		{  
		    err = snd_pcm_prepare(pcmHandle);  
		    if (err < 0)  
		    {
		    	printf("Can't recovery from suspend, prepare failed: %s\n",snd_strerror(err));  				
		    }
			
		}  
		return 0;  
   }  
   return err;  
}  

/*********************************************************
    Function:       voiceLibPcm
    Description:    播放wav格式中的pcm格式的语音文件
    Param:          psThreadPar  线程参数
    Return:         none
    Other:          none
*********************************************************/
static void voiceLibPcm(S_VoiceThreadPar *psThreadPar)
{
	int ret = 0;
	int dir = 0;
	int readSize ;
	int pcmSize = 0;	
	Uint32 readSizeTotal = 0;
	unsigned int nSamplesPersec = psThreadPar->wavHead.nSamplesPersec;
	AdpcmState state;
	int adpcSts = 0;

	Char outBuffer[1024*4];	//adpcm转pcm后的数据
	Char readBuf[1024];		//从文件读取的数据
	Char pcmBuffer[1024*8];	//存放要播放的数据
	
	snd_pcm_uframes_t frames = 64;
	
	//1初始化params
	ret = snd_pcm_hw_params_any(psThreadPar->pPcmHandle, psThreadPar->pPcmParams);
	if(ret < 0)
	{
		  printf("snd_pcm_hw_params_any errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return ;
	}
	
	//2初始化访问权限
	ret = snd_pcm_hw_params_set_access(psThreadPar->pPcmHandle, psThreadPar->pPcmParams, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(ret < 0)
	{
		  printf("snd_pcm_hw_params_set_access errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return ;
	}

	//3设置采样位数
	ret = snd_pcm_hw_params_set_format(psThreadPar->pPcmHandle, psThreadPar->pPcmParams, SND_PCM_FORMAT_S16_LE);
	if(ret < 0)
	{
		  printf("snd_pcm_hw_params_set_format errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return ;
	}

	//4设置声道,1表示单声>道，2表示立体声
	ret = snd_pcm_hw_params_set_channels(psThreadPar->pPcmHandle, psThreadPar->pPcmParams, 2);
	if(ret < 0)
	{
		printf("snd_pcm_hw_params_set_channels errNo=%d,%s.\n",ret,snd_strerror(ret));
		return ;		
	}

	//5设置采样率
	ret = snd_pcm_hw_params_set_rate_near(psThreadPar->pPcmHandle, psThreadPar->pPcmParams, &nSamplesPersec, &dir);
	if(ret < 0)
	{
		printf("snd_pcm_hw_params_set_rate_near errNo=%d,%s.\n",ret,snd_strerror(ret));
		return ;		
	}

	//6设置硬件参数
	ret = snd_pcm_hw_params(psThreadPar->pPcmHandle, psThreadPar->pPcmParams);
	if(ret < 0)
	{
		printf("snd_pcm_hw_params errNo=%d,%s \n",ret,snd_strerror(ret));
		return ;
	}
	printf("@@@@@@@@@@@@ hardware parameter set successful.\n");
	if(WAVE_FORMAT_PCM == psThreadPar->wavHead.wFormatTag)
	{
		readSize = frames * psThreadPar->wavHead.wBlockAlign;	//确定每次读取数据的大小
		frames = 64;
	}
	else if(WAVE_FORMAT_DVI_ADPCM == psThreadPar->wavHead.wFormatTag)
	{	
		readSize = psThreadPar->wavHead.wBlockAlign;
		frames = ((psThreadPar->wavHead.wBlockAlign - 4)*4 + 2)/(psThreadPar->wavHead.wChannels*2);
		pcmSize = (psThreadPar->wavHead.wBlockAlign - 4)*4 + 2;
	}
	else
	{	
		printf("voiceLib wFormatTag=0x%02X unknown \n",psThreadPar->wavHead.wFormatTag);
		return ;
	}		

	
	while (readSizeTotal < psThreadPar->dataChunk.dataSize)
	{	
		if((ret = fread(readBuf, 1, readSize, psThreadPar->fp)) <= 0)
		{
			break;
		}
		if(ret != readSize)
        {
            /*后面的全部置0*/
            memset(&readBuf[ret],0,readSize - ret);
        }
		readSizeTotal += readSize;

		if(WAVE_FORMAT_DVI_ADPCM == psThreadPar->wavHead.wFormatTag)
		{
			//将adpcm格式转换为pcm格式
			if(adpcSts == 0)
			{
				state.index = 0; 
				adpcSts++;
			}
			else
			{
				state.index = readBuf[2];
			}
			state.valprev = (short)readBuf[0] + ((short)(readBuf[1]))*256;	
			outBuffer[0] = readBuf[0];
			outBuffer[1] = readBuf[1];
						
			voiceLibAdpcmDec(&readBuf[4], &outBuffer[2], readSize-4, &state);//(readSize-4)*4 + 2
			voiceLibPcmDataLR(outBuffer,pcmBuffer,pcmSize,psThreadPar->lr,psThreadPar->wavHead.wChannels);
		}				
		else
		{			
			voiceLibPcmDataLR(readBuf,pcmBuffer,ret,psThreadPar->lr,psThreadPar->wavHead.wChannels);
		}
		
		// 写音频数据到PCM设备 
		while((ret = snd_pcm_writei(psThreadPar->pPcmHandle, pcmBuffer, frames)) < 0)
		{
			ret = voiceLibRunRecovery(psThreadPar->pPcmHandle, ret);
	        if (ret < 0) 
			{  
		        printf("snd_pcm_writei error: %s\n", snd_strerror(ret));  
		    	return ;  
		    } 			
		}		
		
	}
	
	printf("@@@@@@@@@@@@ %s:%d write data finished,begin play...\n",__func__,__LINE__);

#if 0
	snd_pcm_drain(gsVoiceLibRes.pPcmHandle);
#else
	while(1)
	{
		printf("@@@@@@@@@@@@ %s:%d ret = %d\n",__func__,__LINE__,ret);
		if(((ret = snd_pcm_avail(psThreadPar->pPcmHandle)) < 0))
		{
			return ;
		}
		usleep(1000);
	}
	//printf("@@@@@@@@@@@@ %s:%d\n",__func__,__LINE__);
#endif

}

/*********************************************************
    Function:       voiceLibThreadComm
    Description:    左右线程公用函数
    Param:          psThreadPar	   线程参数		
    Return:         0-成功,1-失败
    Other:          none
*********************************************************/
int voiceLibThreadComm(S_VoiceThreadPar *psThreadPar)
{
	Char tempPlayName[256] = {0};
	Char temp[256] = {0};
	int ret = 0;
	
	
#if 0
	psThreadPar->pPcmHandle = NULL;

	/*1 打开音频驱动*/
	ret = snd_pcm_open(&psThreadPar->pPcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_open errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return 1;
	}
	/*2 分配params结构体*/
	psThreadPar->pPcmParams = NULL;
	snd_pcm_hw_params_malloc(&psThreadPar->pPcmParams);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_hw_params_malloc errNo=%d,%s.\n",ret,snd_strerror(ret));	  
		  snd_pcm_close(psThreadPar->pPcmHandle);
		  psThreadPar->pPcmHandle = NULL;
		  return 1;
	}
#endif
	while(1)
	{
			strcpy(tempPlayName,psThreadPar->pPlayName);
			psThreadPar->fp = fopen(tempPlayName,"rb");
			if(psThreadPar->fp == NULL)
			{
				printf("voiceLib open file failed. tempPlayName=%s\n",tempPlayName);
				continue;
			}
			printf("voiceLib open file success. tempPlayName=%s\n",tempPlayName);
			
			if(fread(&psThreadPar->wavHead,1,sizeof(WavCommHead),psThreadPar->fp) != sizeof(WavCommHead))
			{
				printf("voiceLib fread size!=sizeof(sWavCommHead)\n");
				fclose(psThreadPar->fp);
				continue;			
			}

			//只需要获取256字节判断是否有data字段
			if(fread(temp,1,256,psThreadPar->fp) != 256)
			{
				printf("voiceLib fread size!=256\n");
				fclose(psThreadPar->fp);
				continue;
			}
			for(ret = 0; ret < 252; ret++)
			{
				if(memcmp(temp+ret,"data",4) == 0)
				{
					break;
				}
			}
			if(ret >= 252)
			{
				printf("voiceLib not found data\n");
				fclose(psThreadPar->fp);
				continue;
			}
			fseek(psThreadPar->fp,sizeof(WavCommHead) + ret,SEEK_SET); //定位歌曲到数据区(8:"data"(4Byte)+dataSize(4Byte))

			//data之后,4个字节表示后面数据区域大小
			if(fread(&psThreadPar->dataChunk,1,sizeof(S_WaveDataChunk),psThreadPar->fp) != sizeof(S_WaveDataChunk))
			{
				printf("voiceLib fread size!=sizeof(wavDataChunk)\n");
				fclose(psThreadPar->fp);
				continue;			
			}
#if 1
			printf("===============================================================\r\n");
			printf("voiceLib sizeof(wav_header)=%d\r\n",sizeof(WavCommHead));
		    printf("voiceLib (4)资源文件标志 rld=%c%c%c%c.\n",psThreadPar->wavHead.rld[0],psThreadPar->wavHead.rld[1],psThreadPar->wavHead.rld[2],psThreadPar->wavHead.rld[3]);
		    printf("voiceLib (4)文件长度     rLen=0x%X.\n",psThreadPar->wavHead.rLen);
		    printf("voiceLib (4)WAVw文件标志 wld=%c%c%c%c.\n",psThreadPar->wavHead.wld[0],psThreadPar->wavHead.wld[1],psThreadPar->wavHead.wld[2],psThreadPar->wavHead.wld[3]);
		    printf("voiceLib (4)波形文件标志 fld=%c%c%c%c.\n",psThreadPar->wavHead.fld[0],psThreadPar->wavHead.fld[1],psThreadPar->wavHead.fld[2],psThreadPar->wavHead.fld[3]);    
		    printf("voiceLib (4)块长度       fLen=%d.\n",psThreadPar->wavHead.fLen);    
		    printf("voiceLib (2)格式种类     wFormatTag=%d.\n",psThreadPar->wavHead.wFormatTag);
		    printf("voiceLib (2)通道数       wChannels=%d.\n",psThreadPar->wavHead.wChannels);
		    printf("voiceLib (4)采集频率     nSamplesPersec=%d.\n",psThreadPar->wavHead.nSamplesPersec);
		    printf("voiceLib (4)传输速率     nAvgBitsPerSample=%d.\n",psThreadPar->wavHead.nAvgBitsPerSample);
		    printf("voiceLib (2)数据块       wBlockAlign=%d.\n",psThreadPar->wavHead.wBlockAlign);    
		    printf("voiceLib (2)PCM位宽       wBitsPerSample=%d.\n",psThreadPar->wavHead.wBitsPerSample); 
			printf("voiceLib fseek data       ptr=%d.\n",ret); 
			printf("voiceLib data数据大小     dataSize=%d.\n",psThreadPar->dataChunk.dataSize); 
			printf("===============================================================\r\n");
#endif		

#if 0
                ret = snd_pcm_open(&psThreadPar->pPcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
                if(ret < 0)
                {
                      printf("voiceLib snd_pcm_open errNo=%d,%s.\n",ret,snd_strerror(ret));
                      return 1;
                }
                //分配params结构体
                psThreadPar->pPcmParams = NULL;
                snd_pcm_hw_params_malloc(&psThreadPar->pPcmParams);
                if(ret < 0)
                {
                      printf("voiceLib snd_pcm_hw_params_malloc errNo=%d,%s.\n",ret,snd_strerror(ret));   
                      snd_pcm_close(psThreadPar->pPcmHandle);
                      psThreadPar->pPcmHandle = NULL;
                      return 1;
                }
#endif

			voiceLibPcm(psThreadPar);
			
			printf("voiceLib %s play end.\n",tempPlayName);
			
			fclose(psThreadPar->fp);
#if 0
        	snd_pcm_hw_params_free(psThreadPar->pPcmParams);
        	psThreadPar->pPcmParams = NULL;
        	snd_pcm_close(psThreadPar->pPcmHandle);
        	psThreadPar->pPcmHandle = NULL;
#endif

			sleep(5);
			
		}
#if 1
	snd_pcm_hw_params_free(psThreadPar->pPcmParams);
	psThreadPar->pPcmParams = NULL;
	snd_pcm_close(psThreadPar->pPcmHandle);
	psThreadPar->pPcmHandle = NULL;
#endif
    return 0;

}


/*********************************************************
    Function:       voiceLibLeftThread
    Description:    右声道语音播放的线程
    Param:          arg		
    Return:         none
    Other:          none
*********************************************************/
static void *voiceLibLeftThread(void *arg)
{
	(void)arg;
	S_VoiceThreadPar sVoiceThreadPar;

	sVoiceThreadPar.pPlayName= gsVoiceLibRes.playNameL;
	strcpy(sVoiceThreadPar.threadName,"left");
	
	sVoiceThreadPar.lr = E_VOICE_LIB_CHANNEL_LEFT;
	//sVoiceThreadPar.pPcmHandle = gsVoiceLibRes.pPcmHandle;
	//sVoiceThreadPar.pPcmParams = gsVoiceLibRes.pPcmParams;

#if 1
	int ret;
	ret = snd_pcm_open(&sVoiceThreadPar.pPcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_open errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return 0;
	}
	//分配params结构体
	sVoiceThreadPar.pPcmParams = NULL;
	snd_pcm_hw_params_malloc(&sVoiceThreadPar.pPcmParams);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_hw_params_malloc errNo=%d,%s.\n",ret,snd_strerror(ret));   
		  snd_pcm_close(sVoiceThreadPar.pPcmHandle);
		  sVoiceThreadPar.pPcmHandle = NULL;
		  return 0;
	}
#endif


	printf("voiceLibLeftThread start.\n");

	voiceLibThreadComm(&sVoiceThreadPar);

	printf("voiceLibLeftThread exit.\n");

	pthread_exit(NULL);    
    return NULL;
}

/*********************************************************
    Function:       voiceLibRightThread
    Description:    右声道语音播放的线程
    Param:          arg		
    Return:         none
    Other:          none
*********************************************************/
static void *voiceLibRightThread(void *arg)
{
	(void)arg;
	S_VoiceThreadPar sVoiceThreadPar;

	sVoiceThreadPar.pPlayName= gsVoiceLibRes.playNameR;
	strcpy(sVoiceThreadPar.threadName,"right");
	
	sVoiceThreadPar.lr = E_VOICE_LIB_CHANNEL_RIGHT;

	//sVoiceThreadPar.pPcmHandle = gsVoiceLibRes.pPcmHandle;
	//sVoiceThreadPar.pPcmParams = gsVoiceLibRes.pPcmParams;

	printf("@@@@@@@@@@@@@ %s:%d,pid is %d\n",__func__,__LINE__,getpid());
	
#if 1
	int ret;
	ret = snd_pcm_open(&sVoiceThreadPar.pPcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_open errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return 0;
	}
	//分配params结构体
	sVoiceThreadPar.pPcmParams = NULL;
	snd_pcm_hw_params_malloc(&sVoiceThreadPar.pPcmParams);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_hw_params_malloc errNo=%d,%s.\n",ret,snd_strerror(ret));   
		  snd_pcm_close(sVoiceThreadPar.pPcmHandle);
		  sVoiceThreadPar.pPcmHandle = NULL;
		  return 0;
	}
#endif

	printf("voiceLibRightThread start.\n");

	voiceLibThreadComm(&sVoiceThreadPar);

	printf("voiceLibRightThread exit.\n");

	pthread_exit(NULL);    
    return NULL;	
}


/*********************************************************
    Function:       VoiceLibOpen
    Description:    打开语音库
    Param:          pVoiceLibOpenPar
    Return:         1-成功,0-失败
    Other:          none
*********************************************************/
int VoiceLibOpen(void)
{
	pthread_t playLThreadId;
	pthread_t playRThreadId;

	//初始化默认参数
	
	//只能通过软件调节音量
	gsVoiceLibRes.indexR = 10;			//默认不扩音
	gsVoiceLibRes.indexL = 10;			
	gsVoiceLibRes.channelFlag = 0;

	strcpy(gsVoiceLibRes.playNameL,"./1.mp3");
	strcpy(gsVoiceLibRes.playNameR,"./2.mp3");

#if 0
	int ret;
	gsVoiceLibRes.pPcmHandle = NULL;

	ret = snd_pcm_open(&gsVoiceLibRes.pPcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_open errNo=%d,%s.\n",ret,snd_strerror(ret));
		  return 1;
	}
	//分配params结构体
	gsVoiceLibRes.pPcmParams = NULL;
	ret = snd_pcm_hw_params_malloc(&gsVoiceLibRes.pPcmParams);
	if(ret < 0)
	{
		  printf("voiceLib snd_pcm_hw_params_malloc errNo=%d,%s.\n",ret,snd_strerror(ret));	  
		  snd_pcm_close(gsVoiceLibRes.pPcmHandle);
		  gsVoiceLibRes.pPcmHandle = NULL;
		  return 1;
	}
#endif
		
	//区分左右声道,则需要创建左右播放的声道
#if 1
	if(pthread_create(&playLThreadId,NULL,voiceLibLeftThread,NULL) == -1)
	{
		printf("voiceLib voiceLibLeftThread failed.\n");			
		return 0;
	}
#endif
#if 1
	if(pthread_create(&playRThreadId,NULL,voiceLibRightThread,NULL) == -1)
	{
		printf("voiceLib voiceLibRightThread failed.\n");			
		return 0;
	}		
#endif
	
	return 1;
}


/*********************************************************
    Function:       VoicePlayThread
    Description:    播放语音的线程
    Param:          arg		
    Return:         none
    Other:          none
*********************************************************/
int main()
{	
	if(0 == VoiceLibOpen())
	{
        printf("VoiceProcess voiceLib open failed,exit.\n");	
        exit(-1);	
	}
	while(1)
	{
		sleep(10);
	}
	return 1;
}




