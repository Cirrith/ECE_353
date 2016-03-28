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

extern void hw3_config(void);
extern void UART0_Handler(void);
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

/******************************************************************************
 * Global Variables
 *****************************************************************************/
char TEAM[] = "ECE Staff";
char PERSON1[] = "Add Name";
char PERSON2[] = "Add Name";

//*****************************************************************************
//*****************************************************************************
void initialize_board(void)
{
  
  DisableInterrupts();
  hw3_config();
	//hw3_gpio_init();
	//hw3_adc_init();
	//hw3_timer0_init();
	//hw3_timer1_init();
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

	}
}
