#include <avr/io.h> 
#include <avr/interrupt.h>

#define devide_ms -245//06 //для кварца 16MГц

void init_timer0(void);
void init_timer2(void);

extern volatile unsigned int pwn_now;
extern volatile unsigned int error;
extern volatile unsigned int error_IPS521_1;
extern volatile unsigned int error_IPS521_2;
extern volatile unsigned int error_IPS521_3;
extern volatile unsigned int state_IPS521_1;
extern volatile unsigned int state_IPS521_2;
extern volatile unsigned int state_IPS521_3;
extern volatile unsigned char sec;
extern volatile unsigned char delay_5451;

extern volatile unsigned char IPS_ON_counter [3];