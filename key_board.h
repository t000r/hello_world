#include <avr/io.h>

void key_board_init (void);
void key_board_scan (void);
void key_board_reset (void);



// new CV
#define KEY_ON	0
#define KEY_PUMPP	1
#define KEY_RUN____LENTA	2
#define KEY_REVERS_LENTA	3

volatile unsigned char key_presssed;
extern volatile unsigned char key_released;
extern volatile unsigned char key_presssed_long;
extern volatile unsigned char key_released_long;