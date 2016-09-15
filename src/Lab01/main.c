/*
 * -------------------------------------------
 *    Lab01 -- Sistemas Incrustados
 * -------------------------------------------
 *	This file contains the main program for a
 *	intelligent lamp that is activated by sound.
 *
 *
 * Authors: Fabián Meléndez / Felipe Rincón
***********************************************/

/* DriverLib Includes */
#include <driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// MSP library
#include "msp.h"

// Hardware definitions
#include "hardware.h"
// Lab01 definitions
#include "lab01.h"
// Interruptions
#include "interruptions.h"


//////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////

uint16_t g_u16TimerCounter_LED = 0;
uint8_t g_u8TimerCounter_ADC14 = 0;
uint8_t g_u8TimerCounter_Debouncer = 0;

// Array for storing the samples of ONE set of A/D conversion
int16_t g_i16ADCResults[NUM_SAMPLES];
// Index for storing data from ONE set of ADC14 conversion
uint8_t g_u8ADCMEMIndex = 0;

// Array for storing the historic of A/D measures
int16_t g_i16SamplesArray[MAX_SAMPLES];
// Index for storing in the inde
uint8_t g_u8ADCIndex = 0;

// Value of the light intensity
float g_fLighValue;

// An array if global flags
bool g_bGlobalFlags[NUM_FLAGS];


//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////

void main(void) {
	// Call SetUp routine.
	SetUp();

	// InitVars
	InitVars();

    // Initial Blinking
	InitialBlinking();

	// Enable Interruptions
	EnableInterruptions();

	// SetInitialState depending on light value
	SetInitialState();

	while(1) {
		// Wait For Events
		__wfe();

		if(g_bGlobalFlags[LUX_FLAG] == true) {
			// Delete LUX_FLAG
			g_bGlobalFlags[LUX_FLAG] = false;
			/* Obtain lux value from OPT3001 */
			g_fLighValue = OPT3001_getLux();
		}

		if(g_bGlobalFlags[ADC14_FLAG] == true) {
			// Delete ADC14_FLAG
			g_bGlobalFlags[ADC14_FLAG] = false;
			// Get results from ADC14
			FillSamplesArray();

			if(g_bGlobalFlags[Five_Seconds_Reached] == true) {
				ProcessMicData();
			}

		}


	}
}

