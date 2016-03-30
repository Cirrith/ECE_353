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
#include "gpio_port.h"
#include "adc.h"
#include "timers.h"
#include "interrupts.h"

extern void hw3_config(void);
extern void UART0_Handler(void);
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

/******************************************************************************
 * Global Variables
 *****************************************************************************/
char TEAM[] = "Team 18";
char PERSON1[] = "Ryan Bambrough";
char PERSON2[] = "Nick Adams";

bool start_Buf[5] = {0,0,0,0,0};
bool a_Buf[5] = {0,0,0,0,0};
bool b_Buf[5] = {0,0,0,0,0};

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

void initialize_board(void)
{
  
  DisableInterrupts();
  hw3_config();
	hw3_gpio_init();
	hw3_adc_init();
	hw3_timer0_init();
	hw3_timer1_init();
  EnableInterrupts();
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
	State Current_State = STATE_IDLE;
	State Next_State = STATE_IDLE;
	bool a_press = false;
	bool b_press = false;
	bool start_press = false;
  
	initialize_board();
  
  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - Spring 2016 HW3\n\r");
  printf("*   %s\n\r",TEAM);
  printf("*      %s\n\r",PERSON1);
  printf("*      %s\n\r",PERSON2);
  printf("**************************************\n\r");
  
  // Infinite Loop
  while(1)
  {	
		Current_State = Next_State;
		
		if(ALERT_2_SEC)
		{
			//printf("2 Second Timer\n\r");
			
			hw3_timer1_init();
			
			ALERT_2_SEC = false;
		}

		if(ALERT_DEBOUNCE)
		{
			a_press = a_button_pressed();
			b_press = b_button_pressed();
			start_press = start_button_pressed();
			ALERT_DEBOUNCE = false;
		}
		
		switch (Current_State)
		{
		case STATE_IDLE:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_UP2:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_DOWN1:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_DOWN2:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_LEFT1:
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
				Next_State = STATE_RIGHT1;
				LEFT = false;
				printf("LEFT\n\r");
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_RIGHT1:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_LEFT2:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_RIGHT2:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_B:
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_A:
		{
			if( b_press| start_press | DOWN | UP | LEFT | RIGHT)
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
			}
			else
			{
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_START:
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
				Next_State = STATE_IDLE;
			}
		break;
		}
		
		case STATE_ERROR:
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
