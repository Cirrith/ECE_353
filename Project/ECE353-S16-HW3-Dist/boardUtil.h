#include "gpio_port.h"
#include "adc.h"
#include "timers.h"
#include "interrupts.h"
#include "eeprom.h"
#include "pc_buffer.h"
#include "uart_irqs.h"
#include "lcd.h"
#include "wireless.h"

#define WATCHDOG  0

#define LED_I2C  I2C1_BASE

#if WATCHDOG
 #define PETWATCHDOG() (WATCHDOG0->LOAD = SEC5s_TIME * 2)
#else
 #define PETWATCHDOG() ;
#endif

extern uint8_t myID[];
extern uint8_t remoteID[];

void ledSetNumber(uint8_t);
void initialize_board(void);

