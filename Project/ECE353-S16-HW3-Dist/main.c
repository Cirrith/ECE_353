//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123.h"

#include "boardUtil.h"

extern void hw3_config(void);
extern void UART0_Handler(void);
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

/******************************************************************************
 * Global Variables
 *****************************************************************************/
 #define NAMELEN  81
 
char team[3] = {0, 0, 0};
char person1[NAMELEN];
char person2[NAMELEN];

bool ps2_Buf[5] = {0,0,0,0,0};
bool start_Buf[5] = {0,0,0,0,0};
bool a_Buf[5] = {0,0,0,0,0};
bool b_Buf[5] = {0,0,0,0,0};

/* 
	Push Button Debounce Functions 
  These are following the method described in ICE 14
 */

bool ps2_button_pressed(void)
{
	bool val = GPIOE->DATA & PS2BTN_PIN; 
	
	ps2_Buf[4] = ps2_Buf[3];
	ps2_Buf[3] = ps2_Buf[2];
  ps2_Buf[2] = ps2_Buf[1];
	ps2_Buf[1] = ps2_Buf[0];
	ps2_Buf[0] = val;
	
	if(~(ps2_Buf[0] | ps2_Buf[1] | ps2_Buf[2] | ps2_Buf[3]) & ps2_Buf[4])
		return true;
	else
		return false;
}

bool start_button_pressed (void)
{
	bool val = GPIOF->DATA & UP_PIN; 
	
	start_Buf[4] = start_Buf[3];
	start_Buf[3] = start_Buf[2];
  start_Buf[2] = start_Buf[1];
	start_Buf[1] = start_Buf[0];
	start_Buf[0] = val;
	
	if(~(start_Buf[0] | start_Buf[1] | start_Buf[2] | start_Buf[3]) & start_Buf[4])
		return true;
	else
		return false;
}

bool a_button_pressed (void)
{
	bool val = GPIOF->DATA & LEFT_PIN; 
	
	a_Buf[4] = a_Buf[3];
	a_Buf[3] = a_Buf[2];
  a_Buf[2] = a_Buf[1];
	a_Buf[1] = a_Buf[0];
	a_Buf[0] = val;
	
	if(~(a_Buf[0] | a_Buf[1] | a_Buf[2] | a_Buf[3]) & a_Buf[4])
		return true;
	else
		return false;
}

bool b_button_pressed (void)
{
	bool val = GPIOF->DATA & RIGHT_PIN; 
	
	b_Buf[4] = b_Buf[3];
	b_Buf[3] = b_Buf[2];
  b_Buf[2] = b_Buf[1];
	b_Buf[1] = b_Buf[0];
	b_Buf[0] = val;
	
	if(~(b_Buf[0] | b_Buf[1] | b_Buf[2] | b_Buf[3]) & b_Buf[4])
		return true;
	else
		return false;
}

typedef enum
{
		STATE_IDLE,
		STATE_UP1,
		STATE_UP2,
		STATE_DOWN1,
		STATE_DOWN2,
		STATE_LEFT1,
		STATE_RIGHT1,
		STATE_LEFT2,
		STATE_RIGHT2,
		STATE_B,
		STATE_A,
		STATE_START,
		STATE_ERROR
}State;

