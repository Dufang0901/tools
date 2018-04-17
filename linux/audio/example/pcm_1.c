#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/soundcard.h>
#include <alsa/asoundlib.h>

#define ALSA_MAX_BUF_SIZE 65535

int play_sound(char* filename,int rate,int bits,int channel,int order)
{
        long loops;
        int rc,size,dir;
        snd_pcm_t *handle;
        snd_pcm_hw_params_t *params;
        snd_pcm_uframes_t frames,periodsize;
        snd_mixer_t *mixer;
        snd_mixer_elem_t *pcm_element;

        char *buffer;
        unsigned int val;
        FILE *fp = fopen(filename,"rb");
        rc = snd_pcm_open(&handle,"default",SND_PCM_STREAM_PLAYBACK,0);

        snd_pcm_hw_params_alloca(&params);
        snd_pcm_hw_params_any(handle,params);
        snd_pcm_hw_params_set_access(handle,params,SND_PCM_ACCESS_RW_INTERLEAVED);
        switch(order){
                case 1:
                        snd_pcm_hw_params_set_format(handle,params,SND_PCM_FORMAT_S16_LE);
                        break;
                case 2:
                        snd_pcm_hw_params_set_format(handle,params,SND_PCM_FORMAT_S16_BE);
                        break;
                defualt:
                        break;
        }
        snd_pcm_hw_params_set_channels(handle,params,channel);

        val = rate;
        snd_pcm_hw_params_set_rate_near(handle,params,&val,0);
        snd_pcm_hw_params_get_buffer_size_max(params,&frames);
        frames = frames < ALSA_MAX_BUF_SIZE? frames:ALSA_MAX_BUF_SIZE;
        rc = snd_pcm_hw_params_set_buffer_size_near(handle,params,&frames);
        snd_pcm_hw_params_get_period_size_min(params,&periodsize,NULL);
        if(!periodsize){
                periodsize=size/4;
        }
        rc = snd_pcm_hw_params_set_period_size_near(handle,params,&periodsize,NULL);
        rc = snd_pcm_hw_params(handle,params);

        snd_mixer_open(&mixer,0);
        snd_mixer_attach(mixer,"default");
        snd_mixer_selem_register(mixer,NULL,NULL);
        snd_mixer_load(mixer);
        for(pcm_element = snd_mixer_first_elem(mixer);pcm_element;pcm_element=snd_mixer_elem_next(pcm_element))
        {
                if(snd_mixer_elem_get_type(pcm_element)==SND_MIXER_ELEM_SIMPLE && snd_mixer_selem_is_active(pcm_element))
                {
                        if(!strcmp(snd_mixer_selem_get_name(pcm_element),"Master"))
                        {
                                snd_mixer_selem_set_playback_volume_range(pcm_element,0,100);
                                snd_mixer_selem_set_playback_volume_all(pcm_element,(long)100);
                        }
                }
        }

        buffer = (char*)malloc(size);
        while(1)
        {
                rc = fread(buffer,1,size,fp);
                if(0== rc)
                        break;
                while((rc = snd_pcm_writei(handle,buffer,size))<0)
                {
                        usleep(200);
                        if(-EPIPE == rc)
                                snd_pcm_prepare(handle);
                        else if(0 > rc)
                                printf("error fomr writei\n");
                }
        }
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        free(buffer);
        snd_mixer_close(mixer);
        fclose(fp);
        return 0;
}


int main(int argc,char** argv){
        play_sound(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
        return 0;
}