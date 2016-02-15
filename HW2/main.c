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
extern int32_t str_length(char *string);
extern int32_t str_copy(char *src, char *dest, uint16_t size);
extern int32_t str_compare(char *str_A, char *str_B, uint16_t size);
extern int32_t str_to_upper(char *str, uint16_t size);
extern int32_t str_fill(char *str, char value, uint16_t size);
extern int32_t str_icompare(char *str_A, char *str_B, uint16_t size);

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
	char text[] = "There are 10 types of people in the world.  People who understand binary and pEoPLE who do not. people ppl people elpoep people";
	char search[] = "People";
	char testing[] = "people";
	char test[100];
	int return_val;
	
  initializeBoard();

	printf("Length: %d\n", str_length("Hello, World!"));
	printf("%d -> %s\n", str_copy(text, test, 94), test);
	printf("%d %d\n", str_compare(text, text, 94), str_compare(text, search, 7));
	printf("%d %d\n", str_icompare(text, text, 94), str_icompare(text, search, 7));
	printf("%d\n", str_icompare(testing, search, 7));
	printf("%d\n", str_icompare("!hElLo!", "!HeLlO!", 8));
	printf("%d %d %d %s\n", str_to_upper(text, 2000), str_to_upper(NULL, 1), str_to_upper(testing, 7), testing);
	printf("%d %d %d %s\n", str_fill(text, 'a', 2000), str_fill(NULL, 'a', 1), str_fill(testing, '!', 7), testing);
	
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
