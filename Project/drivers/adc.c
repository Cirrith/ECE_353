#include "adc.h"

bool initializeADC4PS2(uint32_t adc_base)
{
  ADC0_Type  *myADC;
  uint32_t rcgc_adc_mask;
  uint32_t pr_mask;

  // Verify that it is either ADC0 or ADC1
  // Set the rcgc_adc_mask and pr_mask  
  switch (adc_base) 
  {
    case ADC0_BASE :
    {
      rcgc_adc_mask = SYSCTL_RCGCADC_R0;
			pr_mask = SYSCTL_PRADC_R0;
      break;
    }
    case ADC1_BASE :
    {
      rcgc_adc_mask = SYSCTL_RCGCADC_R1;
			pr_mask = SYSCTL_PRADC_R1;
      break;
    }
    
    default:
      return false;
  }
  
  // Turn on the ADC Clock
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  // Wait for ADCx to become ready
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  // Type Cast adc_base and set it to myADC
  myADC = (ADC0_Type *)adc_base;
		
		//Disable Sample Sequencer 0
	myADC->ACTSS &=  ~ADC_ACTSS_ASEN0;
	
		//Set which values are used in the sample sequencer (Y Y X X X)
	myADC->SSMUX0 = (0x0 << ADC_SSMUX0_MUX0_S) | (0x0 << ADC_SSMUX0_MUX1_S) | (0x1 << ADC_SSMUX0_MUX2_S) | (0x1 << ADC_SSMUX0_MUX3_S) | (0x1 << ADC_SSMUX0_MUX4_S);
		
		//Tell Sample Sequencer that sample 5 is is the end of the sequence
	//myADC->SSCTL0 = ADC_SSCTL0_END4;
		
		//Set Sample Sequencer 0 to interrupt on 1st through 5th sample (We may not want this, it interrups at the end fo the sample conversion)
	myADC->SSCTL0 = ADC_SSCTL0_IE0 | ADC_SSCTL0_IE1 | ADC_SSCTL0_IE2 | ADC_SSCTL0_IE3 | ADC_SSCTL0_IE4 | ADC_SSCTL0_END4; 
		
		//Store Sample Sequencer values in Digital comparator (SSOP0)
	myADC->SSOP0 |= ADC_SSOP0_S0DCOP | ADC_SSOP0_S1DCOP | ADC_SSOP0_S2DCOP | ADC_SSOP0_S3DCOP | ADC_SSOP0_S4DCOP;
		
		//Set which Digital comparator gets what from sample sequencer
	myADC->SSDC0 = (0x4 << ADC_SSDC0_S4DCSEL_S) | (0x3 << ADC_SSDC0_S3DCSEL_S) | (0x2 << ADC_SSDC0_S2DCSEL_S) | (0x1 << ADC_SSDC0_S1DCSEL_S) | ADC_SSDC0_S0DCSEL_S;
		
		//Event Multiplexer Select (EMUX)		
	myADC->EMUX = ADC_EMUX_EM0_PROCESSOR;
		
		//Configure each sample slot in digital comparartor
	myADC->DCCTL0 = ADC_DCCTL0_CIC_LOW | 	ADC_DCCTL0_CIE|  ADC_DCCTL0_CIM_ONCE;
	myADC->DCCTL1 = ADC_DCCTL1_CIC_LOW | 	ADC_DCCTL1_CIE|  ADC_DCCTL1_CIM_ONCE;
	myADC->DCCTL2 = ADC_DCCTL2_CIC_HIGH | ADC_DCCTL2_CIE|  ADC_DCCTL2_CIM_ONCE;
	myADC->DCCTL3 = ADC_DCCTL3_CIC_LOW |	ADC_DCCTL3_CIE|  ADC_DCCTL3_CIM_ONCE;
	myADC->DCCTL4 = ADC_DCCTL4_CIC_HIGH | ADC_DCCTL4_CIE|  ADC_DCCTL4_CIM_ONCE;
		
		//Enable Sammple Sequencer 0 interrupt mask
	myADC->IM = ADC_IM_DCONSS0;
		
		//Set Compare range
	myADC->DCCMP0 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP1 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP2 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP3 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP4 = COMP_HIGH | COMP_LOW;
		
		//Enable SS0
	myADC->ACTSS |=  ADC_ACTSS_ASEN0;
	
  return true;
}

