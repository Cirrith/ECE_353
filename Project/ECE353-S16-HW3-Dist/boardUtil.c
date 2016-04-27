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

	//  Initialize the I2C peripheral
	initializeI2CMaster(EEPROM_I2C_BASE);
	// TODO LED Matrix
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

void initialize_board(void)
{
  DisableInterrupts();
	
	serial_debug_init();
	
	hw3_gpio_init();
	hw3_adc_init();
	hw3_timer0_init();
	hw3_timer1_init();
	
	i2c_init();
	
	lcd_pins_init();
	lcd_screen_init();
	
	wireless_initialize();
	wireless_configure_device(myID, remoteID);
	
  EnableInterrupts();
}
