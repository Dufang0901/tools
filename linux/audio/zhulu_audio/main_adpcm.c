#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <unistd.h>

struct adpcm_state
{
short valprev; /* Previous output value */
char index; /* Index into stepsize table */
};
/* Intel ADPCM step variation table */
static int indexTable[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
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
 
void adpcm_decoder(char *inbuff,char *outbuff,int len_of_in,struct adpcm_state * state )
{
 int  i=0,j=0;
 char tmp_data;
 struct adpcm_state * tmp_state =state;
 signed long diff; /* Difference between sample and predicted sample */
 long step; /* Quantizer step size */
 signed long predsample; /* Output of ADPCM predictor */
 signed long diffq; /* Dequantized predicted difference */
 int index; /* Index into step size table */
 int indexa,indexb;
 int Samp;
 unsigned char SampH, SampL;
 unsigned char inCode; 
 
 /* Restore previous values of predicted sample and quantizer step
 size index
 */
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
    /* Inverse quantize the ADPCM code into a predicted difference
    using the quantizer step size
    */
    
    diffq = step >> 3;
    if( inCode & 4 )
    diffq += step;
    if( inCode & 2 )
    diffq += step >> 1;
    if( inCode & 1 )
    diffq += step >> 2;
    /* Fixed predictor computes new predicted sample by adding the
    old predicted sample to predicted difference
    */
    if( inCode & 8 )
    predsample -= diffq;
    else
    predsample += diffq;
    /* Check for overflow of the new predicted sample
    */
    if( predsample > 32767 )
    predsample = 32767;
    else if( predsample < -32768 )
    predsample = -32768;
    /* Find new quantizer stepsize index by adding the old index
    to a table lookup using the ADPCM code
    */
    index += indexTable[inCode];
    /* Check for overflow of the new quantizer step size index
    */
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
 state->valprev = (short)predsample;
 state->index = (char)index;
 //printf("predsample=%d,index=%d,j=%d\r\n",predsample,index,j);
}
 
 
struct WAV_HEADER
{
    char rld[4]; //riff 标志符号
    unsigned int rLen; 
    char wld[4]; //格式类型（wave）
    char fld[4]; //"fmt"
    unsigned int fLen; //sizeof(wave format matex)
    short wFormatTag; //编码格式
    short wChannels; //声道数
    unsigned int nSamplesPersec ; //采样频率
    unsigned int nAvgBitsPerSample;//WAVE文件采样大小
    short wBlockAlign; //块对齐    
	short wBitsPerSample; //WAVE文件采样大小

	//36byte
    short wReserve;
	short wBlockNum;
    char dld[4]; //”data“
    unsigned int wSampleLength; //音频数据的大小

} wav_header;

int set_pcm_play(FILE *fp);


int main(int argc,char *argv[])
{

	int nread;
    FILE *fp;
	fp=fopen("K31011.mp3","rb");
    if(fp==NULL)
    {
        perror("open file failed:\n");
        exit(1);
    }
    
    nread=fread(&wav_header,1,sizeof(wav_header),fp);
	printf("sizeof(wav_header)=%d\r\n",sizeof(wav_header));
    printf("(4)资源文件标志 rld=%c%c%c%c.\n",wav_header.rld[0],wav_header.rld[1],wav_header.rld[2],wav_header.rld[3]);
    printf("(4)文件长度     rLen=0x%X.\n",wav_header.rLen);
    printf("(4)WAVw文件标志 wld=%c%c%c%c.\n",wav_header.wld[0],wav_header.wld[1],wav_header.wld[2],wav_header.wld[3]);
    printf("(4)波形文件标志 fld=%c%c%c%c.\n",wav_header.fld[0],wav_header.fld[1],wav_header.fld[2],wav_header.fld[3]);    
    printf("(4)块长度       fLen=%d.\n",wav_header.fLen);    
    printf("(2)格式种类     wFormatTag=%d.\n",wav_header.wFormatTag);
    printf("(2)通道数       wChannels=%d.\n",wav_header.wChannels);
    printf("(4)采集频率     nSamplesPersec=%d.\n",wav_header.nSamplesPersec);
    printf("(4)传输速率     nAvgBitsPerSample=%d.\n",wav_header.nAvgBitsPerSample);
    printf("(2)数据块       wBlockAlign=%d.\n",wav_header.wBlockAlign);    
    printf("(2)PCM位宽       wBitsPerSample=%d.\n",wav_header.wBitsPerSample);    
	//printf("(2)每块PCM个数  wBlockNum=%d.\n",wav_header.wBlockNum);    
	printf("(4)数据标志符   dld=%c%c%c%c.\n",wav_header.dld[0],wav_header.dld[1],wav_header.dld[2],wav_header.dld[3]);
    printf("(4)数据长度     wSampleLength=%d.\n",wav_header.wSampleLength);
    //return 1;
    
    
    set_pcm_play(fp);
    return 0;
}

snd_pcm_t* handle; //PCI设备句柄

int set_pcm_play(FILE *fp)
{
	int ret;
	int size;
	
	struct adpcm_state ADPCMstate;
	snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
	int dir=0;
	snd_pcm_uframes_t frames;
	char *buffer;
	char *outbuff;
	int frequency=wav_header.nSamplesPersec;
	int bit=wav_header.wBitsPerSample;
	int datablock=wav_header.wBlockAlign;

        
	snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	snd_pcm_hw_params_alloca(&params); //分配params结构体
	snd_pcm_hw_params_any(handle, params);//初始化params
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //初始化访问权限
	snd_pcm_hw_params_set_format(handle, params, 2);
	snd_pcm_hw_params_set_channels(handle, params, wav_header.wChannels); //设置声道,1表示单声>道，2表示立体声
	snd_pcm_hw_params_set_rate_near(handle, params, &frequency, &dir); //设置>频率
	frames = 505;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
	snd_pcm_hw_params_set_buffer_size (handle, params, frames*4);
	snd_pcm_hw_params(handle, params);
	snd_pcm_hw_params_get_period_size(params, &frames, &dir); /*获取周期长度*/

	size = 256; /*4 代表数据快长度*/
	buffer =(char*)malloc(size);
	outbuff = (char*)malloc(size*4);

	printf("size=%d,frames=%d,datablock=%d\r\n",size,frames,datablock);	
	fseek(fp,sizeof(wav_header),SEEK_SET); //定位歌曲到数据区
	int iii = 0;
	int temp = 0;
	while (1)
	{
		memset(buffer,0,sizeof(buffer));
		memset(outbuff,0,sizeof(outbuff));
		ret = fread(buffer, 1, size, fp);
		//printf("fread ret=%d\r\n",ret);
		if(ret == 0)
		{
			printf("歌曲写入结束\n");
			break;
		}
		if(iii == 0)
		{
			ADPCMstate.index = 0; 
			iii++;
		}
		else
		{
			ADPCMstate.index = buffer[2];
		}
		ADPCMstate.valprev = (short)buffer[0] + ((short)(buffer[1]))*256;  
		outbuff[0] = buffer[0];
		outbuff[1] = buffer[1];
		adpcm_decoder(&buffer[4],&outbuff[2],size-4,&ADPCMstate);//
		temp = (size-4)*4 + 2;
		snd_pcm_writei(handle, outbuff, temp/2);
		
	}
	sleep(20);

		
		
	//snd_pcm_drain(handle);
	//snd_pcm_close(handle);
	printf("bbbbbbb\r\n");
	return 0;
	
}





