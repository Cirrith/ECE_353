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


extern PC_Buffer UART0_Rx_Buffer;
extern PC_Buffer UART0_Tx_Buffer;

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

//*****************************************************************************
// Tx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Tx_Flow(PC_Buffer *tx_buffer)
{
 char c;
 
 // Check to see if we have any data in the circular queue
 if(!pc_buffer_empty(tx_buffer))
 {
 // Move data from the circular queue to the hardware FIFO
 // until the hardware FIFO is full or the circular buffer
 // is empty.
	 while ( !(UART0->FR & UART_FR_TXFF) && !pc_buffer_empty(tx_buffer) ) {
      // Inside Loop: Add the character to the circular buffer
		 pc_buffer_remove(tx_buffer, &c);
	   UART0->DR = c;
	 }
 }
 else
 {
     // Any data in the hardware FIFO will continue to be transmitted
     // but the TX empty interrupt will not be enabled since there
     // is no data in the circular buffer.
 
     // Disable the TX interrupts.
		 UART0->IM &= ~UART_IM_TXIM;
  }

  // Clear the TX interrupt so it can trigger again when the hardware
  // FIFO is empty
	UART0->ICR = 0x20;
}

//*****************************************************************************
// Rx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Rx_Flow(PC_Buffer *rx_buffer)
{
  // Loop until all characters in the RX FIFO have been removed
	char data;
  while ( !(UART0->FR & UART_FR_RXFE) ) {
      // Inside Loop: Add the character to the circular buffer
			data = UART0->DR;
			pc_buffer_add(rx_buffer, data);
	}
  // Clear the RX interrupts so it can trigger again when the hardware 
  // FIFO becomes full
	
	UART0->ICR = 0x50;
}

//*****************************************************************************
// UART0 Interrupt Service handler
//*****************************************************************************
void UART0_Handler(void)
{
    uint32_t  status;

    // Check to see if RXMIS or RTMIS or TXMIS is active
    status = UART0->MIS;

    if ( status & (UART_MIS_RXMIS | UART_MIS_RTMIS ) )
    {
       UART0_Rx_Flow(&UART0_Rx_Buffer);
    }
		
		if (status & UART_MIS_TXMIS)
			UART0_Tx_Flow(&UART0_Tx_Buffer);
		
    return;
}
