//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"

void UART0_Handler(void);
extern void hw1(void);
extern void serialDebugInit(void);

/******************************************************************************
 * Global Variables
 *****************************************************************************/
 extern uint32_t TRANSACTIONS_TOTAL;
 extern uint32_t TRANSACTIONS[100];
 extern char TEAM[];
 extern char PERSON1[];
 extern char PERSON2[];
 
//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  
	int i;
	
  serialDebugInit();  
  
  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - Spring 2016 HW1\n\r");
  printf("*   %s\n\r",TEAM);
  printf("*      %s\n\r",PERSON1);
  printf("*      %s\n\r",PERSON2);
  printf("**************************************\n\r");
  /*
  TRANSACTIONS[0] = 0xA0000101;
	TRANSACTIONS[1] = 0xA0000202;
	TRANSACTIONS[2] = 0xA0010303;
	TRANSACTIONS[3] = 0xB0000114;
	TRANSACTIONS[4] = 0xA0010100;
	TRANSACTIONS[5] = 0xB0030592;
	TRANSACTIONS[6] = 0xF0000000;
	*/
	
	for(i = 0; i < 99; i++)
	{
		TRANSACTIONS[i] = 0xB0000101;
	}
	
	TRANSACTIONS[99] = 0xF0000000;
  
  hw1();
  if(TRANSACTIONS_TOTAL != 0xA0020592)
  {
    printf("Basic Test failed\n\r");
  }
  else
  {
    printf("Basic Test Passed\n\r");
  }
  // Infinite Loop
  while(1){
  
  
  };
}
