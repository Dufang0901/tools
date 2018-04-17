1:
	gcc -Wall adpcm_play.c -o adpcm_play -L/usr/lib/ -lasound -I /usr/include/
	#arm-linux-gnueabihf-gcc -Wall adpcm_play.c -o adpcm_play -L /usr/local/alsa/lib/ -lasound -I /usr/local/alsa/include/
2:
	arm-linux-gnueabihf-gcc -Wall pcm_play.c -o pcm_play -L/usr/lib/ -lasound -I /usr/include/
clean:
	rm adpcm_play pcm_play
	rm *.bak