#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <unistd.h>


typedef enum
{
   	E_VOICE_LIB_VOLUME_L = 0,    //��������������     
   	E_VOICE_LIB_VOLUME_R = 1,    //��������������
}E_VoiceLibVolumeType;


int GetNextCode(char curCode, int n);
void OutPutSample(char *ptr, int sample);
int adpcm_to_pcm(char *inBuf,char *outBuf,int size);
int set_pcm_play(FILE *fp,FILE *fp_bak);
int VoiceLibVolumeSet(E_VoiceLibVolumeType type,long value);

#define VOICE_LIB_MAX_H_VOL		9	//��������������ڵȼ�[0-9]
#define VOICE_LIB_MAX_S_VOL		7	//�������������ڵȼ�[0-7]
#define VOICE_LIB_MAX_VOL		(VOICE_LIB_MAX_H_VOL + VOICE_LIB_MAX_S_VOL)	//���������ڵȼ�[0-16]

struct adpcm_state
{
short valprev; /* Previous output value */
char index; /* Index into stepsize table */
};
/* Intel ADPCM step variation table */
#if 0
static int indexTable[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};
#endif
static int indexTable[8] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
};

static int stepsizeTable[89] = {
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


/*���ڱ���PCM�ļ���ͷ����Ϣ*/
struct PCM_HEADER
{
    char rld[4]; 					//riff ��־����
    unsigned int rLen; 
    char wld[4]; 					//��ʽ���ͣ�wave��
    char fld[4]; 					//"fmt"
    unsigned int fLen; 				//sizeof(wave format matex)
    short wFormatTag; 				//�����ʽ
    short wChannels; 				//������
    unsigned int nSamplesPersec ; 	//����Ƶ��
    unsigned int nAvgBitsPerSample;	//WAVE�ļ�������С
    short wBlockAlign; 				//�����    
	short wBitsPerSample; 			//WAVE�ļ�������С
	char dld[4]; 					//��data��
    unsigned int wSampleLength; 	//��Ƶ���ݵĴ�С

} pcm_header;


/*���ڱ���ADPCM�ļ���ͷ����Ϣ*/
struct ADPCM_HEADER
{
    char rld[4]; //riff ��־����
    unsigned int rLen; 
    char wld[4]; //��ʽ���ͣ�wave��
    char fld[4]; //"fmt"
    unsigned int fLen; //sizeof(wave format matex)
    short wFormatTag; //�����ʽ
    short wChannels; //������
    unsigned int nSamplesPersec ; //����Ƶ��
    unsigned int nAvgBitsPerSample;//WAVE�ļ�������С
    short wBlockAlign; //�����
    
	short wBitsPerSample; //WAVE�ļ�������С
    short wReserve;
	short wBlockNum;
    char dld[4]; //��data��
    unsigned int wSampleLength; //��Ƶ���ݵĴ�С

} adpcm_header;

/*���ڱ���ADPCM�ļ�ÿ�����ݿ��4�ֽ�ͷ����Ϣ*/
typedef struct
{
	short sample0;
	char index;
	char reserved;
}S_BlockHeader;

int main(int argc,char *argv[])
{

	int nread;
    FILE *fp,*fp_bak = NULL;
    //fp=fopen("./audio/1.wav","rb");
	//fp=fopen("./audio/k11.mp3","rb");
	fp=fopen(argv[1],"rb");
	
    if(fp==NULL)
    {
        perror("open file failed:\n");
        exit(1);
    }
	
    nread=fread(&adpcm_header,1,sizeof(adpcm_header),fp);

	
#if 0
	memcpy(&pcm_header,&adpcm_header,sizeof(pcm_header));
	memcpy(pcm_header.dld,adpcm_header.dld,4);
	pcm_header.wSampleLength = adpcm_header.wSampleLength;
	pcm_header.wFormatTag = 0x0001;
	pcm_header.wBitsPerSample = 16;
#endif	
	
	
	printf("sizeof(adpcm_header)=%d\r\n",sizeof(adpcm_header));
    printf("(4)��Դ�ļ���־ rld=%c%c%c%c.\n",adpcm_header.rld[0],adpcm_header.rld[1],adpcm_header.rld[2],adpcm_header.rld[3]);
    printf("(4)�ļ�����     rLen=0x%X.\n",adpcm_header.rLen);
    printf("(4)WAVw�ļ���־ wld=%c%c%c%c.\n",adpcm_header.wld[0],adpcm_header.wld[1],adpcm_header.wld[2],adpcm_header.wld[3]);
    printf("(4)�����ļ���־ fld=%c%c%c%c.\n",adpcm_header.fld[0],adpcm_header.fld[1],adpcm_header.fld[2],adpcm_header.fld[3]);    
    printf("(4)�鳤��       fLen=%d.\n",adpcm_header.fLen);    
    printf("(2)��ʽ����     wFormatTag=%d.\n",adpcm_header.wFormatTag);
    printf("(2)ͨ����       wChannels=%d.\n",adpcm_header.wChannels);
    printf("(4)�ɼ�Ƶ��     nSamplesPersec=%d.\n",adpcm_header.nSamplesPersec);
    printf("(4)��������     nAvgBitsPerSample=%d.\n",adpcm_header.nAvgBitsPerSample);
    printf("(2)���ݿ�       wBlockAlign=%d.\n",adpcm_header.wBlockAlign);    
    printf("(2)PCMλ��       wBitsPerSample=%d.\n",adpcm_header.wBitsPerSample);    
	//printf("(2)ÿ��PCM����  wBlockNum=%d.\n",adpcm_header.wBlockNum);    
	printf("(4)���ݱ�־��   dld=%c%c%c%c.\n",adpcm_header.dld[0],adpcm_header.dld[1],adpcm_header.dld[2],adpcm_header.dld[3]);
    printf("(4)���ݳ���     wSampleLength=%d.\n",adpcm_header.wSampleLength);
    //return 1;
    
    //��������
	VoiceLibVolumeSet(E_VOICE_LIB_VOLUME_L,3);
	VoiceLibVolumeSet(E_VOICE_LIB_VOLUME_R,3);
    set_pcm_play(fp,fp_bak);
    return 0;
}

/***********************************************************************************
Function:		adpcm_decoder
Description:	����ADPCM����
Arguements:		inbuff		��������
				outbuff		�������
Return:			none
Other:
************************************************************************************/
void adpcm_decoder(char *inbuff,char *outbuff,int len_of_in,struct adpcm_state * state )
{
 int  i=0,j=0;
 char tmp_data;
 signed long diff; 			/* Difference between sample and predicted sample */
 long step; 				/* Quantizer step size */
 signed long predsample; 	/* Output of ADPCM predictor */
 signed long diffq;			/* Dequantized predicted difference */
 int index; 				/* Index into step size table */
 int indexa,indexb;
 int Samp;
 unsigned char SampH, SampL;
 unsigned char inCode; 
 
 /* Restore previous values of predicted sample and quantizer step size index */
 predsample = state->valprev;
 index = state->index;
 
 for(i=0;i<len_of_in*2;i++)
{
   tmp_data=inbuff[i/2];  
   if(i%2)
    inCode=(tmp_data& 0xf0) >> 4;
   else
    inCode=tmp_data & 0x0f;
   {
    
    step = stepsizeTable[index];
    /* Inverse quantize the ADPCM code into a predicted differenceusing the quantizer step size */
    
    diffq = step >> 3;
    if( inCode & 4 )
    	diffq += step;
    if( inCode & 2 )
    	diffq += step >> 1;
    if( inCode & 1 )
    	diffq += step >> 2;
	
    /* Fixed predictor computes new predicted sample by adding the old predicted sample to predicted difference */
    if( inCode & 8 )
    	predsample -= diffq;
    else
    	predsample += diffq;
	
    /* Check for overflow of the new predicted sample*/
    if( predsample > 32767 )
    	predsample = 32767;
    else if( predsample < -32768 )
    	predsample = -32768;
	
    /* Find new quantizer stepsize index by adding the old index to a table lookup using the ADPCM code*/
    index += indexTable[inCode];
	
    /* Check for overflow of the new quantizer step size index */
    if( index < 0 )
    	index = 0;
    if( index > 88 )
    	index = 88;
	
    /* Return the new ADPCM code */
    Samp=predsample;
    
   
   }
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
 
 /* Save the predicted sample and quantizer step size index for
 next iteration
 */
 state->valprev  = predsample;
 state->index  = index;
 //printf("predsample=%d,index=%d,j=%d\r\n",predsample,index,j);
}

/***********************************************************************************
Function:		set_pcm_play
Description:	����ADPCM�ļ�
Arguements:		fp		��Ҫ���ŵ��ļ�ָ��
Return:			
Other:
************************************************************************************/
int set_pcm_play(FILE *fp,FILE *fp_bak)
{
		snd_pcm_t* handle; //PCI�豸���
		snd_pcm_hw_params_t* params;//Ӳ����Ϣ��PCM������
		snd_pcm_uframes_t frames;
		int rc;
        int ret;
        //int size;
        
        //unsigned int val;
        int dir=0;
        
        char *buffer;
		char *outbuffer;
		//struct adpcm_state state;
        int channels=adpcm_header.wChannels;
        int frequency=adpcm_header.nSamplesPersec;
        //int bit=adpcm_header.wBitsPerSample;
        int datablock=adpcm_header.wBlockAlign;
		int count;
		
    	/*Ϊbuffer��outbuffer����ռ�*/
    	buffer =(char *)malloc(datablock);
		outbuffer = (char *)malloc(datablock * 4);

		/*1.��PCM�豸*/
        rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if(rc<0)
        {
            perror("open PCM device failed.\n");
            exit(1);
        }

		/*2.����params�ṹ��*/
        snd_pcm_hw_params_alloca(&params); 

		/*3.��ʼ��params*/
        rc = snd_pcm_hw_params_any(handle, params);
        if(rc<0)
        {
            perror("snd_pcm_hw_params_any.\n");
            exit(1);
        }

		/*4.��ʼ�����ʷ�ʽ*/
        rc = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); 
        if(rc<0)
        {
            perror("sed_pcm_hw_set_access.\n");
            exit(1);

        }

		/*5.���ø�ʽ*/
		snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
		
		/*6.��������,1��ʾ��������2��ʾ������*/
        rc = snd_pcm_hw_params_set_channels(handle, params, channels); 
        if(rc<0)
        {
            perror("snd_pcm_hw_params_set_channels.\n");
            exit(1);
        }

	  	/*6.����Ƶ��*/
        rc = snd_pcm_hw_params_set_rate_near(handle, params, &frequency, &dir); 
        if(rc<0)
        {
            perror("snd_pcm_hw_params_set_rate_near.\n");
            exit(1);
        }

		
#if 0
		/*7.����ÿ�����ڵ�֡��*/
		frames = 1024;

        rc = snd_pcm_hw_params_set_period_size(handle, params, frames, dir); 
        if(rc<0)
        {
            perror("snd_pcm_hw_params_set_period_size.\n");
           // exit(1);
        }


		/*8.���û������Ĵ�С,��֡Ϊ��λ*/
		
		ret = snd_pcm_hw_params_set_buffer_size(handle, params, ((adpcm_header.wBlockAlign - 4)*4 + 2));
		ret = 0;
		if(ret < 0)
		{
			printf("voicePlayAdpcm snd_pcm_hw_params_set_buffer_size errNo=%d,%s \n",ret,snd_strerror(ret));
			return ;
		}
		
		//frames = 1024 *32;
		//snd_pcm_hw_params_set_buffer_size_near (handle, params, &frames);




		/*���û�����������������*/
		int periods = 24;
		if ( snd_pcm_hw_params_set_periods_near(handle, params, &periods, &dir) < 0) 
		{
			fprintf(stderr, "Error setting periods. \n");
			return(-1);
		}

#endif

		/*8.������д���豸*/
        rc = snd_pcm_hw_params(handle, params);
        if(rc<0)
        {
	        perror("snd_pcm_hw_params.\n");
	        exit(1);
        }

		/*���ú��ȡ���ڳ���*/
        rc = snd_pcm_hw_params_get_period_size(params, &frames, &dir); 
		printf("after setting,snd_pcm_hw_params_get_period_size:frames=%d\n",(int)frames);	

		/***********************************************************************************/
		unsigned int val;
		snd_pcm_hw_params_get_period_time(params,&val, &dir);  
		printf("period time = %d us\n", val);  
		  
		snd_pcm_hw_params_get_period_size(params,&frames, &dir);  
		printf("period size = %d frames\n", (int)frames);  
		  
		snd_pcm_hw_params_get_buffer_time(params,&val, &dir);  
		printf("buffer time = %d us\n", val);  
		  
		snd_pcm_hw_params_get_buffer_size(params,(snd_pcm_uframes_t *) &val);  
		printf("buffer size = %d frames\n", val);  
		  
		snd_pcm_hw_params_get_periods(params, &val, &dir);  
		printf("periods per buffer = %d frames\n", val);  
		/***********************************************************************************/

		/*��λ������������*/
		fseek(fp,sizeof(adpcm_header),SEEK_SET); 

		/*��дһ���ֿ����ݽ�ȥ*/
		int i;
		char null[2 * adpcm_header.wChannels * frames];
		memset(null,0,sizeof(null));
		for(i=0; i < 30; i++)
		{
			while(ret = snd_pcm_writei(handle, null, frames)<0)
		   {
				 usleep(2000); 
				 if (ret == -EPIPE)		 /* EPIPE means underrun */
				{
					  printf("UnderRun occurred.\n");
					  
					  /*���Ӳ���������ã�ʹ�豸׼����*/
					  snd_pcm_prepare(handle);
				 }
				 else if (ret < 0)
				 {
					printf("snd_pcm_writei error.\n");
				 }
			}
		}

		printf("after null data in input.\n");
		/*��д��Ҫ���ŵ��ļ�����*/
		while (1)
        {
			memset(buffer, 0, datablock);
			memset(outbuffer, 0, datablock * 4);
			ret = fread(buffer, 1, datablock, fp);
			
			if(ret == 0)
			{
				printf("Audio file reading end.\n");
				break;
			}
			if(ret != datablock)
			{
				printf("ret=%d \r\n",ret);
			}
			//adpcm_decoder(buffer,outbuffer,ret,&state);
			count = adpcm_to_pcm(buffer,outbuffer,datablock);
			//fwrite(outbuffer,1,count*2,fp_bak);
			
			while(ret = snd_pcm_writei(handle, outbuffer, count)<0)
		   {
				 usleep(2000); 
				 if (ret == -EPIPE)		 /* EPIPE means underrun */
				{
					  printf("UnderRun occurred.\n");
					  
					  /*���Ӳ���������ã�ʹ�豸׼����*/
					  snd_pcm_prepare(handle);
				 }
				 else if (ret < 0)
				 {
					printf("snd_pcm_writei error.\n");
				 }
			}
			
		}
 
		printf("Audio file writing end.\r\n");
		printf("Playing audio...\n");
        snd_pcm_drain(handle);
           
        snd_pcm_close(handle);
		printf("Audio play end.\r\n");
        free(buffer);
		free(outbuffer);
		fclose(fp);
		//fclose(fp_bak);
        return 0;

}



/***********************************************************************************
Function:		GetNextCode
Description:	��ȡ������ADPCM����֡��codeֵ�����ڽ���
Arguements:		curCode		codeֵ���ڵ��ֽ�
				n			�����ж�codeֵ���ڸ�4λ���ǵ�4λ
Return:			codeֵ
Other:
************************************************************************************/
int GetNextCode(char curCode, int n)
{
	if((n % 2) == 0)
		return (int)(curCode & 0x0f);
	else
		return (int)((curCode & 0xf0)>>4);
}

/***********************************************************************************
Function:		OutPutSample
Description:	��������PCM���ݴ��������������
Arguements:		ptr		���������ָ��
				sample	������PCM���ݣ���2���ֽڣ�16λ
Return:			none
Other:
************************************************************************************/
void OutPutSample(char *ptr, int sample)
{
	int sampleH;	//�߰�λ����
	int sampleL;	//�ڰ�λ����
	sampleH = sampleL = 0;
	
	if (sample >= 0)
   {
	    sampleH=sample / 256;
	    sampleL = sample - 256 * sampleH;
   }
   else
   {
	    sample= 32768+sample;
	    sampleH = sample / 256;
	    sampleL = sample - 256 * sampleH;
	    sampleH += 0x80;
   }
   *ptr = sampleL;
   ptr++;
   *ptr = sampleH;
}

/***********************************************************************************
Function:		adpcm_to_pcm
Description:	��ADPCM��ʽ�����ݽ���ΪPCM��ʽ������
Arguements:		inBuf	��������ָ�룬����Ϊ�ļ�һ��block�ĳ��ȣ���256
				outBuf	������ݴ��ָ�룬����һ��Ϊ����ָ���4��
Return:			�������ݵ�֡��
Other:
************************************************************************************/
int adpcm_to_pcm(char *inBuf,char *outBuf,int size)
{
	char *data = NULL;
	S_BlockHeader *blockHeader = (S_BlockHeader *)inBuf;
	//char curOutBuf[2];
	char *outBufPtr = NULL;
	int sampleCount, i, j, fg;
	int index;
	int code, diff, curSample;
	char *curCode = NULL;
	int outPutFrame = 0;	//���֡��
	
	sampleCount = size-4;
	
	data = (char *)(blockHeader + 1);
	outBufPtr = outBuf;
	
	/*��ȡ��һ������*/
	memcpy(outBufPtr, &(blockHeader->sample0), 2);
	outBufPtr += 2;
	outPutFrame ++;
	
	curCode = data;

	/*��һ�ν�ѹ����ʱ��curSampleΪ��block�е�һ��sample*/
	curSample = blockHeader->sample0;
	index = (int)blockHeader->index;

	i = 0;
	j = 0;
	while(j < sampleCount)
	{
		code = GetNextCode(*curCode, i);
	
		if(code >15)
			exit(0);

		if((code & 8) != 0)
			fg = 1;	//����λΪ1
		else
			fg = 0;	//����λΪ0

		code &= 7;	//��ȥ����λ���codeֵ

		diff = (stepsizeTable[index] * code) / 4 + stepsizeTable[index] / 8;
		if(fg == 1)
			diff = -diff;

		curSample += diff;
		
		if(curSample > 32767)
		{
			curSample = 32767;
			printf("++\n");
		}
			
		else if(curSample < -32767)
		{
			curSample = -32767;
			printf("--\n");
		}

		/*�����������ݴ��������������*/
		OutPutSample(outBufPtr, curSample);
		
		outBufPtr += 2;
		outPutFrame ++;

		/*����indexֵ�������´ν���*/
		index += indexTable[code];
		if(index < 0)
			index = 0;
		if(index > 88)
			index = 88;
		
		if((i % 2) != 0)	//��ѹ��һ���ֽ��������sample��������һ���ֽ�
		{
			curCode++;
			j++;
		}
		i++;
	}

	return outPutFrame;
	
}

/*********************************************************
    Function:       VoiceLibVolumeSet
    Description:    ����ϵͳ����
    Param:          type		E_VoiceLibVolumeType
    				value		����ֵ[0-16]
    Return:         1-�ɹ�,0-ʧ��
    Other:          none
*********************************************************/
int VoiceLibVolumeSet(E_VoiceLibVolumeType type,long value)
{
	if(type > E_VOICE_LIB_VOLUME_R)
	{
		printf("voiceLib type=%d param is invalid.range[0-%d] .\n",type,E_VOICE_LIB_VOLUME_R);
		return  0;
	}
	if(value > VOICE_LIB_MAX_VOL)
	{
		printf("voiceLib value=%d param is invalid.range[0-%d] .\n",value,VOICE_LIB_MAX_VOL);
		return  0;
	}
	printf("voiceLib volume set type=%d,value=%d.\n",type,value);

	
	long valueMin = 0;
	long valueMax = VOICE_LIB_MAX_H_VOL;
	long v = (value >= VOICE_LIB_MAX_H_VOL) ? (VOICE_LIB_MAX_H_VOL) : (value);

    snd_mixer_t *mixer;
	snd_mixer_elem_t *master_element;
    snd_mixer_open(&mixer, 0);
    snd_mixer_attach(mixer, "default");
    snd_mixer_selem_register(mixer, NULL, NULL);
    snd_mixer_load(mixer);  

	for(master_element = snd_mixer_first_elem(mixer);master_element;master_element=snd_mixer_elem_next(master_element))
	{
		if(0 == strcmp(snd_mixer_selem_get_name(master_element),"PCM"))
		{
			break;
		}
	}
	if(master_element == NULL)
	{
		printf("voiceLib not found PCM.\n");
		snd_mixer_close(mixer);
		return 0;
	}
	snd_mixer_selem_set_playback_volume_range(master_element, valueMin, valueMax); 

	
	if(E_VOICE_LIB_VOLUME_L == type)
	{
		snd_mixer_selem_set_playback_volume(master_element, SND_MIXER_SCHN_FRONT_LEFT, v);
	}
	else if(E_VOICE_LIB_VOLUME_R == type)
	{
		snd_mixer_selem_set_playback_volume(master_element, SND_MIXER_SCHN_FRONT_RIGHT, v);  
	}

	snd_mixer_close(mixer);

	return 1;
}

