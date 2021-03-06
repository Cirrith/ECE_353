#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdbool.h>
#include "driver_defines.h"
#include "TM4C123.h"
#include "pc_buffer.h"
#include "uart_irqs.h"

extern volatile bool ALERT_DEBOUNCE;
extern volatile bool ALERT_5_SEC;
extern volatile bool ALERT_2_SEC;
extern volatile bool ALERT_GAME_TICK;
extern volatile bool ALERT_GAME_DRAW;

extern volatile bool ALERT_LED; 

extern volatile bool LEFT;
extern volatile bool RIGHT;
extern volatile bool UP;
extern volatile bool DOWN;

extern volatile bool ALERT_WIRELESS;

void TIMER0A_Handler(void);
void TIMER1A_Handler(void);
void TIMER0B_Handler(void);
void TIMER1B_Handler(void);

#endif
