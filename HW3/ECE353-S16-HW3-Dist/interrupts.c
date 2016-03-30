#include "interrupts.h"

#define Timer0 TIMER0_BASE
#define Timer1 TIMER1_BASE

#define Adc0 ADC0_BASE

// Globals that the main routine looks at
volatile bool ALERT_DEBOUNCE = false;
volatile bool ALERT_2_SEC = false;

volatile bool UP = false;
volatile bool DOWN = false;
volatile bool RIGHT = false;
volatile bool LEFT = false;

// ISR Used to de-bounce push buttons
void TIMER0A_Handler(void) {
	TIMER0_Type *gp_timer;
	ADC0_Type *myADC;
	
	// Let main know the timer went off
	ALERT_DEBOUNCE = true;
	
	// Clear this interrupt
	gp_timer = (TIMER0_Type*)Timer0;
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
	
	// Start the ADC0
	myADC = (ADC0_Type *) Adc0;
	myADC->PSSI = ADC_PSSI_SS0;
}

// 2 Second Timer ISR
void TIMER1A_Handler() {
	TIMER0_Type *gp_timer;
	
	// Let main know the timer went off
	ALERT_2_SEC = true;
	
	// Clear this interrupt
	gp_timer = (TIMER0_Type*)Timer1;
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
}

// ADC Sample Seq 0 ISR
void ADC0SS0_Handler() {
	ADC0_Type *myADC;
	
	myADC = (ADC0_Type *) Adc0;
	
	// Mask off the bits for the PS2 direction
	DOWN = myADC->DCISC & 0x2;
	UP = myADC->DCISC & 0x4;
	RIGHT = myADC->DCISC & 0x8;
	LEFT = myADC->DCISC & 0x10;
	
	// Clear this interrupt
	myADC->DCISC |= 0x1F;
	myADC->ISC |= 0x10001;
	
}
