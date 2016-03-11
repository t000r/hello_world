#include <avr/io.h>

#define wait_cycle 10000

unsigned char i2serror=0;

void i2sinit(void)								/* выполняется в начале, в First.c */
{

  TWBR = (16000000UL / 50000UL - 16) / 8;		/* частота TWI clock 100000 kHz  for mega32*/

  PORTC|= ((1<<PC4)|(1<<PC5));
  DDRC &=~((1<<PC4)|(1<<PC5));
}


void cheek_busy(void){
  unsigned int exit_time = wait_cycle;

  while (((TWCR & _BV(TWINT)) == 0)&(--exit_time!=0)) /* exit_time--*/;
  if (exit_time==0)   { TWCR = 0 ;i2serror=1;};
}

void i2swrite(uint8_t i2s_byte)	/* запись байта в расширитель */
{ 
  TWDR = i2s_byte; 
  TWCR = _BV(TWINT) | _BV(TWEN); 					/* выполнение, clear interrupt to start transmission */
  cheek_busy();
}

void i2sstop(void)								/* выполняется после обновлением страницы */
{ 
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); 		/* send stop condition */
}

void i2sstart(void)								    /* выполняется перед обновлением страницы */
{
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); 		/* Старт, TWSTA - мастер, TWEN - активизация интерфейса */
  cheek_busy();
  
}


unsigned char i2sread (void)
{
unsigned char x;
	TWCR = ((1<<TWINT)+(1<<TWEN));
	cheek_busy();
	x=TWDR;
	return(x);
}



void at24c64_write (unsigned int  adr,unsigned char data){
i2sstart();
i2swrite(0xa0);
i2swrite((adr>>8)&0x1F);
i2swrite(adr);
i2swrite(data);
i2sstop();

cheek_busy();
cheek_busy();
}


unsigned char at24c64_read (unsigned int  adr) {
unsigned char x=0;
i2sstart();
i2swrite(0xa0);
i2swrite((adr>>8)&0x1F);
i2swrite((unsigned char)adr);
i2sstop();
i2sstart();
i2swrite(0xa1);
x=i2sread();
i2sstop();
return(x);
}
