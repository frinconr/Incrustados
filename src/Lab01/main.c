/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_21_00_05 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Authors: Fabián Meléndez / Felipe Rincón
*******************************************************************************/
/* DriverLib Includes */
// #include "driverlib.h"
#include <driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// MSP library
#include "msp.h"

// Lab01 constant definitions

#include "hardware.h"
#include "lab01.h"
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

float g_fLighValue;

bool g_bGlobalFlags[NUM_FLAGS];

int16_t g_i16LastResult = 2;



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

