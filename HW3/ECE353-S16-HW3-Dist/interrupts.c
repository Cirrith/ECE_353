#include "interrupts.h"

volatile bool ALERT_DEBOUNCE = false;
volatile bool ALERT_2_SEC = false;

//Used to de-bounce push buttons
void TIMER0A_Handler(void) {
	TIMER0_Type *gp_timer;
	
	ALERT_DEBOUNCE = true;
	
	gp_timer = (TIMER0_Type*)TIMER0_BASE;
	
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
}

//Global 2 Second Timer
void TIMER1A_Handler() {
	TIMER0_Type *gp_timer;
	
	ALERT_2_SEC = true;
	
	gp_timer = (TIMER0_Type*)TIMER1_BASE;
	
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
}

void TIMER0B_Handler(void)
{
	;
}
	
void TIMER1B_Handler(void)
{
	;
}

void WTIMER0A_Handler()
{
	;
}

void WTIMER1B_Handler ()
{
	;
}
