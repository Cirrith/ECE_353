#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <stdbool.h>
#include <stdint.h>

#include "driver_defines.h"

#define SEC5s_TIME			250000000
#define SEC2s_TIME			100000000
#define SEC5ms_TIME			250000
#define SECPT5ms_TIME		25000

//*****************************************************************************
// Configure a general purpose timer to be a 32-bit timer.  
//
// Paramters
//  base_address          The base address of a general purpose timer
//
//  mode                  bit mask for Periodic, One-Shot, or Capture
//
//  count_up              When true, the timer counts up.  When false, it counts
//                        down
//
//  enable_interrupts     When set to true, the timer generates and interrupt
//                        when the timer expires.  When set to false, the timer
//                        does not generate interrupts.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_config_32(uint32_t base_addr, uint32_t mode, bool count_up, bool enable_interrupts);


//*****************************************************************************
// Waits for 'ticks' number of clock cycles and then returns.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_wait(uint32_t base_addr, uint32_t ticks);

bool hw3_timer_enable(uint32_t, uint32_t);
void hw3_timer0_init(void);
void hw3_timer1_init(void);
void hw3_timer2_init(void);
#endif
