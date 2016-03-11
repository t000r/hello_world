void save_configuration(void);
void read_configuration(void);

void save_mode(void);
void save_obvya(void);
void inc_and_save_rulon_number(void);

long Read_probeg(unsigned int addr);
void clear_probeg(unsigned int addr);
void clear_eep_stat(void);

unsigned char eep_r_byte (unsigned int addres);


typedef struct d_t
{
unsigned char min;
unsigned char ch;
unsigned char day;
unsigned char month;
} TIME_DATE;// td;



#define MESEGE_SIZE  16
extern volatile unsigned char uart_tx_mesege[MESEGE_SIZE];
extern TIME_DATE td;

#define EE_STAT_COUNT			0x180

#define EE_STAT_CH			0x00
#define EE_STAT_MIN			EE_STAT_CH+1
#define EE_STAT_DAY			EE_STAT_MIN+1
#define EE_STAT_MONTH		EE_STAT_DAY+1
#define EE_STAT_ERR_CODE	EE_STAT_MONTH+1
#define EE_STAT_ERR_N		EE_STAT_ERR_CODE+1

#define SIZE_STAT	6
#define MAX_NUM_OF_STAT	100

