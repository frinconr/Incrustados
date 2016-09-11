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
 * This subroutine:
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


// ***********************************************************
// S1_PORT1_ISR
// ***********************************************************
/*
 * Interrupt service routine for the S1 button it should be ca-
 * lled when the S2 button is pressed. This interruption imple-
 * ments the manual override of the LED, and toggles the light
 * when the interruption is valid (when not in the debouncing
 * time).
 *
 * It:
 * 	- Clears the iterrupt flag.
 * 	- Implement a debouncing using the g_u8TimerCounter_Debouncer
 * 	and the timer interrupt. It waits TIMERA0_COUNT_Debouncer,
 * 	after the interrupt was valid to accept another valid in-
 * 	terrupt.
 * 	- If the interrupt is valid, then it decides of the light
 * 	should be turned on or off, based on g_u16TimerCounter_LED
 * 	if zero, then the light is turned off, so should be turned
 * 	on, if not it should be turned off.
 *
 */
void S1_PORT1_ISR(void)
{
	// Clear interrupt flag
	P1->IFG &= ~BIT1;

	// Implement debouncing
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


// ***********************************************************
// ADC14_IRQHandler
// ***********************************************************
/* Interrupt service routine for ADC14 converter. It should be
 * called when a conversion is ready to be read in the regis-
 * ter ADC14->MEM[0].
 *
 *
 */
void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0) {
    	// Get lastest value of converter, insert into array of conversion set results.
    	g_i16ADCResults[g_u8ADCMEMIndex] = ADC14->MEM[0]; // Move A0 results, IFG is cleared

    	// Increment g_u8ADCMEMIndex for the next conversion
    	g_u8ADCMEMIndex++;

    	// Check if we have already the number of samples
        if(g_u8ADCMEMIndex == NUM_SAMPLES){
        	// Reset g_u8ADCMEMIndex
        	g_u8ADCMEMIndex = 0;

        	// Set ADC14_FLAG to indicate we have results
        	g_bGlobalFlags[ADC14_FLAG] = true;

        	// Enable ADC14IFG.3 interruption
        	ADC14->IER0 &= ~ADC14_IER0_IE3;
        } else {
        	// Queue another conversion
        	ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC;
        }
    }
}