/******************************************************************************
 * Initializes ADC to use Sample Sequencer #3, triggered by the processor,
 * no IRQs
 *****************************************************************************/
bool initializeADC4POT(uint32_t adc_base)
{
  ADC0_Type  *myADC;
  uint32_t rcgc_adc_mask;
  uint32_t pr_mask;
  
  // examine the adc_base.  Verify that it is either ADC0 or ADC1
  // Set the rcgc_adc_mask and pr_mask  
  switch (adc_base) 
  {
    case ADC0_BASE :
    {
      // set rcgc_adc_mask
			rcgc_adc_mask = SYSCTL_RCGCADC_R0;
      
      // Set pr_mask 
			pr_mask = SYSCTL_PRADC_R0;
      break;
    }
    case ADC1_BASE :
    {
      // set rcgc_adc_mask
			rcgc_adc_mask = SYSCTL_RCGCADC_R1;
      
      // Set pr_mask 
			pr_mask = SYSCTL_PRADC_R1;
      break;
    }
    
    default:
      return false;
  }
  
  // Turn on the ADC Clock
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  // Wait for ADCx to become ready
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  // Type Cast adc_base and set it to myADC
  myADC = (ADC0_Type *)adc_base;
  
  // disable sample sequencer #3 by writing a 0 to the 
  // corresponding ASENn bit in the ADCACTSS register 
	myADC->ACTSS &= ~ADC_ACTSS_ASEN3;

  // Set the event multiplexer to trigger conversion on a processor trigger
  // for sample sequencer #3.
  //myADC->EMUX &= ~ADC_EMUX_EM3_M;
	myADC->EMUX |= ADC_EMUX_EM3_PROCESSOR;

  // Set IE0 and END0 in SSCTL3
	myADC->SSCTL3 |= ADC_SSCTL3_IE0;
	myADC->SSCTL3 |= ADC_SSCTL3_END0;
  
  return true;
}

/******************************************************************************
 * Reads SSMUX3 for the given ADC.  Busy waits until completion
 *****************************************************************************/
uint32_t getADCValue( uint32_t adc_base, uint8_t channel)
{
  ADC0_Type  *myADC;
  uint32_t result;
  
  if( adc_base == 0)
  {
    return false;
  }
  
  myADC = (ADC0_Type *)adc_base;
  
  myADC->SSMUX3 = channel;          // Set the Channel
  
  myADC->ACTSS |= ADC_ACTSS_ASEN3;  // Enable SS3
  
  //myADC->PSSI =   ADC_PSSI_SS3;     // Start SS3
  
  while( (myADC->RIS & ADC_RIS_INR3)  == 0)
  {
    // wait
  }
  
  result = myADC->SSFIFO3 & 0xFFF;    // Read 12-bit data
  
  myADC->ISC  = ADC_ISC_IN3;          // Ack the conversion
	
  myADC->ACTSS &= ~ADC_ACTSS_ASEN3;  // Disable SS3
  
  return result;
}

// Init the ADC and enable its interrupt
void hw3_adc_init(void)
{
	initializeADC4PS2(PS2_ADC_BASE);
	NVIC_SetPriority(ADC0SS0_IRQn, 1);
	NVIC_EnableIRQ(ADC0SS0_IRQn);
	
	initializeADC4POT(POT_ADC_BASE);
	NVIC_SetPriority(ADC1SS3_IRQn, 1);
	NVIC_EnableIRQ(ADC1SS3_IRQn);
}

