#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "filter.h"
#include "psg.h"


#define BUFSIZE 1024
#define FORDER 5

const int freq=44100;
const Uint16 format=AUDIO_S16SYS;
const int channels=2;
const int chunksize=4096;
const int n_channels=4;

int timer_dec=300;
int filter_enabled=1;

const double filter_ks[FORDER]={0.03,0.05,0.07,0.15,0.70};
filter_type* left_filter;
filter_type* right_filter;



void sound_callback(void *udata, Uint8 *stream, int len)
{
		int16_t* buf=(void*)stream;
		for(int i=0;i<len/4;i++)
		{
			int16_t left=get_next_ds_sample();
			int16_t right=get_next_ds_sample();
			if(filter_enabled)
			{
				buf[2*i+0]=filter(left_filter,left);
				buf[2*i+1]=filter(right_filter,right);
			} else
			{
				buf[2*i+0]=left;
				buf[2*i+1]=right;
			}
		}
}

void sound_init(void)
{
	if(Mix_OpenAudio(freq,format,channels,chunksize)==-1)
	{
	printf("Mix_OpenAudio: %s\n", Mix_GetError());
    exit(2);
	}
	Mix_AllocateChannels(n_channels);
	Mix_HookMusic(sound_callback,NULL);
}

int main(int argc, char*argv[])
{
	SDL_Event event;
	int done=0;
	int td=1;
	char *endptr;
	
	if(argc>1)
	{
		for(int i=1;i<argc;i++)
		{
			if(strcmp("-nf",argv[i])==0) filter_enabled=0;
			else
			if(strcmp("-h",argv[i])==0)
			{
				printf("%s [options]\n",argv[0]);
				printf("    -nf    no filter. Не фильтровать звук.\n");
				printf("    <1-n>  timer decrement/seed\n");
				exit(0);
			}
			else
			{
				errno = 0;
				td=strtol(argv[i],&endptr,10);
				if(errno==0 && endptr!=argv[i]) timer_dec=td;
			}
		}
	}
	
	SDL_Init(SDL_INIT_AUDIO);
	
	left_filter=filter_init(FORDER,filter_ks);
	right_filter=filter_init(FORDER,filter_ks);
	psg_init(timer_dec);
	sound_init();
	while(done==0)
	{
		usleep(10000);
		if (SDL_PollEvent (&event))
	    {
			if (event.type == SDL_QUIT)
			{
				done = 1;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					done = 1;
				}
			}
		}
	}
	Mix_HookMusic(NULL,NULL);
	filter_done(left_filter);
	filter_done(right_filter);
			
}

