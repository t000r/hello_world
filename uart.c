#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/signal.h>

//#include <avr/interrupt.h>

#define BV(bit)			(1<<(bit))
#define cbi(reg,bit)	reg &= ~(BV(bit))
#define sbi(reg,bit)	reg |= (BV(bit))



/* set baud rate here */
#define UART_BAUD_RATE 9600

#define F_CPU 16000000

//иписание адресной команды nnnn aaaa 
#define slave_adres	3

/* automatically calcuate baud register value */
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)


#define BV(bit)			(1<<(bit))
#define cbi(reg,bit)	reg &= ~(BV(bit))
#define sbi(reg,bit)	reg |= (BV(bit))




#define MESEGE_SIZE  16
volatile unsigned char uart_tx_mesege[MESEGE_SIZE];
volatile unsigned char uart_rx_mesege[MESEGE_SIZE];

volatile unsigned char count_rx;
volatile unsigned char crc_rx;
volatile unsigned char count_tx;
volatile unsigned char crc_tx;

volatile unsigned char incoming_mes;  //флаг указывающий входяще сообщение получено
volatile unsigned char outcoming_mes;  //флаг указывающий сообщение готово к отправке



volatile unsigned char *rx_ptr;
volatile unsigned char *tx_ptr;

volatile unsigned char delay_rx;

void delay(unsigned int pause)  
{
unsigned int i;
for(i=0;i<pause;i++) {}  
}

void uart_init(void)
/* initialize uart */
{	
	//при5мопередатчик 9 бит в режиме мультипроцессорного общения
	sbi(PORTD,2);
	sbi(DDRD,2);
	delay(100);
	cbi(PORTD,2);
	

  /* set baud rate */
	UBRRH=(unsigned char)(UART_BAUD_SELECT>>8);
	UBRRL=(unsigned char)UART_BAUD_SELECT;


	UCSRA= _BV(MPCM);
	UCSRB=(1<<RXCIE)|(1<<RXEN)|(1<<TXEN)|(1<<UCSZ2);       
	UCSRC = (1<<URSEL)|(3<<UCSZ0);

	delay(100);
	sbi(PORTD,2);

 
}


void start_lin_trans(void){
//	while (bit_is_set(UCSRB, UDRIE));
	count_tx=0xff;
	tx_ptr=uart_tx_mesege;  //указатель устанавливаем на запись байта в начало массива
	crc_tx=0;
//	cbi(UCSRB, RXEN);
	sbi(UCSRB, UDRIE);
	
}



SIGNAL(SIG_UART_DATA)      
/* signal handler for uart data buffer empty interrupt */

{register char out_byte=*tx_ptr;
	if (count_tx==0) {UDR=crc_tx;
						cbi(UCSRB, UDRIE);		//передача контрольной суммы и конец сообщения
//						sbi(UCSRB, RXEN);
						goto exit;}
	crc_tx+=out_byte;
	count_tx--;
	tx_ptr++;
	

	if (count_tx==0xfe) {	sbi(UCSRB, TXB8);count_tx=(out_byte>>4);  //передача 
								} else cbi(UCSRB, TXB8);

UDR=out_byte;

exit:;
	
}



void wait_new_mes(void)
/* после обработки сообщения переводим контроллер в режим ожидания нового сообщения */
{	incoming_mes=0; /*res_flag=0;*/
	UCSRA= _BV(MPCM);
//	sbi(UCSRB, RXEN);   
	delay_rx=0;
	count_rx=0;
}


SIGNAL(SIG_UART_RECV)      
/* signal handler for receive complete interrupt */
{ register char in_byte=UDR;
//	if ((in_byte&0x0f)==slave_adres) 	PORTB^=1;
	if (count_rx) {count_rx--;
					if (count_rx==0) {if (crc_rx==in_byte) {incoming_mes=1;
															//cbi(UCSRB, RXEN);
															goto exit;};}
					crc_rx+=in_byte;										
					*rx_ptr=in_byte;
					delay_rx=100;
					rx_ptr++;
					} else 
	{if ((in_byte&0x0f)!=slave_adres) goto exit;
	UCSRA=0; //выключаем режим определения адреса
	count_rx=(in_byte>>4)+1;
	crc_rx=in_byte;
	rx_ptr=uart_rx_mesege; //указатель устанавливаем на запись байта в начало массива
	*rx_ptr=in_byte;
	rx_ptr++;
	};
exit:;		
}
