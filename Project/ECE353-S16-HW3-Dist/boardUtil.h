#include "gpio_port.h"
#include "adc.h"
#include "timers.h"
#include "interrupts.h"
#include "eeprom.h"
#include "pc_buffer.h"
#include "uart_irqs.h"
#include "lcd.h"
#include "wireless.h"

/* Parameters */
#define DEBUG     1  // 0 = no debug msgs  1 = debug msgs
#define WATCHDOG  0  // 0 = watch off      1 = watchdog on
/* End Parameters */

#define LED_I2C  I2C1_BASE
#define NAMELEN  81
#define HSADDR  (2+(2*NAMELEN))
#define PINGPACKET 0xA5

#if DEBUG
# define Dprintf printf
#else
# define Dprintf(FORMAT, ...) ((void)0)
#endif

#if WATCHDOG
# define PETWATCHDOG() (WATCHDOG0->LOAD = SEC5s_TIME * 2)
#else
# define PETWATCHDOG() ;
#endif

extern uint8_t myID[];
extern uint8_t remoteID[];

extern char ryan[];
extern char nick[];
extern char team[3];
extern char person1[NAMELEN];
extern char person2[NAMELEN];
extern uint8_t highscore;

i2c_status_t ledSendData(uint8_t addr, uint8_t data);
void initialize_board(void);

