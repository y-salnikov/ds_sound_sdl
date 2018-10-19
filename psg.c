#include <stdint.h>
#include "psg.h"
uint16_t timer_decrement=1;

void psg_init(uint16_t timer_dec)
{
	timer_decrement=timer_dec;
}

int16_t get_next_ds_sample(void)
{
	static uint16_t timer;
	static uint16_t r1;
	static uint16_t r3;
	static uint8_t inited;
	int16_t output=-32767;
	if(inited==0)
	{
		r1=3;
		r3=05000;
		timer=010000;
		inited=1;
	}
	timer=timer-timer_decrement;
	if((r1 & timer)==0) output=32767;
	else output=-32767;
	if(--r3==0)
		{
			r3=05000;
			r1++;
		}
	return output;
}