int 
main(void)
{
	char ryan[] = "Ryan Bambrough";
	char nick[] = "Nick Adams";
	char input[NAMELEN];
	
	State Current_State = STATE_IDLE;
	State Next_State = STATE_IDLE;
	bool a_press = false;
	bool b_press = false;
	bool start_press = false;
	bool ps2_press = false;
	
	int i,j;
	uint32_t data;
  
	initialize_board();
  lcd_clear();
  for(i = 0; i < 7; i++)
  {
    
    lcd_set_page(i+1);
    for(j=0; j<56; j++)
    {
      lcd_set_column(j+20);
      lcd_write_data(image[i*56 + j]);
    }
  }
  
	eeprom_byte_read(EEPROM_I2C_BASE, 0, (uint8_t *)&team[0]);
	eeprom_byte_read(EEPROM_I2C_BASE, 1, (uint8_t *)&team[1]);
	
	if (team[0] != '1' || team[1] != '8') {
		eeprom_byte_write(EEPROM_I2C_BASE, 0, '1');
		eeprom_byte_write(EEPROM_I2C_BASE, 1, '8');
		
		j = strlen(ryan)+1;
		for (i = 0; i < j; i++) {
			eeprom_byte_write(EEPROM_I2C_BASE, i+2, ryan[i]);
		}
		
		j = strlen(nick)+1;
		for (i = 0; i < j; i++) {
			eeprom_byte_write(EEPROM_I2C_BASE, i+NAMELEN+2, nick[i]);
		}
	}
	
	eeprom_byte_read(EEPROM_I2C_BASE, 0, (uint8_t *)&team[0]);
	eeprom_byte_read(EEPROM_I2C_BASE, 1, (uint8_t *)&team[1]);
	
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
	
	//wireless_test();
	//test_eeprom();
  
  // Infinite Loop
  while(1)
  {	
		Current_State = Next_State;
		
		if (ALERT_WIRELESS) {
			if ( wireless_get_32(false, &data) == NRF24L01_RX_SUCCESS )
			{
					printf("Received: %d\n\r", data);
			}

			//PETWATCHDOG();
			ALERT_WIRELESS = false;
		}
		
		// 5 Sec Timeout, only active if we're in the middle of waiting for the code
		if(ALERT_5_SEC)
		{
			printf("Packets Rx: %d Tx: %d\n\r", wirelessPacketsReceived, wirelessPacketsSent);
			ALERT_5_SEC = false;
		}
		
		// 2 Sec Timeout, only active if we're in the middle of waiting for the code
		if(ALERT_2_SEC)
		{
			if (Current_State != STATE_IDLE && Current_State != STATE_ERROR) {
				// Start over if timeout
				printf("Timeout\n\r");
				Current_State = STATE_IDLE;
			}
			
			ALERT_2_SEC = false;
		}

		// Debounce all the buttons on the timer
		if(ALERT_DEBOUNCE)
		{
			a_press = a_button_pressed();
			b_press = b_button_pressed();
			start_press = start_button_pressed();
			ps2_press = ps2_button_pressed();
			ALERT_DEBOUNCE = false;
		}
		else { // Buttons weren't pressed
			a_press = false;
			b_press = false;
			start_press = false;
			ps2_press = false;
		}
		
		if (ps2_press) {
			#if WATCHDOG
			WATCHDOG0->CTL = 0;
			#endif
			
			//uint32_t data = getADCValue(ADC1_BASE, POT_CHANNEL);
			printf("\n\rStudent 1: %s\n\r", person1);
			printf("Student 2: %s\n\r", person2);
			printf("Team Number: %s\n\r", team);
			
			printf("\n\rEnter Student 1: ");
			scanf("%79[^\n]", input);
			for (i = 0; i < NAMELEN; i++) {
				if (input[i] == '\n' || input[i] == '\r') {
					eeprom_byte_write(EEPROM_I2C_BASE, i+2, 0);
					break;
				}
				
				eeprom_byte_write(EEPROM_I2C_BASE, i+2, input[i]);
				person1[i] = input[i];
				if (person1[i] == 0) break;
			}
			
			printf("Enter Student 2: ");
			scanf("%79[^\n]", input);
			for (i = 0; i < NAMELEN; i++) {
				if (input[i] == '\n' || input[i] == '\r') {
					eeprom_byte_write(EEPROM_I2C_BASE, i+NAMELEN+2, 0);
					break;
				}
				
				eeprom_byte_write(EEPROM_I2C_BASE, i+NAMELEN+2, input[i]);
				person2[i] = input[i];
				if (person2[i] == 0) break;
			}
			
			printf("Enter Team: ");
			scanf("%2[^\n]", input);
			eeprom_byte_write(EEPROM_I2C_BASE, 0, input[0]);
			eeprom_byte_write(EEPROM_I2C_BASE, 1, input[1]);
			team[0] = input[0];
			team[1] = input[1];
			
			// Goodbye!
			NVIC_SystemReset();
		}
		
		switch (Current_State)
		{
		case STATE_IDLE: // Wait for UP, restart the timer once it was
		{			
			if(a_press | b_press | start_press | LEFT | DOWN | RIGHT)
			{
				Next_State = STATE_ERROR;
				LEFT = false;
				DOWN = false;
				RIGHT = false;
			}
			else if (UP)
			{
				Next_State = STATE_UP2;
				UP = false;
				printf("UP\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_UP2: // Wait for UP, restart the timer once it was
		{
			if(a_press | b_press | start_press | LEFT | DOWN | RIGHT)
			{
				Next_State = STATE_ERROR;
				LEFT = false;
				DOWN = false;
				RIGHT = false;
			}
			else if (UP)
			{
				Next_State = STATE_DOWN1;
				UP = false;
				printf("UP\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_UP2;
			}
		break;
		}
		
		case STATE_DOWN1: // Wait for DOWN, restart the timer once it was
		{
			if(a_press | b_press | start_press | LEFT | UP | RIGHT)
			{
				Next_State = STATE_ERROR;
				LEFT = false;
				UP = false;
				RIGHT = false;
			}
			else if (DOWN)
			{
				Next_State = STATE_DOWN2;
				DOWN = false;
				printf("DOWN\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_DOWN1;
			}
		break;
		}
		
		case STATE_DOWN2: // Wait for DOWN, restart the timer once it was
		{
			if(a_press | b_press | start_press | LEFT | UP | RIGHT)
			{
				Next_State = STATE_ERROR;
				LEFT = false;
				UP = false;
				RIGHT = false;
			}
			else if (DOWN)
			{
				Next_State = STATE_LEFT1;
				DOWN = false;
				printf("DOWN\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_DOWN2;
			}
		break;
		}
		
		case STATE_LEFT1: // Wait for LEFT, restart the timer once it was
		{
			hw3_timer2_init();
			if(a_press | b_press | start_press | DOWN | UP | RIGHT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				RIGHT = false;
			}
			else if (LEFT)
			{
				Next_State = STATE_RIGHT1;
				LEFT = false;
				printf("LEFT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_LEFT1;
			}
		break;
		}
		
		case STATE_RIGHT1: // Wait for RIGHT, restart the timer once it was
		{
			if(a_press | b_press | start_press | DOWN | UP | LEFT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
			}
			else if (RIGHT)
			{
				Next_State = STATE_LEFT2;
				RIGHT = false;
				printf("RIGHT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_RIGHT1;
			}
		break;
		}
		
		case STATE_LEFT2: // Wait for LEFT, restart the timer once it was
		{
			if(a_press | b_press | start_press | DOWN | UP | RIGHT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				RIGHT = false;
			}
			else if (LEFT)
			{
				Next_State = STATE_RIGHT2;
				LEFT = false;
				printf("LEFT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_LEFT2;
			}
		break;
		}
		
		case STATE_RIGHT2: // Wait for RIGHT, restart the timer once it was
		{
			if(a_press | b_press | start_press | DOWN | UP | LEFT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
			}
			else if (RIGHT)
			{
				Next_State = STATE_B;
				RIGHT = false;
				printf("RIGHT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_RIGHT2;
			}
		break;
		}
		
		case STATE_B: // Wait for B to be pressed, restart the timer once it was
		{
			if(a_press | start_press | DOWN | UP | LEFT | RIGHT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
				RIGHT = false;
			}
			else if (b_press)
			{
				Next_State = STATE_A;
				printf("B\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_B;
			}
		break;
		}
		
		case STATE_A: // Wait for A to be pressed, restart the timer once it was
		{
			if(b_press | start_press | DOWN | UP | LEFT | RIGHT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
				RIGHT = false;
			}
			else if (a_press)
			{
				Next_State = STATE_START;
				printf("A\n\r");
			  hw3_timer2_init();
			}
			else
			{
				Next_State = STATE_A;
			}
		break;
		}
		
		case STATE_START: // Wait for START to be pressed, restart the timer once it was
		{
			if( b_press| a_press | DOWN | UP | LEFT | RIGHT)
			{
				Next_State = STATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
				RIGHT = false;
			}
			else if (start_press)
			{
				Next_State = STATE_IDLE;
				printf("START\n\r");
				printf("Success! 30 Lives Awarded\n\r");
			}
			else
			{
				Next_State = STATE_START;
			}
		break;
		}
		
		case STATE_ERROR: // All States come here if incorrect key was pressed
		{
				Next_State = STATE_IDLE;
				printf("Error in Sequence\n\r");
		break;
		}

		default:
		{
			printf("UNKNOWN ERROR\n\r");
			while(1){}; // Infinite loop
		}
		}
	}
}
