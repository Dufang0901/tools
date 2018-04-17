#include<stdio.h>
#include<alsa/asoundlib.h>

void main()
{
	long al, ar, amin = 0, amax = 10;
	snd_mixer_t *mixer;
	snd_mixer_elem_t *master_element;
	snd_mixer_selem_id_t id;

	snd_mixer_open(&mixer, 0);
	snd_mixer_attach(mixer, "default");
	snd_mixer_selem_register(mixer, NULL, NULL);
	snd_mixer_load(mixer); 
	master_element = snd_mixer_first_elem(mixer); 

	int counter =5;
	while(counter)
	{
		snd_mixer_selem_get_id(master_element,&id);
		snd_mixer_selem_get_playback_volume_range(master_element, &amin, &amax);
		printf("%d\n",__LINE__);
		
		snd_mixer_selem_get_playback_volume(master_element, SND_MIXER_SCHN_FRONT_LEFT, &al);  
		printf("%d\n",__LINE__);
		snd_mixer_selem_get_playback_volume(master_element, SND_MIXER_SCHN_FRONT_RIGHT, &ar);
		printf("%d\n",__LINE__);

		printf("numid=%d,range=%ld-%ld,left=%ld,right=%ld\n",id,amin,amax,al,ar);

		master_element = snd_mixer_elem_next(master_element);
		printf("%d\n",__LINE__);
		counter--;
	}
	
	snd_mixer_close(mixer);
	
}