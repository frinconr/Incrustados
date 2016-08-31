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

// Array for storing the samples of ONE A/D conversion
volatile uint16_t g_u16ADCResults[NUM_SAMPLES];

// Array for storing the historic of A/D measures
volatile uint16_t g_u16SamplesArray[MAX_SAMPLES];

// Index for storing in the
volatile uint8_t g_u8ADCIndex;


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
	// Disable Interruptions
	__disable_irq();

    // ****************************
    //         DEVICE CONFIG
    // ****************************
    // - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // ****************************
    //         PORTS CONFIG
    // ****************************

	// - - - - - - - - - - - - - -
	// P2 Config
	// - - - - - - - - - - - - - -
	// This is the LED port

	// - Configure P2.LED_MASK as output
	P2->DIR |= LED_MASK;
	// Clean Port0
	P2->OUT = 0;

	// - - - - - - - - - - - - - -
	// P1 Config
	// - - - - - - - - - - - - - -
	// This is the button interrupt port

	// - Configure P1.4 (S2) as input

	// Configuring P1.1 (switch) as input with pull-up
	// resistor. Rest of pins are configured as output low.
	// Notice intentional '=' assignment since all P1 pins are being
	// deliberately configured

	// Configure P1.1 as input
	P1->DIR &= ~(uint8_t) BIT1;
	// Enable pull-up resistor (P1.1 output high)
	P1->OUT = BIT1;
	P1->REN = BIT1;
	// Configure as a GPIO
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	// Interrupt on high-to-low transition
	P1->IES = BIT1;
	// Clear all P1 interrupt flags
	P1->IFG = 0;
	// Enable interrupt for P1.1
	P1->IE = BIT1;

	// Enable Port 1 interrupt on the NVIC
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_EnableIRQ(PORT1_IRQn);



	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.

	// Set clock as SMCLK (3MHz), divider by 4, and enable interruption
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__2 | TIMER_A_CTL_IE;
	// Load value of 15000 = (0x3A98)
	TIMER_A0->CCR[0] = 0x3A98;
	// Set mode as: UpMode (counts to CCR, interrupts and counter goes back to 0).
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	// Set interruption in NVIC
	NVIC_SetPriority(TA0_N_IRQn,1);
	NVIC_EnableIRQ(TA0_N_IRQn);


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

	for(l_u8IterationCounter = 0; l_u8IterationCounter < 2*BLINKING_ITERATIONS-1; l_u8IterationCounter++) {
		// Toggle P2
		P2->OUT ^= LED_MASK;

		// Blinking delay
		for(l_u16DelayCounter = 0; l_u16DelayCounter < BLINKING_DELAY; l_u16DelayCounter++);
	}
}


/* FillSamplesArray
 *
 * Uses the last_sample argument and
 *
 */
void FillSamplesArray(uint16_t last_sample) {
	uint32_t l_u32AverageTotalSamples = 0;
	uint32_t l_u32AverageLastSecond = 0;
	int8_t l_i8LocalIndex;

	// Fill g_u16ADCResults array where g_u8ADCIndex indicates
	g_u16ADCResults[g_u8ADCIndex] = last_sample;

	// Update g_u8ADCIndex
	g_u8ADCIndex = (g_u8ADCIndex+1) % MAX_SAMPLES;

	// Now we will calculate the average of the samples
	for(l_i8LocalIndex = 0; l_i8LocalIndex < MAX_SAMPLES; l_i8LocalIndex++) {
		l_u32AverageTotalSamples += g_u16ADCResults[l_i8LocalIndex];
	}
	l_u32AverageTotalSamples = l_u32Average/MAX_SAMPLES;

	// Now we will calculate the average of the samples of the last second

	// Calculate start index
	l_i8LocalIndex = g_u8ADCIndex - SAMPLES_PER_SECOND;
	if (l_i8LocalIndex < 0) {
		l_i8LocalIndex += MAX_SAMPLES;
	}

	// Calculate the average (start with current value of index)
	for(l_i8LocalIndex; l_i8LocalIndex < g_u8ADCIndex; l_i8LocalIndex) {
		l_u32AverageLastSecond += g_u16ADCResults[l_i8LocalIndex];
	}
	l_u32AverageLastSecond = l_u32AverageLastSecond/SAMPLES_PER_SECOND;

	// Check if we need to turn on the LED
	if(l_u32AverageTotalSamples*0.9 < l_u32AverageLastSecond) {
		// Turn on the LED
		// Preload timing for TA0_0_ISR to turn LED on
		g_u16TimerCounter = TIMERA0_COUNT_01s;
	}

}


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


//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////

void main(void) {
	// Call SetUp routine.
	SetUp();

    // Initial Blinking
	InitialBlinking();

	// Enable Interruptions
	EnableInterruptions();

	while(1) {
		// Wait For Events
		__wfe();
	}
}


