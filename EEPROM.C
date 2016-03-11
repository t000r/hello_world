/* EEPROM - адресное пространство	*/
#include <avr/eeprom.h> 

#include "EEP.h"
//значения записываются в виде пробег/8 c последующим увеличением на 1



volatile unsigned char eeprom_busy=0;
extern volatile unsigned char flag_clear_eep_stat;

long Read_probeg(unsigned int addr)  //адрес смещения пробега
{eeprom_busy=1;
unsigned int probeg[8*2];
unsigned int max_probeg;
eeprom_read_block(&probeg[0],(void*)addr,16*2);
//unsigned long z;
max_probeg=0x0;
unsigned char n;
for (n=0;n<(8*2);n++) {if (probeg[n]==0xffff) probeg[n]=0; if (probeg[n]>=max_probeg) max_probeg=probeg[n];};
for (n=(7+8);n!=0;n--) if (probeg[n]==max_probeg) goto exit;;
exit:;
if (max_probeg) max_probeg--; else n=0;

eeprom_busy=0;

return ((long)max_probeg*8*2+n);
}


void Write_probeg(unsigned int addr,long prob)
{
eeprom_busy=1;
 unsigned int sm=(prob>>4)+1;
 unsigned char sm1=prob;sm1&=15;sm1=sm1<<1;
		while (!eeprom_is_ready());
		eeprom_write_word((void*)addr+sm1,sm);
eeprom_busy=0;		
}
void clear_probeg(unsigned int addr){
unsigned char n;
for (n=0;n<(16*2);n+=2)   {eeprom_busy=1;
						while (!eeprom_is_ready());
						eeprom_write_word((void*)addr+n,0);
						eeprom_busy=0;
						};
};



/*void clear_temp_info(void){
eeprom_busy=1;
unsigned char n;
for (n=0;n<16;n+=2) {while (!eeprom_is_ready());eeprom_write_word((void*)ee_hour_comb_w+n,0);};
for (n=0;n<16;n+=2) {while (!eeprom_is_ready());eeprom_write_word((void*)ee_square_w+n,0);};
for (n=0;n<16;n+=2) {while (!eeprom_is_ready());eeprom_write_word((void*)ee_probeg_w+n,0);};
eeprom_busy=0;
}

void clear_all_info(void){
eeprom_busy=1;
unsigned char n;
for (n=0;n<0xe8;n++) {while (!eeprom_is_ready());eeprom_write_word((void*)ee_hour_comb_s+(int)n*2,0);};
eeprom_busy=0;
}
*/

/*
void clear_stat(void){
eeprom_busy=1;
unsigned char n;
for (n=0;n<14*8;n++) {while (!eeprom_is_ready());eeprom_write_word((void*)ee_statistic+(int)n*2,0);};
eeprom_busy=0;
}*/

#define good_info 0x8




void test_eep (unsigned int addres){
unsigned char dat0,dat1,dat2;
while (!eeprom_is_ready());
dat0=eeprom_read_byte((uint8_t *)addres);
dat1=eeprom_read_byte((uint8_t *)(addres+2));
dat2=eeprom_read_byte((uint8_t *)(addres+4));

if((dat0==dat1)&(dat0!=dat2)) {while (!eeprom_is_ready());eeprom_write_byte((uint8_t *)(addres+4),dat0);};
if((dat0==dat2)&(dat0!=dat1)) {while (!eeprom_is_ready());eeprom_write_byte((uint8_t *)(addres+2),dat0);};
if((dat1==dat2)&(dat0!=dat1)) {while (!eeprom_is_ready());eeprom_write_byte((uint8_t *)addres,dat1);};

}


void eep_wr_byte (unsigned int addres,unsigned char dat)
{
eeprom_busy=1;
while (!eeprom_is_ready());  eeprom_write_byte((uint8_t *)addres,dat);
eeprom_busy=0;
}

void eep_wr_word (unsigned int addres,unsigned int dat)
{
eep_wr_byte(addres+1,dat>>8);eep_wr_byte(addres,dat);
}


void eep_wr_dword (unsigned int addres,unsigned long dat)
{
eep_wr_word(addres+2,dat>>16);eep_wr_word(addres,dat);
}



