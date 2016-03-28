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

bool UP[5] = {0,0,0,0,0};
bool LEFT[5] = {0,0,0,0,0};
bool RIGHT[5] = {0,0,0,0,0};

bool up_button_pressed (void)
{
	bool val = GPIOF->DATA & UP_PIN; 
	
	UP[4] = UP[3];
	UP[3] = UP[2];
  UP[2] = UP[1];
	UP[1] = UP[0];
	UP[0] = val;
	
	if(~(UP[0] | UP[1] | UP[2] | UP[3]) & UP[4])
		return true;
	else
		return false;
}

bool left_button_pressed (void)
{
	bool val = GPIOF->DATA & LEFT_PIN; 
	
	LEFT[4] = LEFT[3];
	LEFT[3] = LEFT[2];
  LEFT[2] = LEFT[1];
	LEFT[1] = LEFT[0];
	LEFT[0] = val;
	
	if(~(LEFT[0] | LEFT[1] | LEFT[2] | LEFT[3]) & LEFT[4])
		return true;
	else
		return false;
}

bool right_button_pressed (void)
{
	bool val = GPIOF->DATA & RIGHT_PIN; 
	
	RIGHT[4] = RIGHT[3];
	RIGHT[3] = RIGHT[2];
  RIGHT[2] = RIGHT[1];
	RIGHT[1] = RIGHT[0];
	RIGHT[0] = val;
	
	if(~(RIGHT[0] | RIGHT[1] | RIGHT[2] | RIGHT[3]) & RIGHT[4])
		return true;
	else
		return false;
}

//*****************************************************************************
//*****************************************************************************
void initialize_board(void)
{
  
  DisableInterrupts();
  hw3_config();
	hw3_gpio_init();
	//hw3_adc_init();
	hw3_timer0_init();
	hw3_timer1_init();
  EnableInterrupts();
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{

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
		bool left_press;
		bool right_press;
		bool up_press;
		
		if(ALERT_2_SEC)
		{
			printf("2 Second Timer\n\r");
			
			hw3_timer1_init();
			
			ALERT_2_SEC = false;
		}
		
		if(ALERT_DEBOUNCE)
		{
			left_press = left_button_pressed();
			right_press = right_button_pressed();
			up_press = up_button_pressed();
			
			if(left_press)
				printf("LEFT was pressed\n\r");
			
			if(right_press)
				printf("RIGHT was pressed\n\r");
						
			if(up_press)
				printf("UP was pressed\n\r");
			
			ALERT_DEBOUNCE = false;
		}
	}
}
