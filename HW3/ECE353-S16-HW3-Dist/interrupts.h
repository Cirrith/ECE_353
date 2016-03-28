#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdbool.h>
#include "driver_defines.h"
#include "TM4C123.h"

extern volatile bool ALERT_DEBOUNCE;
extern volatile bool ALERT_2_SEC;

void TIMER0A_Handler(void);
void TIMER1A_Handler(void);
void TIMER0B_Handler(void);
void TIMER1B_Handler(void);

#endif
