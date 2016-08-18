/******************************************************************************
*
* MSP432 blink.c template - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

#include "msp.h"

void main(void)
{
	// Call SetUp routine.
	SetUp();

	// Initial Blinking to show configuration succeeded
	InitialBlinking
}

void InitialBlinking() {
	uint32_t l_counter1;
	uint32_t l_counter2;

	// The following code toggles P1.0 port
	P1->DIR |= BIT0;                             /* Configure P1.0 as output */

	for(l_counter2=0; l_counter2<3; l_counter2++) {
		P1->OUT ^= BIT0;                         				/* Toggle P1.0 */
		for(l_counter1=10000; l_counter1>0; l_counter1--);      /* Delay */
	}
}


void SetUp() {
	/* Stop watchdog timer */
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
}