void eep_wr (unsigned int addres,unsigned char dat){
eeprom_busy=1;

while (!eeprom_is_ready());eeprom_write_byte((uint8_t *)addres,dat);
while (!eeprom_is_ready());eeprom_write_byte((uint8_t *)(addres+2),dat);
while (!eeprom_is_ready());eeprom_write_byte((uint8_t *)(addres+4),dat);
eeprom_busy=0;
}

void eep_word (unsigned int addres,unsigned int dat){
eep_wr(addres+1,dat>>8);eep_wr(addres,dat);
}

void eep_dword (unsigned int addres,unsigned long dat){
eep_word (addres,dat);
eep_word (addres+8,dat>>16);
//eep_wr(addres+9,dat>>24);eep_wr(addres+8,dat>>16);eep_wr(addres+1,dat>>8);eep_wr(addres,dat);
}



unsigned char eep_r_byte (unsigned int addres)
{
eeprom_busy=1;
while (!eeprom_is_ready()); unsigned char n=eeprom_read_byte((uint8_t *)addres);
eeprom_busy=0;
return(n);
}

unsigned int eep_r_word (unsigned int addres)
{
return((((int)eep_r_byte(addres+1))<<8)+eep_r_byte(addres));
}

/*
unsigned long eep_r_dword (unsigned int addres)
{
return((((long)eep_r_word(addres+2))<<16)+eep_r_word(addres));
}
*/



unsigned char read_byte (unsigned int addres)
{
eeprom_busy=1;
test_eep (addres);
unsigned char n=eeprom_read_byte((uint8_t *)addres);
eeprom_busy=0;
return(n);
}

unsigned int read_word (unsigned int addres)
{
return((((int)read_byte(addres+1))<<8)+read_byte(addres));
}


unsigned long read_dword (unsigned int addres){
return((((long)read_word(addres+8))<<16)+read_word(addres));
};


unsigned int read_config(unsigned int addres,unsigned int max,unsigned int undo){
unsigned int x=read_word(addres);if (x>max) {x=undo;eep_word(addres,x);};
return(x);
}

unsigned long read_config_d(unsigned int addres,unsigned long max,unsigned long undo){
unsigned long x=read_dword(addres);if (x>max) {x=undo;eep_dword(addres,x);};
return(x);
}

#define EE_rulon_number				0x40
#define EE_rulon_number_all			EE_rulon_number+0x20
#define EE_mode						EE_rulon_number_all+0x20
#define EE_rulon_number_auto		EE_mode+0x20

#define EE_delay_before_IPS_ON		0x100
#define EE_IPS_ON_pulse				EE_delay_before_IPS_ON+6
#define EE_IPS_ON_max_time			EE_IPS_ON_pulse+6
#define EE_IPS_test_delay			EE_IPS_ON_max_time+6
#define EE_IPS_test_time			EE_IPS_test_delay+6
#define EE_cut_time					EE_IPS_test_time+6
#define EE_terminal_sensor_time		EE_cut_time+6
#define EE_terminal_type			EE_terminal_sensor_time+6

#define EE_obvya	EE_terminal_type+6
#define EE_serial_number	EE_obvya+6
#define EE_product_date	EE_serial_number+6


#define EE_IPS1_ON_pulse	EE_product_date+6
#define EE_IPS1_ON_max_time	EE_IPS1_ON_pulse+6
#define EE_IPS1_test_delay	EE_IPS1_ON_max_time+6
#define EE_IPS1_test_time	EE_IPS1_test_delay+6



#define DEFAULT_delay_before_IPS_ON		1
#define DEFAULT_IPS_ON_pulse			34
#define DEFAULT_IPS_ON_max_time			11
#define DEFAULT_IPS_test_delay			24
#define DEFAULT_IPS_test_time			16
#define DEFAULT_cut_time				8
#define DEFAULT_terminal_sensor_time	2	
#define DEFAULT_terminal_type			0xB0 // нормально замкнуты	(на землю)

