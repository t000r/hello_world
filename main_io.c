#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include "timer0.h"
#include "key_board.h"
#include "global.h"

unsigned char work_mode = 0;
unsigned char run5451_flag = 0;

unsigned char pump521_flag = 0;

// ADC
void init_ADC (void);
void adc_convert (void);
unsigned int rez;
// ADC




int main (void)
{
// init_ports
WORK_LED_DDR |= (1 << WORK_LED_PIN);
WORK_LED_PORT&=~(1 << WORK_LED_PIN);

RUN_LED_DDR |= (1 << RUN_LED_PIN);
RUN_LED_PORT&=~(1 << RUN_LED_PIN);

SINGLE_LED_DDR |= (1 << SINGLE_LED_PIN);
SINGLE_LED_PORT&=~(1 << SINGLE_LED_PIN);

// IPS
IPS_5451_DDR|=  (1 << IPS_5451_PIN);
IPS_5451_PORT&=~(1 << IPS_5451_PIN);

IPS_5451_DDR&=~ (1 << IPS_5451_PIN_DG); // вход 
IPS_5451_PORT|= (1 << IPS_5451_PIN_DG); // pullup

IPS_521_1_DDR|=  (1 << IPS_521_1_PIN);
IPS_521_1_PORT&=~(1 << IPS_521_1_PIN);

IPS_521_1_DDR&=~ (1 << IPS_521_1_PIN_DG);
IPS_521_1_PORT|= (1 << IPS_521_1_PIN_DG); // pullup 

IPS_521_2_DDR|=  (1 << IPS_521_2_PIN);
IPS_521_2_PORT&=~(1 << IPS_521_2_PIN);

IPS_521_2_DDR&=~ (1 << IPS_521_2_PIN_DG);
IPS_521_2_PORT|= (1 << IPS_521_2_PIN_DG); // pullup

IPS_521_3_DDR|=  (1 << IPS_521_3_PIN);
IPS_521_3_PORT&=~(1 << IPS_521_3_PIN);

IPS_521_3_DDR&=~ (1 << IPS_521_3_PIN_DG);
IPS_521_3_PORT|= (1 << IPS_521_3_PIN_DG); // pullup
// init_ports end



key_board_init();
init_ADC ();
init_timer0 ();
init_timer2 ();


WORK_LED_PORT |= (1 << WORK_LED_PIN);
_delay_ms(500);
WORK_LED_PORT &=~(1 << WORK_LED_PIN);

// Ждем включения
sei();
while (1)
	{
	if (key_released!=0xFF)
		{
		if (key_released == KEY_ON)
			break;
		}
	}
//WORK_LED_PORT |= (1 << WORK_LED_PIN);
_delay_ms(200);
// Диагностика 
if (bit_is_set (IPS_521_1_PINx, IPS_521_1_PIN_DG)) // NC
	{
	PORTB |= (1 << 2);
	_delay_ms(300);
	PORTB &=~(1 << 2);
	}
if (bit_is_set (IPS_521_2_PINx, IPS_521_2_PIN_DG)) // NC
	{
	_delay_ms(300);
	PORTB |= (1 << 2);
	_delay_ms(300);
	PORTB &=~(1 << 2);
	_delay_ms(300);
	PORTB |= (1 << 2);	
	_delay_ms(300);
	PORTB &=~(1 << 2);
	}
if (bit_is_set (IPS_521_3_PINx, IPS_521_3_PIN_DG)) // NC
	{
	_delay_ms(300);
	PORTB |= (1 << 1);
	_delay_ms(300);
	PORTB &=~(1 << 1);
	}
_delay_ms(200);
WORK_LED_PORT &=~(1 << WORK_LED_PIN);
//

wdt_enable(WDTO_1S); 
sei();
for(;;)
	{
	if (key_released!=0xFF)
		{
		switch (key_released)
			{
			case KEY_ON:
				if (work_mode)
					work_mode = 0;
				else
					work_mode = 1;
			break;
			
			case KEY_PUMPP:
				if (work_mode)
					{
					if (pump521_flag)
						pump521_flag = 0;
					else
						pump521_flag = 1;
					}
			break;
			
			case KEY_RUN____LENTA:
				if (work_mode)
					{
					if (run5451_flag)
						run5451_flag = 0;
					else
						run5451_flag = 1;
					}
			break;
			
			case KEY_REVERS_LENTA:
				run5451_flag = 0;
				//PORTB &=~(1 << 2);
			break;
			}
		key_released = 0xFF;
		
		}
	if (key_released_long!=0xFF)
		{
		switch (key_released_long)
			{
			case KEY_ON:
				if (work_mode)
					work_mode = 0;
				else
					work_mode = 1;
			break;
			
			case KEY_REVERS_LENTA:
				run5451_flag = 0;
			break;
			}
		key_released_long = 0xFF;
		}
		
	if (key_presssed!=0xFF)
		{
		switch (key_presssed)
			{
			case KEY_REVERS_LENTA:
				run5451_flag = 2; // реверс ВКЛ
			break;
			}
		key_presssed = 0xFF;
		}		
	if (key_presssed_long!=0xFF)
		{
		switch (key_presssed_long)
			{
			case KEY_REVERS_LENTA:
				;//run5451_flag = 2; // реверс ВКЛ
			break;
			}
		key_presssed_long = 0xFF;
		}	
		
		
		
///////////////////////////////////////////////////////////////////////
// Логика	
	if (work_mode)
		{
		
		if ((sec>>1)&0x01)
			WORK_LED_PORT |= (1 << WORK_LED_PIN);
		else
			{if (error >= 10) WORK_LED_PORT &=~(1 << WORK_LED_PIN); else WORK_LED_PORT |= (1 << WORK_LED_PIN);}
		}
	else
		{
		WORK_LED_PORT&=~(1 << WORK_LED_PIN);
		// выкл ВСЁ
		run5451_flag = 0;
		pump521_flag = 0;
		pwn_now = 0;
		
		OFF_IPS521_1;
		OFF_IPS521_2;
		OFF_IPS521_3;
		
		error = 0;
		error_IPS521_1 = 0;
		error_IPS521_2 = 0;
		error_IPS521_3 = 0;
		
		state_IPS521_1 = 0;
		state_IPS521_2 = 0;
		state_IPS521_3 = 0;
		
		IPS_ON_counter[0] = IPS_ON_counter[1] = IPS_ON_counter[2] = 0;
		// .. 

		IPS_521_3_DDR&=~ (1 << IPS_521_3_PIN_DG);
		IPS_521_3_PORT|= (1 << IPS_521_3_PIN_DG); // pullup
// init_ports end		
		}
		
		
	if (run5451_flag == 1) // нормальный режим работы
		{
		// ВКЛ IPS521_1
		if (error_IPS521_1|error_IPS521_2)
			;//{OFF_IPS521_1;OFF_IPS521_2;}
		else
			{		
			state_IPS521_1 = 1;//ON__IPS521_1;
			}		
		//PORTB |= (1 << 2); // индикация
		;//pwn_now = (rez >> 2);
		}
	else if (run5451_flag == 2) //revers start
		{
		if ((pwn_now == 1) || (pwn_now == 0) ) // второе услови для реверса без вкл прямого
			{cli (); pwn_now = 0; sei();
			run5451_flag = 3;
			state_IPS521_1 = 0;//OFF_IPS521_1;// ВЫКЛ прямую IPS
			}
		//if (pwn_now == 0)
			//PORTB |= (1 << 2); ВКЛ реверс
		}
	else if (run5451_flag == 3) //revers run
		{
		//if (error_IPS521_1|error_IPS521_2)
			;//{OFF_IPS521_1;OFF_IPS521_2;}
		//else
			{		
			//PORTB |= (1 << 2); //ВКЛ реверс
			state_IPS521_2 = 1;//ON__IPS521_2;
			}

		}
	else // ВЫКЛ и всё прочее
		{
		state_IPS521_1 = 0; // OFF_IPS521_1;
		state_IPS521_2 = 0;//OFF_IPS521_2;
		//pwn_now = 0;
		}
// индикаия  этого режима
	if (run5451_flag == 1)
		{
		if (error_IPS521_1)
			{
			if ((sec>>1)&0x01)
				PORTB |= (1 << 2);
			else
				PORTB &=~(1 << 2);
			}
		else
			PORTB |= (1 << 2);
		}
	else if (run5451_flag == 3)
		{
		if (error_IPS521_2)
			{
			if ((sec>>1)&0x01)
				PORTB |= (1 << 2);
			else
				PORTB &=~(1 << 2);
			}
		else
			PORTB |= (1 << 2);
		}	
	else
		{
		PORTB &=~(1 << 2);
		}
	
	
		
		
//// PUMP	
	if (pump521_flag)
		{
		if (state_IPS521_3 == 0) delay_5451 = 2;
		state_IPS521_3 = 1;
		}
	else
		{state_IPS521_3 = 0;}	

	if (pump521_flag)
		{
		if ((sec>>1)&0x01)
			PORTB |= (1 << 1);
		else
			{if (error_IPS521_3) PORTB &=~(1 << 1); else PORTB |= (1 << 1);}
		}
	else
		{
		PORTB &=~(1 << 1);
		}
///////////////////////////////////////////////////////////////////////

	// чтение ADC 
	adc_convert ();
	/*if (rez < 512)
		PORTB |= (1 << 2);
	else
		PORTB &=~(1 << 2);
	//*/
	}
}


void init_ADC (void)
{
	PORTC&=~((1 << 0));
	DDRC&=~((1 << 0));
	ADMUX  = 0;//((1 << ADLAR));
	//ADMUX= (  (1 << MUX0)); 
	ADCSRA = ((1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0));
}

void adc_convert (void)
{
ADCSRA|= (1<<ADSC);
while (!(ADCSRA& (1 << ADIF) ))
	;
	
rez = ADC;
if (rez > 1023) rez = 1023;				
}