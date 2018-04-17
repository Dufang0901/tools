#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>

struct WAV_HEADER
{
    char rld[4]; //riff 标志符号 4
    unsigned int rLen; // 4
    char wld[4]; //格式类型（wave） 4
    char fld[4]; //"fmt" 4

    unsigned int fLen; //sizeof(wave format matex) 4
    
    short wFormatTag; //编码格式
    short wChannels; //声道数
    unsigned int nSamplesPersec ; //采样频率
    unsigned int nAvgBitsPerSample;//WAVE文件采样大小
    short wBlockAlign; //块对齐
    short wBitsPerSample; //WAVE文件采样大小
   // int a;
    char dld[4]; //”data“
    unsigned int wSampleLength; //音频数据的大小

} wav_header;

int set_pcm_play(FILE *fp);

int main(int argc,char *argv[])
{

	int nread;
    FILE *fp;
    fp=fopen(argv[1],"rb");
	//fp=fopen("pcm.wav","rb");
    if(fp==NULL)
    {
        perror("open file failed:\n");
        exit(1);
    }

	printf("wav_header is %d bytes\n",sizeof(wav_header));
    nread=fread(&wav_header,1,sizeof(wav_header),fp);
    
	printf("sizeof(wav_header) = %d\r\n",sizeof(wav_header));
	printf("(4)File Mark   		rld=%c%c%c%c.\n",wav_header.rld[0],wav_header.rld[1],wav_header.rld[2],wav_header.rld[3]);
	printf("(4)File Length 		rLen=0x%X.\n",wav_header.rLen);
	printf("(4)WAVw Mark 		wld=%c%c%c%c.\n",wav_header.wld[0],wav_header.wld[1],wav_header.wld[2],wav_header.wld[3]);
	printf("(4)Wave File Mark   fld=%c%c%c%c.\n",wav_header.fld[0],wav_header.fld[1],wav_header.fld[2],wav_header.fld[3]);	  
	printf("(4)Block Length		fLen=%d.\n",wav_header.fLen);	 
	printf("(2)Format Tag 		wFormatTag=%d.\n",wav_header.wFormatTag);
	printf("(2)Channel Count	wChannels=%d.\n",wav_header.wChannels);
	printf("(4)Sampling Rate 	nSamplesPersec=%d.\n",wav_header.nSamplesPersec);
	printf("(4)Bit Per Second 	nAvgBitsPerSample=%d.\n",wav_header.nAvgBitsPerSample);
	printf("(2)Block Align		wBlockAlign=%d.\n",wav_header.wBlockAlign);    
	printf("(2)Bit Per Sample	wBitsPerSample=%d.\n",wav_header.wBitsPerSample);	  
	//printf("(2)每块PCM个数  wBlockNum=%d.\n",wav_header.wBlockNum);	 
	printf("(4)Data Mark		dld=%c%c%c%c.\n",wav_header.dld[0],wav_header.dld[1],wav_header.dld[2],wav_header.dld[3]);
	printf("(4)Data Length 		wSampleLength=%d.\n",wav_header.wSampleLength);
    
    //return 1;
    
    
    set_pcm_play(fp);
    return 0;
}

int set_pcm_play(FILE *fp)
{
        int rc;
        int ret;
        int size;
        snd_pcm_t* handle; //PCI设备句柄
        snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
        unsigned int val;
        int dir=0;
        snd_pcm_uframes_t frames, periodSize, bufferSize;
        char *buffer;
        int channels=wav_header.wChannels;
        int frequency=wav_header.nSamplesPersec;
        int bit=wav_header.wBitsPerSample;
        int datablock=wav_header.wBlockAlign;
        unsigned char ch[100]; //用来存储wav文件的头信息
    
    
        
        rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if(rc<0)
        {
                perror("\nopen PCM device failed:");
                exit(1);
        }


        snd_pcm_hw_params_alloca(&params); //分配params结构体
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_alloca:");
                exit(1);
        }
         rc=snd_pcm_hw_params_any(handle, params);//初始化params
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_any:");
                exit(1);
        }
        rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //初始化访问权限
        if(rc<0)
        {
                perror("\nsed_pcm_hw_set_access:");
                exit(1);

        }

        //设置采样位数
        switch(bit/8)
        {
        case 1:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
                break ;
        case 2:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
                break ;
        case 3:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S24_LE);
                break ;

        }
		//设置通道数
        rc=snd_pcm_hw_params_set_channels(handle, params, channels); //设置声道,1表示单声道，2表示立体声
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_set_channels:");
                exit(1);
        }
		//设置频率
        val = frequency;
        rc=snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir); //设置频率
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_set_rate_near:");
                exit(1);
        }
		//设置缓冲区大小
		bufferSize = 32;
		rc = snd_pcm_hw_params_set_buffer_size_near(handle, params, &bufferSize);
		if(rc < 0)
		{
			perror("\nsnd_pcm_hw_params_set_buffer_size_near:");
			exit(1);
		}
		
		//设置周期大小
		periodSize = bufferSize / 2;	//一般缓冲区大小是周期大小的两倍
		rc = snd_pcm_hw_set_period_size_near(handle, params, &periodSize, 0);
		if(rc < 0)
		{
			perror("\nsnd_pcm_hw_set_period_size_near:");
			exit(1);
		}
		
		//将以上参数写入ALSA驱动
        rc = snd_pcm_hw_params(handle, params);
        if(rc<0)
        {
        perror("\nsnd_pcm_hw_params: ");
        exit(1);
        }

        rc=snd_pcm_hw_params_get_period_size(params, &frames, &dir); /*获取周期长度*/
		printf("frames=%d\n",(int)frames);
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_get_period_size:");
                exit(1);
        }

        size = frames * datablock; /*datablock=2 代表数据块长度*/
		printf("size=%d,frames=%d,datablock=%d\r\n",size,frames,datablock);

        buffer =(char*)malloc(size);
		//fseek(fp,58,SEEK_SET); //定位歌曲到数据区
		fseek(fp,sizeof(wav_header),SEEK_SET);

		while (1)
        {
			memset(buffer,0,sizeof(buffer));
			ret = fread(buffer, 1, size, fp);
			//printf("fread ret=%d\r\n",ret);
			if(ret == 0)
			{
				printf("歌曲写入结束\n");
				break;
			}
			else if (ret != size)
			{
			}

			// 写音频数据到PCM设备 
			while(ret = snd_pcm_writei(handle, buffer, frames)<0)
		   {
				 usleep(2000); 
				 if (ret == -EPIPE)
				{
				  /* EPIPE means underrun */
				  fprintf(stderr, "underrun occurred\n");
				  //完成硬件参数设置，使设备准备好 
				  snd_pcm_prepare(handle);
				 }
				 else if (ret < 0)
				 {
					fprintf(stderr,"error from writei: %s\n",snd_strerror(ret));
				 }
			}
		}
		//sleep(10);
		printf("aaaaaaaa\r\n");
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
		printf("bbbbbbb\r\n");
        free(buffer);
		fclose(fp);
        return 0;
}
