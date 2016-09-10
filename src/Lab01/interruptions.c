/*
 * interruptions.c
 *
 *  Created on: Sep 10, 2016
 *      Author: Fabian Melendez
 */


// Lab01 definitions
#include "interruptions.h"
//////////////////////////////////////////////////////////////////////////////
// Interruptions
//////////////////////////////////////////////////////////////////////////////

// **********************************
// Interrupt service routine for
// Timer A0
//
// T = 1 ms
// **********************************
void TA0_0_ISR(void) {
	// Clear interruption flag
	TIMER_A0->CTL ^= TIMER_A_CTL_IFG;

	// - Divide the clock further, to achieve human readable times.
	if(g_u16TimerCounter > 0){
		// - Toggle P2.00
		P2->OUT ^= LED_MASK;
		g_u16TimerCounter--;

	} else {
		P2->OUT = 0;
    }
	return;
}


// **********************************
// Interrupt service routine for
// Timer A0
//
// T = 1 ms
// **********************************
/* Port1 ISR */
void S1_PORT1_ISR(void)
{
	// Clear interrupt flag
	P1->IFG &= ~BIT1;

	// This will help with the debouncing
	if(g_u16TimerCounter == 0) {
		// Preload timing for TA0_0_ISR to turn LED on
		g_u16TimerCounter = TIMERA0_COUNT_01s;
	}
}


