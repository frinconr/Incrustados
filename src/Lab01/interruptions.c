/*
 * interruptions.c
 *
 *  Created on: Sep 10, 2016
 *      Authors: Fabian Melendez / Felipe Rincón.
 */


// Include header files
#include "interruptions.h"


//////////////////////////////////////////////////////////////////////////////
// Interruptions
//////////////////////////////////////////////////////////////////////////////

// ***********************************************************
// TA0_0_ISR
// ***********************************************************
/*
 * Interrupt service routine for the timer A0. It expects the
 * subroutine to be called with a period of T=1 ms.
 *
 * It:
 * 	- Turns off the TIMER A0 interrupt flag.
 * 	- With the counter g_u16TimerCounter_LED it handles the LED
 * 	lighting, if the counter is greater than zero, then the LED
 * 	is turned on, and the counter decreased. If the counter is
 * 	zero, then the led is turned off.
 * 	- It implements a debouncing for the S2 button subroutine
 *  using the g_u8TimerCounter_Debouncer.
 *  - It launches a ADC14 conversion every 0.2s, using the coun-
 *  ter g_u8TimerCounter_ADC14.
 */

void TA0_0_ISR(void) {
	// Clear interruption flag
	TIMER_A0->CTL ^= TIMER_A_CTL_IFG;

	// Divide the clock further, to achieve human readable times.
	if(g_u16TimerCounter_LED > 0){
		// - Turn on LED on P2
		P2->OUT |= LED_MASK;

		// Decrease timer
		g_u16TimerCounter_LED--;

	} else {
		// Turn off LED on P2
		P2->OUT &= ~LED_MASK;
    }

	// Implement debouncing for S2 button
	if(g_u8TimerCounter_Debouncer > 0) {
		g_u8TimerCounter_Debouncer--;
	}


	// Activate ADC14 conversion each 200ms
	g_u8TimerCounter_ADC14 = (g_u8TimerCounter_ADC14 + 1)  % 20;
	if(g_u8TimerCounter_ADC14 == 0){
		ADC14->IER0 = ADC14_IER0_IE3;           // Enable ADC14IFG.3
		ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC;
		// Change LUX_FLAG
		g_bGlobalFlags[LUX_FLAG] = true;
	}



	return;
}


// **********************************
// PORT 1 ISR
//
//
// T = 1 ms
// **********************************
/* Port1 ISR */
void S1_PORT1_ISR(void)
{
	// Clear interrupt flag
	P1->IFG &= ~BIT1;

	if (g_u8TimerCounter_Debouncer == 0) {
		// Preload debouncing
		g_u8TimerCounter_Debouncer = TIMERA0_COUNT_Debouncer;

		// This will help with the debouncing
		if(g_u16TimerCounter_LED == 0) {
			TurnLightOn();
		} else {
			TurnLightOff();
		}
	}
}


// **********************************
// Interrupt service routine for
// ADC14
// **********************************
void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0) {
    	// Get lastest value of converter, insert into
    	g_i16ADCResults[g_u8ADCMEMIndex] = ADC14->MEM[0]; // Move A0 results, IFG is cleared

    	// Increment g_u8ADCMEMIndex
    	g_u8ADCMEMIndex++;

    	// Check if we have already the number of samples
        if(g_u8ADCMEMIndex == NUM_SAMPLES){
        	// Reset g_u8ADCMEMIndex
        	g_u8ADCMEMIndex = 0;

        	// Set ADC14_FLAG to indicate we have results
        	g_bGlobalFlags[ADC14_FLAG] = true;

        	ADC14->IER0 &= ~ADC14_IER0_IE3;           // Enable ADC14IFG.3
        } else {
        	ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC;
        }

        __no_operation();                   							// Set Breakpoint1 here
        // Start conversion-software trigger

    }
}


