#include "mad.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <sys/mman.h>

static int decode(unsigned char const *, unsigned long);
int set_pcm();
snd_pcm_t*             handle=NULL;        //PCI设备句柄
snd_pcm_hw_params_t*   params=NULL;			//硬件信息和PCM流配置


int main(int argc, char *argv[])
{
	struct stat stat;
	void *fdm;

	int fd;
	
	fd=open("33.mp3",O_RDWR);
	if(fd<0)
	{
		perror("open file failed:");
		return 1;
	}    

	if (fstat(fd, &stat) == -1 ||stat.st_size == 0)
	{
		printf("fstat failed:\n");
		return 2;
	}

	fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (fdm == MAP_FAILED)
	{
		printf("munmap error\r\n");
		return 3;
	}
    

  
	if(set_pcm()!=0)                 //设置pcm 参数
	{
		printf("set_pcm fialed:\n");
		return 1;   
	}
	
	decode(fdm, stat.st_size);
	//decode(fdm, stat.st_size/2);
	//decode(fdm+stat.st_size/2, stat.st_size-stat.st_size/2);
	if(munmap(fdm, stat.st_size) == -1)
	{
		printf("munmap error\r\n");
		return 4;
	}

	printf("aaaaaaaaaaaaaa\r\n");
	snd_pcm_drain(handle);
	printf("bbbbbbbbbbbbbbbbbb\r\n");
	snd_pcm_close(handle);

	return 0;
}


int set_pcm()
{
    int    rc;     
    int  dir=0;
    int rate = 44100;;                	/* 采样频率 44.1KHz*/
    int format = SND_PCM_FORMAT_S16_LE; /*     量化位数 16      */
    int channels = 2;                 	/*     声道数 2           */
    
    rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(rc<0)
	{
			perror("\nopen PCM device failed:");
			exit(1);
	}
    snd_pcm_hw_params_alloca(&params); //分配params结构体
        
    rc=snd_pcm_hw_params_any(handle, params);//初始化params
	if(rc<0)
	{
			perror("\nsnd_pcm_hw_params_any:");
			exit(1);
	}
    rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);                                 //初始化访问权限
	if(rc<0)
	{
			perror("\nsed_pcm_hw_set_access:");
			exit(1);
	}
        
    rc=snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);             //设置16位采样精度  
	if(rc<0)
	{
		perror("snd_pcm_hw_params_set_format failed:");
		exit(1);
	} 

    rc=snd_pcm_hw_params_set_channels(handle, params, channels);  //设置声道,1表示单声>道，2表示立体声
	if(rc<0)
	{
			perror("\nsnd_pcm_hw_params_set_channels:");
			exit(1);
	}    
        
	rc=snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir);  //设置>频率
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_rate_near:");
		exit(1);
	}   
        
         
	rc = snd_pcm_hw_params(handle, params);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params: ");
		exit(1);
	} 

    return 0;              
}

struct buffer {
  unsigned char const *start;
  unsigned long length;
};


static enum mad_flow input(void *data,struct mad_stream *stream)
{
	struct buffer *buffer = data;

	printf("this is input\n");
	if (!buffer->length)
	{
			return MAD_FLOW_STOP;
	}	

	mad_stream_buffer(stream, buffer->start, buffer->length);

	buffer->length = 0;

	return MAD_FLOW_CONTINUE;
}


static inline signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}


static enum mad_flow output(void *data,struct mad_header const *header,struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples,n;
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */
	
  nchannels = pcm->channels;
  n=nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];
  printf("this is output nsamples=%d\n",nsamples);
  
  unsigned char Output[6912], *OutputPtr;  
  int fmt, wrote, speed, exact_rate, err, dir; 
  
  
//   printf("This is output\n");
   
   
 
   OutputPtr = Output;  
   
   while (nsamples--) 
   {
    signed int sample;

    /* output sample(s) in 16-bit signed little-endian PCM */
    
    sample = scale(*left_ch++);
   
    *(OutputPtr++) = sample >> 0;  
    *(OutputPtr++) = sample >> 8;  
    if (nchannels == 2)  
        {  
            sample = scale (*right_ch++);  
            *(OutputPtr++) = sample >> 0;  
            *(OutputPtr++) = sample >> 8;  
        }  
    
  
  }
 
    OutputPtr = Output;  
    snd_pcm_writei (handle, OutputPtr, n);  
    OutputPtr = Output;     

  return MAD_FLOW_CONTINUE;
}


static enum mad_flow error(void *data,struct mad_stream *stream,struct mad_frame *frame)
{
  struct buffer *buffer = data;
  printf("this is mad_flow error\n");
  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
      stream->error, mad_stream_errorstr(stream),
      stream->this_frame - buffer->start);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}



static int decode(unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;

  /* initialize our private message structure */

  buffer.start  = start;
  buffer.length = length;

  /* configure input, output, and error functions */

  mad_decoder_init(&decoder, &buffer,
           input, 0 /* header */, 0 /* filter */, output,
           error, 0 /* message */);

  /* start decoding */
  printf("decode aaaaaaaa\r\n");
  
   result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
   //result = mad_decoder_run(&decoder, MAD_DECODER_MODE_ASYNC);
	printf("decode bbbbbbbbb\r\n");
  /* release the decoder */

  mad_decoder_finish(&decoder);
	printf("decode cccccccc\r\n");
  return result;
}