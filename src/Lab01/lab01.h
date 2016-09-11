/*
 * lab01.h
 *
 *	This file contains the definitions for the Lab01.
 *
 *  Created on: Aug 20, 2016
 *      Author: Fabian Meléndez Bolaños
 */

#ifndef LAB01_H_DEFS
#define LAB01_H_DEFS

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


// MSP library
#include "msp.h"

#include <stdbool.h>

#include "hardware.h"


//////////////////////////////////////////////////////////////////////////////
// PRODUCT TYPE DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

// Define product types
#define PRODUCT_TYPE_05W 5
#define PRODUCT_TYPE_10W 10
#define PRODUCT_TYPE_15W 15

// Now define the product type
#define PRODUCT_TYPE PRODUCT_TYPE_15W

// Default LED_MASK to be overwritten
#define LED_MASK BIT2

// Custom implementations for each product TYPE
#if PRODUCT_TYPE == PRODUCT_TYPE_05W
	// LED will only use RED
	#define LED_MASK BIT0
#endif /* PRODUCT_TYPE_05W*/

#if PRODUCT_TYPE == PRODUCT_TYPE_10W
	// LED will use blue
	#define LED_MASK BIT1
#endif /* PRODUCT_TYPE_10W */

#if PRODUCT_TYPE == PRODUCT_TYPE_15W
	// LED will use blue
	#define LED_MASK BIT2
#endif /* PRODUCT_TYPE_15W */

//////////////////////////////////////////////////////////////////////////////
// CONSTANTS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

// Light threshold
#define LIGHT_THRESHOLD 50

// Number of blinking iterations
#define BLINKING_ITERATIONS 3

// Loop iterations for blinking delay
#define BLINKING_DELAY 20000

// For the toggle of the LED.
#define TIMERA0_COUNT_01s 100

#define TIMERA0_COUNT_30s 3000

// Number of samples for one A/D conversion
#define NUM_SAMPLES 8

// Samples in the last second (number of A/D conversions)
#define SAMPLES_PER_SECOND  5

// Number of samples to store in samples array (5seconds of samples)
#define MAX_SAMPLES 5*SAMPLES_PER_SECOND


#define MISC_COUNTER_MAX 100


// Flags definitions
#define NUM_FLAGS 1

#define LUX_FLAG 0

//////////////////////////////////////////////////////////////////////////////
// Declaring extern variables
//////////////////////////////////////////////////////////////////////////////

// Global counter for timer interrupt
extern uint16_t g_u16TimerCounter_LED;

// Global counter for timer interrupt
extern uint16_t g_u16TimerCounter_Misc;

// Array for storing the samples of ONE A/D conversion
extern uint16_t g_u16ADCResults[NUM_SAMPLES];

// Array for storing the historic of A/D measures
extern uint16_t g_u16SamplesArray[MAX_SAMPLES];

// Index for storing in the
extern uint8_t g_u8ADCIndex;

extern float g_fLUXValue;

extern bool g_bGlobalFlags[NUM_FLAGS];
//////////////////////////////////////////////////////////////////////////////
// Declaring functions
//////////////////////////////////////////////////////////////////////////////

void SetUp();
void TurnLightOn();
void InitialBlinking();
void FillSamplesArray(uint16_t last_sample);
void ProcessMicData();



#endif /* LAB01_H_DEFS */


