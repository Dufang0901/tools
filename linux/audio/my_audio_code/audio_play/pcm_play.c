#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>

struct WAV_HEADER
{
    char rld[4]; //riff ��־���� 4
    unsigned int rLen; // 4
    char wld[4]; //��ʽ���ͣ�wave�� 4
    char fld[4]; //"fmt" 4

    unsigned int fLen; //sizeof(wave format matex) 4
    
    short wFormatTag; //�����ʽ
    short wChannels; //������
    unsigned int nSamplesPersec ; //����Ƶ��
    unsigned int nAvgBitsPerSample;//WAVE�ļ�������С
    short wBlockAlign; //�����
    short wBitsPerSample; //WAVE�ļ�������С
   // int a;
    char dld[4]; //��data��
    unsigned int wSampleLength; //��Ƶ���ݵĴ�С

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
	//printf("(2)ÿ��PCM����  wBlockNum=%d.\n",wav_header.wBlockNum);	 
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
        snd_pcm_t* handle; //PCI�豸���
        snd_pcm_hw_params_t* params;//Ӳ����Ϣ��PCM������
        unsigned int val;
        int dir=0;
        snd_pcm_uframes_t frames, periodSize, bufferSize;
        char *buffer;
        int channels=wav_header.wChannels;
        int frequency=wav_header.nSamplesPersec;
        int bit=wav_header.wBitsPerSample;
        int datablock=wav_header.wBlockAlign;
        unsigned char ch[100]; //�����洢wav�ļ���ͷ��Ϣ
    
    
        
        rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if(rc<0)
        {
                perror("\nopen PCM device failed:");
                exit(1);
        }


        snd_pcm_hw_params_alloca(&params); //����params�ṹ��
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_alloca:");
                exit(1);
        }
         rc=snd_pcm_hw_params_any(handle, params);//��ʼ��params
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_any:");
                exit(1);
        }
        rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //��ʼ������Ȩ��
        if(rc<0)
        {
                perror("\nsed_pcm_hw_set_access:");
                exit(1);

        }

        //���ò���λ��
        switch(bit/8)
        {
        case 1:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
                break ;
        case 2:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
                break ;
        case 3:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S24_LE);
                break ;

        }
		//����ͨ����
        rc=snd_pcm_hw_params_set_channels(handle, params, channels); //��������,1��ʾ��������2��ʾ������
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_set_channels:");
                exit(1);
        }
		//����Ƶ��
        val = frequency;
        rc=snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir); //����Ƶ��
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_set_rate_near:");
                exit(1);
        }
		//���û�������С
		bufferSize = 32;
		rc = snd_pcm_hw_params_set_buffer_size_near(handle, params, &bufferSize);
		if(rc < 0)
		{
			perror("\nsnd_pcm_hw_params_set_buffer_size_near:");
			exit(1);
		}
		
		//�������ڴ�С
		periodSize = bufferSize / 2;	//һ�㻺������С�����ڴ�С������
		rc = snd_pcm_hw_set_period_size_near(handle, params, &periodSize, 0);
		if(rc < 0)
		{
			perror("\nsnd_pcm_hw_set_period_size_near:");
			exit(1);
		}
		
		//�����ϲ���д��ALSA����
        rc = snd_pcm_hw_params(handle, params);
        if(rc<0)
        {
        perror("\nsnd_pcm_hw_params: ");
        exit(1);
        }

        rc=snd_pcm_hw_params_get_period_size(params, &frames, &dir); /*��ȡ���ڳ���*/
		printf("frames=%d\n",(int)frames);
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_get_period_size:");
                exit(1);
        }

        size = frames * datablock; /*datablock=2 �������ݿ鳤��*/
		printf("size=%d,frames=%d,datablock=%d\r\n",size,frames,datablock);

        buffer =(char*)malloc(size);
		//fseek(fp,58,SEEK_SET); //��λ������������
		fseek(fp,sizeof(wav_header),SEEK_SET);

		while (1)
        {
			memset(buffer,0,sizeof(buffer));
			ret = fread(buffer, 1, size, fp);
			//printf("fread ret=%d\r\n",ret);
			if(ret == 0)
			{
				printf("����д�����\n");
				break;
			}
			else if (ret != size)
			{
			}

			// д��Ƶ���ݵ�PCM�豸 
			while(ret = snd_pcm_writei(handle, buffer, frames)<0)
		   {
				 usleep(2000); 
				 if (ret == -EPIPE)
				{
				  /* EPIPE means underrun */
				  fprintf(stderr, "underrun occurred\n");
				  //���Ӳ���������ã�ʹ�豸׼���� 
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
