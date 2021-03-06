#include "timers.h"

//*****************************************************************************
// Verifies that the base address is a valid GPIO base address
//*****************************************************************************
static bool verify_base_addr(uint32_t base_addr)
{
   switch( base_addr )
   {
     case TIMER0_BASE:
     case TIMER1_BASE:
     case TIMER2_BASE:
     case TIMER3_BASE:
     case TIMER4_BASE:
     case TIMER5_BASE:
     {
       return true;
     }
     default:
     {
       return false;
     }
   }
}

//*****************************************************************************
// Returns the RCGC and PR masks for a given TIMER base address
//*****************************************************************************
static bool get_clock_masks(uint32_t base_addr, uint32_t *timer_rcgc_mask, uint32_t *timer_pr_mask)
{
  // Set the timer_rcgc_mask and timer_pr_mask using the appropriate
  // #defines in ../include/sysctrl.h
  switch(base_addr)
  {
    case TIMER0_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R0;
      *timer_pr_mask = SYSCTL_PRTIMER_R0;
      break;
    }
    case TIMER1_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R1;
      *timer_pr_mask = SYSCTL_PRTIMER_R1;
      break;
    }
    case TIMER2_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R2;
      *timer_pr_mask = SYSCTL_PRTIMER_R2;
      break;
    }
    case TIMER3_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R3;
      *timer_pr_mask = SYSCTL_PRTIMER_R3;
      break;
    }
    case TIMER4_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R4;
      *timer_pr_mask = SYSCTL_PRTIMER_R4;
      break;
    }
    case TIMER5_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R5;
      *timer_pr_mask = SYSCTL_PRTIMER_R5;
      break;
    }
    default:
    {
      return false;
    }
  }
  return true;
}


//*****************************************************************************
// Waits for 'ticks' number of clock cycles and then returns.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_wait(uint32_t base_addr, uint32_t ticks)
{
  TIMER0_Type *gp_timer;
  
  // Verify the base address.
  if ( ! verify_base_addr(base_addr) )
  {
    return false;
  }

  // Type cast the base address to a TIMER0_Type struct
  gp_timer = (TIMER0_Type *)base_addr;

  // Stop the timers
	gp_timer->CTL = ~TIMER_CTL_TAEN;
	gp_timer->CTL = ~TIMER_CTL_TBEN;
  // Set the Interval Load Register
	gp_timer->TAILR = ticks; 
  // Clear any timeout interrupts before we wait
  gp_timer->ICR |= TIMER_ICR_TATOCINT; 
  // Enable the Timer
  gp_timer->CTL |= TIMER_CTL_TAEN;
  // wait for the timer to expire.
	while(!(gp_timer->RIS & TIMER_RIS_TATORIS))
	{
		
	}
    
  return true;
}


//*****************************************************************************
// Configure a general purpose timer to be a 32-bit timer.  
//
// Paramters
//  base_address          The base address of a general purpose timer
//
//  mode                  bit mask for Periodic, One-Shot, or Capture
//
//  count_up              When true, the timer counts up.  When false, it counts
//                        down
//
//  enable_interrupts     When set to true, the timer generates and interrupt
//                        when the timer expires.  When set to false, the timer
//                        does not generate interrupts.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_config_32(uint32_t base_addr, uint32_t mode, bool count_up, bool enable_interrupts)
{
  uint32_t timer_rcgc_mask;
  uint32_t timer_pr_mask;
  TIMER0_Type *gp_timer;
  
  // Verify the base address.
  if ( ! verify_base_addr(base_addr) )
  {
    return false;
  }
  
  // get the correct RCGC and PR masks for the base address
  get_clock_masks(base_addr, &timer_rcgc_mask, &timer_pr_mask);
  
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= timer_rcgc_mask;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & timer_pr_mask) == 0) {};

  // Type cast the base address to a TIMER0_Type struct
  gp_timer = (TIMER0_Type *)base_addr;
    
  // Stop the timers
	gp_timer->CTL = ~TIMER_CTL_TAEN;
	gp_timer->CTL = ~TIMER_CTL_TBEN;
  // Set the timer to be a 32-bit timer
	gp_timer->CFG = TIMER_CFG_32_BIT_TIMER;
  // Clear the timer mode 
	gp_timer->TAMR &= ~TIMER_TAMR_TAMR_M;
  // Set the mode
	gp_timer->TAMR |= mode;
  // Set the timer direction.  count_up: 0 for down, 1 for up.
  gp_timer->TAMR &= ~TIMER_TAMR_TACDIR;
  if( count_up )
  {
    // Set the direction bit
		gp_timer->TAMR |= TIMER_TAMR_TACDIR;
  }
  
  // Disable Timer Interrupts  
	gp_timer->IMR &= ~TIMER_IMR_TATOIM;
	
	if(enable_interrupts)
	{
		gp_timer->IMR |= TIMER_IMR_TATOIM;
	}
    
  return true;  
}

// Enables the timer for the given amount of ticks
bool hw3_timer_enable(uint32_t base_addr, uint32_t ticks)
{
	TIMER0_Type *gp_timer;
  
  if (!verify_base_addr(base_addr))
  {
    return false;
  }
	
	
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	
	gp_timer = (TIMER0_Type *)base_addr;
	gp_timer->ICR |= TIMER_ICR_TATOCINT;  // Clear interrupts
	gp_timer->TAILR = ticks;							// Set the number of ticks
	
	gp_timer->CTL |= TIMER_CTL_TAEN;
	
	return true;
}

// Configure Timer0 and enable its interrupt
void hw3_timer0_init(void)
{
	gp_timer_config_32(TIMER0_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	hw3_timer_enable(TIMER0_BASE, SEC5ms_TIME);
	NVIC_SetPriority(TIMER0A_IRQn, 1);
	NVIC_EnableIRQ(TIMER0A_IRQn);
}

// Configure Timer1 and enable its interrupt
void hw3_timer1_init(void)
{
	gp_timer_config_32(TIMER1_BASE, TIMER_TAMR_TAMR_1_SHOT, false, true);
	hw3_timer_enable(TIMER1_BASE, SEC2s_TIME);
	NVIC_SetPriority(TIMER1A_IRQn, 1);
	NVIC_EnableIRQ(TIMER1A_IRQn);
}
