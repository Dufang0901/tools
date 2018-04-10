#include <stdio.h>
#include <stdint.h>

struct PCM_HEADER
{
    char rld[4]; 					//riff 标志符号
    unsigned int rLen; 
    char wld[4]; 					//格式类型（wave）
    char fld[4]; 					//"fmt"
    unsigned int fLen; 				//sizeof(wave format matex)
    short wFormatTag; 				//编码格式
    short wChannels; 				//声道数
    unsigned int nSamplesPersec ; 	//采样频率
    unsigned int nAvgBitsPerSample;	//WAVE文件采样大小
    short wBlockAlign; 				//块对齐    
	short wBitsPerSample; 			//WAVE文件采样大小
	char dld[4]; 					//”data“
    unsigned int wSampleLength; 	//音频数据的大小

} head;


int frame_size_get(int inSampleRate, int ChannleNumber)
{
    int size= -1;
    switch(inSampleRate)
    {
    case 8000:
        {
            size= 320;
        }
        break;
    case 16000:
        {
            size= 640;
        }
        break;
    case 24000:
        {
            size= 960;
        }
        break;
    case 32000:
        {
            size= 1280;
        }
        break;
    case 48000:
        {
            size= 1920;
        }
        break;
    case 44100:
        {
            size= 441*4;//为了保证8K输出有320,441->80,*4->320
        }
        break;
    case 22050:
        {
            size= 441*2;
        }
        break;
    case 11025:
        {
            size= 441;
        }
        break;
    default:
        break;
    }
    return ChannleNumber*size;
}


void RaiseVolume(char* buf, uint32_t size,uint32_t uRepeat,double vol)
{
	 if (!size )
	 {
	  	return;
	 }

	int i,j;
	 for (i = 0; i < size;)
	{
		  signed long minData = -0x8000; //如果是8bit编码这里变成-0x80
		  signed long maxData = 0x7FFF;//如果是8bit编码这里变成0xFF
		  
		  signed short wData = buf[i+1];
		  //wData = MAKEWORD(buf[i],buf[i+1]);
		  wData = buf[i] | buf[i+1]<<8;
		  signed long dwData = wData;
		  
		  for (j = 0; j < uRepeat; j++)
		  {
			   dwData = dwData * vol;//1.25;
			   if (dwData < -0x8000)
			   {
					printf("----\n");
					dwData = -0x8000;
			   }
			   else if (dwData > 0x7FFF)
			   {
					printf("++++\n");
					dwData = 0x7FFF;
			   }
		  }
		  //wData = LOWORD(dwData);
		  wData = dwData &0xffff;
		  //buf[i] = LOBYTE(wData);
		  buf[i] = wData & 0xff;
		  //buf[i+1] = HIBYTE(wData);
		  buf[i+1] = (wData>>8) & 0xff;
		  i += 2;
	}
}


//vol取0—10即可，为0时为静音，小于1声音减小，大于1声音增大，测试取大于10的数字效果不明显
int pcm_volume_control(char* foldname, char* fnewname, double vol)
{
    int frame_size= 0;
    char* frame_buffer;
    FILE* fp_old= fopen(foldname,"rb+");
    FILE* fp_new= fopen(fnewname,"wb+");
    if((NULL== fp_old) || (NULL== fp_new))
    {
        return -1;
    }
    fread(&head,1,sizeof(head),fp_old);
    fwrite(&head,1,sizeof(head),fp_new);
	printf("rate=%d channels=%d\n",head.nSamplesPersec,head.wChannels);
    frame_size= frame_size_get( head.nSamplesPersec,head.wChannels);
	printf("frame_size=%d\n",frame_size);
    frame_buffer= (char*)malloc(frame_size);

    while(frame_size== fread(frame_buffer,1,frame_size,fp_old))
    {
 		RaiseVolume(frame_buffer,frame_size,1,vol);
        fwrite(frame_buffer,1,frame_size,fp_new);
    }
    fclose(fp_old);
    fclose(fp_new);
    free(frame_buffer);  
    return 0;
}

void main()
{
	pcm_volume_control("./audio/K30011.wav","./audio/new.mp3",0.8);	

}



