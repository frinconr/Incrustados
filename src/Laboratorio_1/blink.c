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
// MAIN
//////////////////////////////////////////////////////////////////////////////

void main(void)
{
	// Call SetUp routine.
	SetUp();

	// Initial Blinking to show configuration succeeded
	InitialBlinking();

	while(1) {}
}


