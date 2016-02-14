//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "boardUtil.h"

void UART0_Handler(void);

/******************************************************************************
 * Global Variables
 *****************************************************************************/
extern char TEAM[];
extern char PERSON1[];
extern char PERSON2[];


/******************************************************************************
 * External Function Declarations
 *****************************************************************************/
extern int32_t str_emphasize(char *text, char *search);



//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  
  DisableInterrupts();
  serialDebugInit();
  EnableInterrupts();
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	char text[] = "There are 10 types of people in the world.  People who understand binary and those who do not.";
	char search[] = "People";
	int return_val;
	
  initializeBoard();

  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - Spring 2016 HW2\n\r");
  printf("*   %s\n\r",TEAM);
  printf("*      %s\n\r",PERSON1);
  printf("*      %s\n\r",PERSON2);
  printf("**************************************\n\r");
  
	printf("Before Function: %s\n\r",text);
	
	return_val = str_emphasize(text,search);
	
	printf("After Function : %s\n\r",text);
	printf("Found %d occurances of the search string\n\r",return_val);
	
  // Infinite Loop
  while(1){
  
  
  };
}
