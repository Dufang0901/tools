#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <unistd.h>


 
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
    char dld[4]; //”data“
    unsigned int wSampleLength; //音频数据的大小

} wav_header;

int set_pcm_play(FILE *fp);

int main(int argc,char *argv[])
{

	int nread;
    FILE *fp;
    fp=fopen("new.pcm","rb");
	//fp=fopen("1.wav","rb");
    if(fp==NULL)
    {
        perror("open file failed:\n");
        exit(1);
    }
    
    nread=fread(&wav_header,1,sizeof(wav_header),fp);
	
	
	char buf[100]={0};
	memcpy(buf,wav_header.rld,100);
	int i = 0;
	printf("==========================================\n");
	for(i=0;i<44;i++)
	{
		if(i%16 == 0)
		{
			printf("\n");
		}
		printf("%02X ",buf[i]);
	}
	printf("\n==========================================\n");
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
        
        snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
        unsigned int val;
        int dir=0;
        snd_pcm_uframes_t frames;
        char *buffer;
		char *outbuff;
        int channels=wav_header.wChannels;
        int frequency=wav_header.nSamplesPersec;
        int bit=wav_header.wBitsPerSample;
        int datablock=wav_header.wBlockAlign;
  
    
        
        snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        snd_pcm_hw_params_alloca(&params); 
        snd_pcm_hw_params_any(handle, params);
        snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); 
        snd_pcm_hw_params_set_format(handle, params, 2);
        snd_pcm_hw_params_set_channels(handle, params, channels); 
        snd_pcm_hw_params_set_rate_near(handle, params, &frequency, &dir);
		frames = 505;
        //snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
		snd_pcm_hw_params_set_buffer_size (handle, params, frames*4);
        snd_pcm_hw_params(handle, params);
        //snd_pcm_hw_params_get_period_size(params, &frames, &dir);
		//frames = 505;
        size = frames * datablock; /*4 代表数据快长度*/
	printf("size=%d,frames=%d,datablock=%d\r\n",size,frames,datablock);

        buffer =(char*)malloc(size);
		
		fseek(fp,sizeof(wav_header),SEEK_SET); //定位歌曲到数据区

		while (1)
        {
			memset(buffer,0,sizeof(buffer));
			ret = fread(buffer, 1, size, fp);
			//printf("fread ret=%d\r\n",ret);
			//sleep(1);
			if(ret == 0)
			{
				printf("歌曲写入结束\n");
				break;
			}
	
			// 写音频数据到PCM设备 
			ret = snd_pcm_writei(handle, buffer, frames);
		}
		sleep(10);
        return 0;
}
