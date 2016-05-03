#include <string.h>
#include "boardUtil.h"

PC_Buffer UART0_Rx_Buffer;
PC_Buffer UART0_Tx_Buffer;

uint8_t myID[]      = { '3', '5', '3', '7', '1'};
uint8_t remoteID[]  = { '3', '5', '3', '7', '0'};

void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

void i2c_init(void)
{
	gpio_enable_port(EEPROM_GPIO_BASE);

	// Configure SCL 
	gpio_config_digital_enable(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN);
	gpio_config_alternate_function(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN);
	gpio_config_port_control(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PCTL_M, EEPROM_I2C_SCL_PIN_PCTL);

	// Configure SDA 
	gpio_config_digital_enable(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
	gpio_config_open_drain(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
	gpio_config_alternate_function(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
	gpio_config_port_control(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PCTL_M, EEPROM_I2C_SDA_PIN_PCTL);

	initializeI2CMaster(EEPROM_I2C_BASE);
}

void serial_debug_init(void)
{
	// Configure the UART for Debug
	gpio_enable_port(GPIOA_BASE);
	gpio_config_digital_enable(GPIOA_BASE, PA0 | PA1);
	gpio_config_alternate_function(GPIOA_BASE, PA0 | PA1);
	gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA0_M, GPIO_PCTL_PA0_U0RX);
	gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA1_M, GPIO_PCTL_PA1_U0TX);

	initialize_uart();
}

void watchdog_init(void)
{
	SYSCTL->RCGCWD |= SYSCTL_RCGCWD_R0;
	while ( !(SYSCTL->PRWD & SYSCTL_PRWD_R0) )
		;

	PETWATCHDOG();
	WATCHDOG0->CTL |= 1;
	
	NVIC_SetPriority(WATCHDOG0_IRQn, 0);
	NVIC_EnableIRQ(WATCHDOG0_IRQn);
}

i2c_status_t ledSendData(uint8_t addr, uint8_t data) {
  i2c_status_t status;
	
	while ( I2CMasterBusy(LED_I2C)) {};
	
	status = i2cSetSlaveAddr(LED_I2C, 0x20, I2C_WRITE);
	if (status != I2C_OK)
		return status;
	
	i2cSendByte(LED_I2C, addr, I2C_MCS_START | I2C_MCS_RUN);
	i2cSendByte(LED_I2C, data, I2C_MCS_RUN | I2C_MCS_STOP);
	return status;
}

void led_init(void) {
	ledSendData(0x0, 0);
	ledSendData(0x1, 0);
}

void eeprom_init() {
	int i, len;
	eeprom_byte_read(EEPROM_I2C_BASE, 0, (uint8_t *)&team[0]);
	eeprom_byte_read(EEPROM_I2C_BASE, 1, (uint8_t *)&team[1]);
	
	if (team[0] != '1' || team[1] != '8') {
		eeprom_byte_write(EEPROM_I2C_BASE, 0, '1');
		eeprom_byte_write(EEPROM_I2C_BASE, 1, '8');
		
		len = strlen(ryan) + 1;
		for (i = 0; i < len; i++) {
			eeprom_byte_write(EEPROM_I2C_BASE, i+2, ryan[i]);
		}
		
		len = strlen(nick) + 1;
		for (i = 0; i < len; i++) {
			eeprom_byte_write(EEPROM_I2C_BASE, i+NAMELEN+2, nick[i]);
		}
	}
	
	eeprom_byte_read(EEPROM_I2C_BASE, 0, (uint8_t *)&team[0]);
	eeprom_byte_read(EEPROM_I2C_BASE, 1, (uint8_t *)&team[1]);
	eeprom_byte_read(EEPROM_I2C_BASE, HSADDR, &highscore);
	
	for (i = 0; i < NAMELEN; i++) {
		eeprom_byte_read(EEPROM_I2C_BASE, i+2, (uint8_t *)&person1[i]);
		if (person1[i] == 0) break;
	}
	
	for (i = 0; i < NAMELEN; i++) {
		eeprom_byte_read(EEPROM_I2C_BASE, i+NAMELEN+2, (uint8_t *)&person2[i]);
		if (person2[i] == 0) break;
	}
	
  printf("\n\r");
  printf("Student 1: %s\n\r", person1);
  printf("Student 2: %s\n\r", person2);
  printf("Team Number: %s\n\r", team);
  printf("Last Highscore: %d\n\r", highscore);
}

void initialize_board(void)
{
  DisableInterrupts();
	
	serial_debug_init();
	
	hw3_gpio_init();
	hw3_adc_init();
	
	SysTick_Config(SEC5ms_TIME);
	hw3_timer0_init();
	hw3_timer1_init();
	hw3_timer2_init();
	
	i2c_init();
	led_init();
	
	lcd_pins_init();
	lcd_screen_init();
	
	wireless_initialize();
	wireless_configure_device(myID, remoteID);
	
#if WATCHDOG
	watchdog_init();
#endif

	eeprom_init();
	
  EnableInterrupts();
}