#define DEFAULT_IPS1_ON_pulse			35
#define DEFAULT_IPS1_ON_max_time		20
#define DEFAULT_IPS1_test_delay			5
#define DEFAULT_IPS1_test_time			9



extern unsigned char delay_before_IPS_ON; 	// задержка перед началом подачи шпагата/сетки 
extern unsigned char IPS_ON_pulse;			// число импульсов включенной IPS ++
extern unsigned char IPS_ON_max_time;		// максимальное время включенной IPS ++
extern unsigned char IPS_test_delay;		// время задержики перед началом тестирования IPS ++
extern unsigned char IPS_test_time;			// время теста IPS
extern unsigned char cut_time;				// время обрезки шпагата
extern unsigned char terminal_sensor_time;	// время на срабатываение концевика
extern unsigned char terminal_type;

extern unsigned char IPS1_ON_pulse;			// число импульсов включенной IPS1
extern unsigned char IPS1_ON_max_time;		// максимальное время включенной IPS1
extern unsigned char IPS1_test_delay;		// задержка перед началом старта тестирования КЗ
extern unsigned char IPS1_test_time;		// время теста IPS1

extern unsigned char mode;// AUTO- 0; 
extern unsigned char obvya;// старшие 4 биты 				0 - сетка; 		1 -  шпагат
						// младие 4 бита выбор режима сетки - 1-9

extern unsigned int rulon_number;
extern unsigned int rulon_number_all;

extern unsigned int serial_number;
extern unsigned int product_date;

void clear_eep_stat(void)
{
for (int i=0;i<=(SIZE_STAT*MAX_NUM_OF_STAT);i++)
	{at24c64_write ((uint8_t *)i,0xFF);	}
	
for (int i=EE_STAT_COUNT;i<(EE_STAT_COUNT+0x10);i++)
	{while (!eeprom_is_ready());  eeprom_write_byte((uint8_t *)i,0xFF);}	
}


void read_configuration(void)
{

/*for (int i=0;i<(EE_STAT_COUNT+SIZE_STAT*MAX_NUM_OF_STAT+0x10);i++)
	{while (!eeprom_is_ready());  eeprom_write_byte((uint8_t *)i,255);}*/
if (  (read_word(EE_IPS_ON_pulse) > 200) && (read_word(EE_IPS_ON_max_time) > 200) )
	flag_clear_eep_stat=1; // чистим память по 1-му вкл	
	
delay_before_IPS_ON		=read_config(EE_delay_before_IPS_ON, 		10,  DEFAULT_delay_before_IPS_ON);
IPS_ON_pulse       		=read_config(EE_IPS_ON_pulse,        		200, DEFAULT_IPS_ON_pulse);
IPS_ON_max_time       	=read_config(EE_IPS_ON_max_time,			200, DEFAULT_IPS_ON_max_time);
IPS_test_delay       	=read_config(EE_IPS_test_delay,				200, DEFAULT_IPS_test_delay);

IPS_test_time       	=read_config(EE_IPS_test_time,				200, DEFAULT_IPS_test_time);
cut_time       			=read_config(EE_cut_time,					200, DEFAULT_cut_time);
terminal_sensor_time    =read_config(EE_terminal_sensor_time,		200, DEFAULT_terminal_sensor_time);
terminal_type    		=read_config(EE_terminal_type,				0xF9, DEFAULT_terminal_type);

IPS1_ON_pulse			=read_config(EE_IPS1_ON_pulse, 				200, DEFAULT_IPS1_ON_pulse);
IPS1_ON_max_time       	=read_config(EE_IPS1_ON_max_time,        	200, DEFAULT_IPS1_ON_max_time);
IPS1_test_delay       	=read_config(EE_IPS1_test_delay,			200, DEFAULT_IPS1_test_delay);
IPS1_test_time       	=read_config(EE_IPS1_test_time,				200, DEFAULT_IPS1_test_time);


mode=Read_probeg(EE_mode)%2;
obvya=read_config(EE_obvya, 		0x20,  0x11);

serial_number			=read_config(EE_serial_number, 		9999,  0);
product_date			=read_config(EE_product_date, 		1230,  812);

rulon_number	=	Read_probeg(EE_rulon_number);
rulon_number_all=	Read_probeg(EE_rulon_number_all);


}


