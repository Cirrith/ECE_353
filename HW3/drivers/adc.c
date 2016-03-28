#include "adc.h"

/******************************************************************************
 * Initializes ADC to use Sample Sequencer #3, triggered by the processor,
 * no IRQs
 *****************************************************************************/
bool initializeADC(  uint32_t adc_base )
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
      // Set pr_mask 
      rcgc_adc_mask = SYSCTL_RCGCADC_R0;
			pr_mask = SYSCTL_PRADC_R0;
      break;
    }
    case ADC1_BASE :
    {
			// set rcgc_adc_mask
      // Set pr_mask 
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
	
	myADC->IM = ADC_IM_DCONSS0;
		
		//Set Sample Sequencer 0 to interrupt on 1st through 5th sample
	myADC->SSCTL0 = ADC_SSCTL0_IE0 | ADC_SSCTL0_IE1 | ADC_SSCTL0_IE2 | ADC_SSCTL0_IE3 | ADC_SSCTL0_IE4 | ADC_SSCTL0_END4; 
		
		//Set which samples get moved to the digial comparator
	myADC->SSOP0 |= ADC_SSOP0_S0DCOP | ADC_SSOP0_S1DCOP | ADC_SSOP0_S2DCOP | ADC_SSOP0_S3DCOP | ADC_SSOP0_S4DCOP;
		
		//Determine which Digital comparator gets which sample
	myADC->SSDC0 = (0x4 << ADC_SSDC0_S4DCSEL_S) | (0x3 << ADC_SSDC0_S3DCSEL_S) | (0x2 << ADC_SSDC0_S2DCSEL_S) | (0x1 << ADC_SSDC0_S1DCSEL_S) | ADC_SSDC0_S0DCSEL_S;
		
		//Set Compare range
	myADC->DCCMP0 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP1 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP2 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP3 = COMP_HIGH | COMP_LOW;
	myADC->DCCMP4 = COMP_HIGH | COMP_LOW;
	
		//Configure each sample slot in sequencer
	myADC->DCCTL0 = ADC_DCCTL0_CIC_LOW | ADC_DCCTL0_CIE|  ADC_DCCTL0_CIM_ONCE;
	myADC->DCCTL1 = ADC_DCCTL1_CIC_LOW | ADC_DCCTL1_CIE|  ADC_DCCTL1_CIM_ONCE;
	myADC->DCCTL2 = ADC_DCCTL2_CIC_HIGH | ADC_DCCTL2_CIE|  ADC_DCCTL2_CIM_ONCE;
	myADC->DCCTL3 = ADC_DCCTL3_CIC_LOW | ADC_DCCTL3_CIE|  ADC_DCCTL3_CIM_ONCE;
	myADC->DCCTL4 = ADC_DCCTL4_CIC_HIGH | ADC_DCCTL4_CIE|  ADC_DCCTL4_CIM_ONCE;
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
  
  myADC->PSSI =   ADC_PSSI_SS3;     // Start SS3
  
  while( (myADC->RIS & ADC_RIS_INR3)  == 0)
  {
    // wait
  }
  
  result = myADC->SSFIFO3 & 0xFFF;    // Read 12-bit data
  
  myADC->ISC  = ADC_ISC_IN3;          // Ack the conversion
  
  return result;
}

void hw3_adc_init(void)
{
	initializeADC(PS2_ADC_BASE);
}

