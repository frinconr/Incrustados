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
//////////////////////////////////////////////////////////////////////////////
// PRODUCT TYPE DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

// Define product types
#define PRODUCT_TYPE_05W 5
#define PRODUCT_TYPE_10W 10
#define PRODUCT_TYPE_15W 15

// Now define the product type
#define PRODUCT_TYPE PRODUCT_TYPE_05W


#define LED_MASK BIT0

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
	// LED will use green
	#define LED_MASK BIT2
#endif /* PRODUCT_TYPE_15W */

//////////////////////////////////////////////////////////////////////////////
// CONSTANTS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

// Number of blinking iterations
#define BLINKING_ITERATIONS 3
// Loop iterations for blinking delay
#define BLINKING_DELAY 10000

// For the toggle of the LED.
#define TIMERA0_COUNT 45

#endif /* LAB01_H_DEFS */