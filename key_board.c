#include "key_board.h"
#include "timer0.h"

volatile unsigned char key_presssed = 0xFF;
volatile unsigned char key_released = 0xFF;
volatile unsigned char key_presssed_long = 0xFF;
volatile unsigned char key_released_long = 0xFF;

#define KEY_NUM	4
volatile unsigned char press_counter [KEY_NUM];

void key_board_init (void)
{
DDRD &=~( (1 << PD4)|(1 << PD5)|(1 << PD6)|(1 << PD7));
PORTD |=( (1 << PD4)|(1 << PD5)|(1 << PD6)|(1 << PD7));

key_board_reset ();
}

#define	SMALL_TIME	2
#define	LONG__TIME	60

volatile unsigned char lines;
volatile unsigned char lines_before = 0xFF;
void key_board_scan (void)
{
lines = (PIND&0xF0);
lines>>=4;


if ((key_released==0xFF) && (key_released_long==0xFF))
	{
	unsigned char rise_or_up = lines_before^lines;
	if ( rise_or_up) 
		{
		for (unsigned char i = 0; i < KEY_NUM; i++)
			{
			if (bit_is_set (rise_or_up, i) && bit_is_set (lines, i) )  
				{
				if ( (press_counter [i] > SMALL_TIME) && (press_counter [i] < LONG__TIME))
					{key_released = i;}
				if (press_counter [i] > LONG__TIME)	
					{key_released_long = i;}
				//key_released = i;
				//PORTB ^= (1 << 0);
				break;
				}
			}
		}	
	
	}

if ((key_released==0xFF) && (key_presssed_long==0xFF))
	{
	for (unsigned char i = 0; i < KEY_NUM; i++)
		if (bit_is_clear (lines, i))
			{
			if (press_counter [i] < 230) press_counter [i]++;
			if (press_counter [i] == SMALL_TIME) key_presssed = i;
			//if (press_counter [i] == LONG__TIME) sound=3;
			if (press_counter [i] == (LONG__TIME+6)) {key_presssed_long = i; press_counter [i] = LONG__TIME+1;}
			}
		else
			{
			press_counter [i] = 0;		
			}
	}



lines_before = lines;
}

void key_board_reset (void)
{
lines_before = lines  = 0xFF;
key_released = 0xFF; key_released_long =  key_presssed_long = 0xFF;
for (unsigned char i = 0; i < KEY_NUM; i++) press_counter [i] = 0;

}