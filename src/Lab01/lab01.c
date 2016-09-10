/*
 * lab01.c
 *
 *  Created on: Sep 10, 2016
 *      Author: fabian
 */

#include "lab01.h"

/** SetUp
 *
 * Configures the device.
 * - General device config.
 * - Ports configuration
 * - Interruptions Configuration.
 */
void SetUp() {
	// Disable Interruptions
	DisableInterruptions();

	// ****************************
    //         DEVICE CONFIG
    // ****************************
    // - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	/* Stop Watchdog  */
	// MAP_WDT_A_holdTimer();

    // ****************************
    //         PORTS CONFIG
    // ****************************

	// - - - - - - - - - - - - - -
	// P2 Config
	// - - - - - - - - - - - - - -
	ConfigP2LEDOutput();

	// - - - - - - - - - - - - - -
	// P1 Config
	// - - - - - - - - - - - - - -
	ConfigP1ButtonInterrupt();

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	ConfigTimerA0UpMode();
}


//////////////////////////////////////////////////////////////////////////////
// Function Definitions
//////////////////////////////////////////////////////////////////////////////

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
	l_u32AverageTotalSamples = l_u32AverageTotalSamples/MAX_SAMPLES;

	// Now we will calculate the average of the samples of the last second

	// Calculate start index
	l_i8LocalIndex = g_u8ADCIndex - SAMPLES_PER_SECOND;
	if (l_i8LocalIndex < 0) {
		l_i8LocalIndex += MAX_SAMPLES;
	}


	int8_t l_i8Index;
	// Loop through the last second of samples
	for(l_i8Index = 0; l_i8Index < SAMPLES_PER_SECOND; l_i8Index++) {
		l_u32AverageLastSecond += g_u16ADCResults[l_i8LocalIndex];
		l_i8LocalIndex = (l_i8LocalIndex+1) % MAX_SAMPLES;
	}
	l_u32AverageLastSecond = l_u32AverageLastSecond/SAMPLES_PER_SECOND;

	// Check if we need to turn on the LED
	if(l_u32AverageTotalSamples*0.9 < l_u32AverageLastSecond) {
		// Turn on the LED
		// Preload timing for TA0_0_ISR to turn LED on
		g_u16TimerCounter = TIMERA0_COUNT_01s;
	}

}
