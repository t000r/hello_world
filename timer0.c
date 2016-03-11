#include "timer0.h"
#include "key_board.h"

#include "global.h"

#include <avr/wdt.h>

volatile unsigned char sec;
volatile unsigned char delay_5451;
volatile unsigned int t0_counter;
volatile unsigned char IPS_ON_counter [3];
volatile unsigned char IPS_OFF_counter [3];

volatile unsigned int error;
volatile unsigned int error_IPS521_1;
volatile unsigned int error_IPS521_2;
volatile unsigned int error_IPS521_3;
volatile unsigned int state_IPS521_1;
volatile unsigned int state_IPS521_2;
volatile unsigned int state_IPS521_3;


extern unsigned char run5451_flag;
extern unsigned int rez;

//инициализация таймера0
void init_timer0(void) 
{
  TCCR0 = _BV (CS01) | _BV (CS00);				/* TCCR0: работает на частоте CK / 64  */
  TCNT0 = devide_ms;   // /245
  TIMSK|=(_BV (TOIE0) );   			/* TOIE1: Timer/Counter0 Overflow Interrupt Enable */
  
// 1кгц
}


SIGNAL (SIG_OVERFLOW0)         //прерывание вызывается при пекреполнении таймера 0
{// 1k
sei(); 
TCNT0 += devide_ms;
t0_counter++;

wdt_reset();

if (t0_counter == 1000)
	{
	//PORTB^=(1 << PB0);
	;//t0_counter = 0;
	}
if (!(t0_counter&0x00FF)) 	// /1024 = 4 Гц 
	{
	sec++;
	if (delay_5451) delay_5451--;
	}
// кнопки 
if (!(t0_counter&0x000F)) 	// /16 = 64 Гц 
	key_board_scan();

if (!(t0_counter&0x0003)) 	// /4 = 256 Гц плавная регулировка ШИМа
	{
	if ( (run5451_flag == 2) || (run5451_flag == 0) ) 
		if (pwn_now) pwn_now--;
		
	if ( (run5451_flag == 1) || (run5451_flag == 3) )  
		{
		if (pwn_now < (rez >> 2)) 		pwn_now++;
		else if (pwn_now > (rez >> 2))	pwn_now--;
		}
		
	}	
// IPS2
// IPS3
// IPS4

if (!(t0_counter&0x000F)) // /16 = 64 Гц
	{
static volatile unsigned char sw;
sw++;
if (sw > 3) sw = 0;
switch (sw)
{
case 0:	
	//////////////////////  3
	if ( (state_IPS521_3) && (error_IPS521_3 == 0) )
		ON__IPS521_3;
	if (state_IPS521_3 == 0)
		OFF_IPS521_3;
		
		
	if (bit_is_set (IPS_521_3_PINx, IPS_521_3_PIN))
		{
		IPS_ON_counter[2]++; 
		if (IPS_ON_counter[2] > 2) 
			{
			IPS_ON_counter[2] = 2;
			if (bit_is_clear (IPS_521_3_PINx, IPS_521_3_PIN_DG))
				{error_IPS521_3 = 1;OFF_IPS521_3;}
			}
		}
	else
		{
		IPS_ON_counter[2] = 0;
		}
break;

case 1:		
	//////////////////////  2
	if ( (state_IPS521_2) && (error_IPS521_2 == 0) )
		ON__IPS521_2;
	if (state_IPS521_2 == 0)
		OFF_IPS521_2;
		
		
	if (bit_is_set (IPS_521_2_PINx, IPS_521_2_PIN))
		{
		IPS_ON_counter[1]++; 
		if (IPS_ON_counter[1] > 2) 
			{
			IPS_ON_counter[1] = 2;
			if (bit_is_clear (IPS_521_2_PINx, IPS_521_2_PIN_DG))
				{error_IPS521_2 = 1;OFF_IPS521_2;}
			}
		}
	else
		{
		IPS_ON_counter[1] = 0;
		}	
break;

case 2:
	//////////////////////  1
	if ( (state_IPS521_1) && (error_IPS521_1 == 0) )
		ON__IPS521_1;
	if (state_IPS521_1 == 0)
		OFF_IPS521_1;
		
		
	if (bit_is_set (IPS_521_1_PINx, IPS_521_1_PIN))
		{
		IPS_ON_counter[0]++; 
		if (IPS_ON_counter[0] > 2) 
			{
			IPS_ON_counter[0] = 2;
			if (bit_is_clear (IPS_521_1_PINx, IPS_521_1_PIN_DG))
				{error_IPS521_1 = 1;OFF_IPS521_1;}
			}
		}
	else
		{
		IPS_ON_counter[0] = 0;
		}
break;
}	
	}

}




//////////////////   T2
volatile unsigned int pwn_now = 5;
volatile unsigned char count_t2;

//инициализация таймера2
void init_timer2(void) 
{
  TCCR2 = _BV (CS22) | _BV (CS21);				/* TCCR2: работает на частоте CK / 256  */
  TIMSK|=(_BV (TOIE2) );   			/* TOIE1: Timer/Counter0 Overflow Interrupt Enable */
// 65кгц
}


SIGNAL (SIG_OVERFLOW2)         //прерывание вызывается при пекреполнении таймера 0
{// 65k  /256 = 253
TCNT2 = 255;
count_t2++;


/*
if (count_t2 < pwn_now)	PORTB |= (1 << 1); 
else					PORTB &=~(1 << 1);*/

if (delay_5451 == 0) 
	{
	//контроль кз IPS1 - 5451
	if  (count_t2 == (pwn_now-2))
		if  (pwn_now > 20)
			{if ( bit_is_set (IPS_5451_PINx, IPS_5451_PIN) && (bit_is_clear (IPS_5451_PINx, IPS_5451_PIN_DG) ))//смторим DG ножку и если что считаем ошибку
				{if (error < 30000) error++;}
			}
	}		
	/*if (error > 2000) WORK_LED_PORT |= (1 << WORK_LED_PIN);
	else 			WORK_LED_PORT &=~(1 << WORK_LED_PIN);*/
if (error < 10)
	{
	if (count_t2 < pwn_now)	IPS_5451_PORT|= (1 << IPS_5451_PIN); 
	else					IPS_5451_PORT&=~(1 << IPS_5451_PIN);	
	}
else
	IPS_5451_PORT&=~(1 << IPS_5451_PIN);
}
