#include "interrupts.h"

#define Timer0 TIMER0_BASE
#define Timer1 TIMER1_BASE

#define ADC0 ADC0_BASE

volatile bool ALERT_DEBOUNCE = false;
volatile bool ALERT_2_SEC = false;

//Used to de-bounce push buttons
void TIMER0A_Handler(void) {
	TIMER0_Type *gp_timer;
	
	ALERT_DEBOUNCE = true;
	
	gp_timer = (TIMER0_Type*)Timer0;
	
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
}

//Global 2 Second Timer
void TIMER1A_Handler() {
	TIMER0_Type *gp_timer;
	
	ALERT_2_SEC = true;
	
	gp_timer = (TIMER0_Type*)Timer1;
	
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
}

//ADC Compare Interrupt
void ADC0SS0_Handler() {
	
	
}
	//Use ADC ISC (Interrupt Status and Clear)

