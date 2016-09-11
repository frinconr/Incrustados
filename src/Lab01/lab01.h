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
#define MAX_SAMPLES (5*SAMPLES_PER_SECOND)


#define MISC_COUNTER_MAX 100


// Flags definitions
typedef enum {
	LUX_FLAG,
	ADC14_FLAG,
	Five_Seconds_Reached,
	// Total flags
	NUM_FLAGS
} GlobalFlags;


//////////////////////////////////////////////////////////////////////////////
// Declaring extern variables
//////////////////////////////////////////////////////////////////////////////

// Global counter for timer interrupt
extern uint8_t g_u16TimerCounter_LED;

// Global counter for timer interrupt
extern uint8_t g_u16TimerCounter_ADC14;

// Array for storing the samples of ONE A/D conversion
extern int16_t g_i16ADCResults[NUM_SAMPLES];
extern uint8_t g_u8ADCMEMIndex;

// Array for storing the historic of A/D measures
extern int16_t g_i16SamplesArray[MAX_SAMPLES];

// Index for storing in the
extern uint8_t g_u8ADCIndex;

extern float g_fLighValue;

extern bool g_bGlobalFlags[NUM_FLAGS];

extern int16_t g_i16LastResult;

//////////////////////////////////////////////////////////////////////////////
// Declaring functions
//////////////////////////////////////////////////////////////////////////////

void SetUp();
void InitVars();
void SetInitialState();
void TurnLightOn();
void InitialBlinking();
void FillSamplesArray();
void ProcessMicData();



#endif /* LAB01_H_DEFS */


