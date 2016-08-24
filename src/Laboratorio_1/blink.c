/******************************************************************************
*
* MSP432 blink.c template - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////////////////////////////////

#include "msp.h"
#include "lab01.h"

//////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////
volatile uint16_t g_u16TimerCounter = 0;

//////////////////////////////////////////////////////////////////////////////
// Function Definitions
//////////////////////////////////////////////////////////////////////////////


/** SetUp
 *
 * Configures the device.
 * - General device config.
 * - Ports configuration
 * - Interruptions Configuration.
 */
void SetUp() {
    // ****************************
    //         DEVICE CONFIG
    // ****************************
    // - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // ****************************
    //         PORTS CONFIG
    // ****************************
	// - Configure P1.0 as output
	P1->DIR |= LED_MASK;

	// Disable interruptions
	__disable_irq();

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__2 | TIMER_A_CTL_IE;
	NVIC_SetPriority(TA0_N_IRQn,1);
	NVIC_EnableIRQ(TA0_N_IRQn);
	TIMER_A0->CCR[0] = 0xFFFF;
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
}

/** Enable Interruptions
 *	- Enables all interruptions
*/
void EnableInterruptions() {
	// Enable all interruptions
	__enable_irq();
}


/** InitialBlinking
 *
 * Creates an initial blinking to confirm correct product configuration.
 * - Blinks BLINKING_ITERATIONS
 * - Using a loop delay of BLINKING_DELAY
 * - LED color/power is defined by LED_MASK
 */
void InitialBlinking() {
	// Counter for blinking iterations
	uint8_t l_u8IterationCounter;
	// Counter for delay between iterations
	uint16_t l_u16DelayCounter;

	for(l_u8IterationCounter = 0; l_u8IterationCounter < 2*BLINKING_ITERATIONS; l_u8IterationCounter++) {
		// Toggle P1
		P1->OUT ^= LED_MASK;

		// Blinking delay
		for(l_u16DelayCounter = BLINKING_DELAY; l_u16DelayCounter > 0; l_u16DelayCounter--);
	}
}


//////////////////////////////////////////////////////////////////////////////
// Interruptions
//////////////////////////////////////////////////////////////////////////////

/* This interruption happens every 0.02184 s
 * This is because a 3MHz clock was selected and a divider by 8 was selected.
 * */
void TA0_0_ISR(void)
{
        // - Divide the clock further, to achieve human readable times.
        if(g_u16TimerCounter == TIMERA0_COUNT)
        {
                // - Toggle P1.0
                P1->OUT ^= BIT0;
                g_u16TimerCounter = 0U;

        }
        else
        {
        	g_u16TimerCounter += 1;
        }
        return;
}


//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////

void main(void)
{
	// Call SetUp routine.
	SetUp();

	// Initial Blinking to show configuration succeeded
	InitialBlinking();

	// Enable Interruptions
	EnableInterruptions();

	// Final Loop
	while(1) {}
}