void save_mode(void)
{
Write_probeg(EE_mode,Read_probeg(EE_mode)+1);
//eep_word(EE_mode,	mode);
}

void save_obvya(void)
{
eep_word(EE_obvya,	obvya);
}


void save_configuration(void)
{
eep_word(EE_delay_before_IPS_ON,	delay_before_IPS_ON);
eep_word(EE_IPS_ON_pulse,			IPS_ON_pulse);
eep_word(EE_IPS_ON_max_time,		IPS_ON_max_time);
eep_word(EE_IPS_test_delay,			IPS_test_delay);

eep_word(EE_IPS_test_time,			IPS_test_time);
eep_word(EE_cut_time,				cut_time);
eep_word(EE_terminal_sensor_time,	terminal_sensor_time);
eep_word(EE_terminal_type,			terminal_type);
eep_word(EE_serial_number,			serial_number);
eep_word(EE_product_date,			product_date);

eep_word(EE_IPS1_ON_pulse,			IPS1_ON_pulse);
eep_word(EE_IPS1_ON_max_time,		IPS1_ON_max_time);
eep_word(EE_IPS1_test_delay,		IPS1_test_delay);
eep_word(EE_IPS1_test_time,			IPS1_test_time);

}

void inc_and_save_rulon_number(void)
{
rulon_number++;
rulon_number_all++;
Write_probeg(EE_rulon_number,rulon_number);
Write_probeg(EE_rulon_number_all,rulon_number_all);
if (mode==0) // режим AUTO 
	Write_probeg(EE_rulon_number_auto, Read_probeg(EE_rulon_number_auto)+1);
}




typedef struct 
{
unsigned char min;
unsigned char ch;
unsigned char day;
unsigned char month;

unsigned char error_code;
unsigned char num;
} error_mess;



void save_error (unsigned char error)
{
//error_mess mess;
unsigned int stat_count=eep_r_word(EE_STAT_COUNT);
unsigned char last_error=255;

/*
if (stat_count!=0xFF) // были записи ранее
	{
	last_error=(eep_r_byte(EE_STAT_ERR_CODE+SIZE_STAT*stat_count));
	unsigned char last_err_n=eep_r_byte(EE_STAT_ERR_N+SIZE_STAT*stat_count);
	if (error==last_error) // ошибка повторяется
		{
		eep_wr_byte(EE_STAT_ERR_N + SIZE_STAT*stat_count,  ( (last_err_n!=255) ? (last_err_n+1) :(255) ));
		}
	}
*/	
//if (error!=last_error) // новая ошибка
	{
	stat_count++;
	if (stat_count>=MAX_NUM_OF_STAT)
		{
		stat_count%=MAX_NUM_OF_STAT;
		}
	eep_wr_word(EE_STAT_COUNT,stat_count);
	
	//eep_wr_byte(EE_STAT_CH + SIZE_STAT*stat_count, td.ch);
	at24c64_write (EE_STAT_CH + SIZE_STAT*stat_count, td.ch);
	
	//eep_wr_byte(EE_STAT_MIN + SIZE_STAT*stat_count, td.min);
	at24c64_write(EE_STAT_MIN + SIZE_STAT*stat_count, td.min);
	
	//eep_wr_byte(EE_STAT_DAY + SIZE_STAT*stat_count, td.day);
	at24c64_write(EE_STAT_DAY + SIZE_STAT*stat_count, td.day);
	
	//eep_wr_byte(EE_STAT_MONTH + SIZE_STAT*stat_count, td.month);
	at24c64_write(EE_STAT_MONTH + SIZE_STAT*stat_count, td.month);
	
	
	
	
	//eep_wr_byte(EE_STAT_ERR_CODE + SIZE_STAT*stat_count, error);
	at24c64_write(EE_STAT_ERR_CODE + SIZE_STAT*stat_count, error);
	
	//eep_wr_byte(EE_STAT_ERR_N + SIZE_STAT*stat_count, 1);
	at24c64_write(EE_STAT_ERR_N + SIZE_STAT*stat_count, 1);
		
		
	}

}

