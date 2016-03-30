#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <stdbool.h>
#include <stdint.h>

#include "driver_defines.h"

#define SEC2s_TIME			100000000
#define SEC5ms_TIME			250000

bool hw3_timer_enable(uint32_t, uint32_t);
void hw3_timer0_init(void);
void hw3_timer1_init(void);

#endif
