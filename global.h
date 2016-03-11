#ifndef _FLOBAL_H_
	#define _FLOBAL_H_
	#define WORK_LED_PORT	PORTB
	#define WORK_LED_DDR	DDRB
	#define WORK_LED_PIN	PB0

	#define RUN_LED_PORT	PORTB
	#define RUN_LED_DDR	DDRB
	#define RUN_LED_PIN	PB1

	#define SINGLE_LED_PORT	PORTB
	#define SINGLE_LED_DDR	DDRB
	#define SINGLE_LED_PIN	PB2

	#define IPS_5451_DDR	DDRC
	#define IPS_5451_PORT	PORTC
	#define IPS_5451_PINx	PINC
	#define IPS_5451_PIN	PC3
	#define IPS_5451_PIN_DG	PC2

	/*#define IPS_521_1_DDR	DDRB
	#define IPS_521_1_PORT	PORTB
	#define IPS_521_1_PINx	PINB
	#define IPS_521_1_PIN	PB3	
	#define IPS_521_1_PIN_DG	PB4*/
	#define IPS_521_3_DDR	DDRB
	#define IPS_521_3_PORT	PORTB
	#define IPS_521_3_PINx	PINB
	#define IPS_521_3_PIN	PB3	
	#define IPS_521_3_PIN_DG	PB4

	
	//
	#define IPS_521_2_DDR	DDRD
	#define IPS_521_2_PORT	PORTD
	#define IPS_521_2_PINx	PIND
	#define IPS_521_2_PIN	PD2	
	#define IPS_521_2_PIN_DG	PD0	

            
/*	#define IPS_521_3_DDR	DDRC
	#define IPS_521_3_PORT	PORTC
	#define IPS_521_3_PINx	PINC
	#define IPS_521_3_PIN	PC5	
	#define IPS_521_3_PIN_DG	PC4*/
	#define IPS_521_1_DDR	DDRC
	#define IPS_521_1_PORT	PORTC
	#define IPS_521_1_PINx	PINC
	#define IPS_521_1_PIN	PC5	
	#define IPS_521_1_PIN_DG	PC4	
	               
	#define ON__IPS521_1	{IPS_521_1_PORT|=(1<<IPS_521_1_PIN);}
	#define OFF_IPS521_1	{IPS_521_1_PORT&=~(1<<IPS_521_1_PIN);}
	
	#define ON__IPS521_2	{IPS_521_2_PORT|= (1<<IPS_521_2_PIN);}
	#define OFF_IPS521_2	{IPS_521_2_PORT&=~(1<<IPS_521_2_PIN);}

	#define ON__IPS521_3	{IPS_521_3_PORT|= (1<<IPS_521_3_PIN);}
	#define OFF_IPS521_3	{IPS_521_3_PORT&=~(1<<IPS_521_3_PIN);}	
	
	#define IS_IPS521_1_ON	bit_is_set (IPS_521_1_PINx, IPS_521_1_PIN)
	#define IS_IPS521_1_ERROR	bit_is_clear (IPS_5451_PINx, IPS_521_1_PIN_DG)
	#define IS_IPS521_1_HIGH	bit_is_set (IPS_5451_PINx, IPS_521_1_PIN_DG)
#endif