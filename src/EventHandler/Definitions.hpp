/*
 * Definitions.hpp
 *
 *	Global definitions file. Contains interruptions priorities,
 *	LED Masks, timing and constant values.
 *	Also contains the global flags definitions.
 *
 *  Created on: Oct 5, 2016
 *      Author: Fabian Melendez
 *      		Felipe Rincon
 */

#ifndef DEFINITIONS_HPP_
#define DEFINITIONS_HPP_

// Needed (?)
#define __NOP __nop

//////////////////////////////////////////////////////////////////////////////
// Interrupt priorities
//////////////////////////////////////////////////////////////////////////////

#define TimerA0_Priority      1
#define S2Button_Priority     2
#define Timer32_Priority      0

//////////////////////////////////////////////////////////////////////////////
// LED MASKs
//////////////////////////////////////////////////////////////////////////////

#define LED1Mask BIT0
#define LED2Mask BIT2

//////////////////////////////////////////////////////////////////////////////
// Time definitions
//////////////////////////////////////////////////////////////////////////////

#define LED1Latency 600
#define DebounceTime 20


#define MAX_MSJS 20

//////////////////////////////////////////////////////////////////////////////
// Global Flags
//////////////////////////////////////////////////////////////////////////////

typedef enum {
	Debounce_Flag_S1,
	Debounce_Flag_S2,
	// Total flag count
	NUM_FLAGS
} eGlobalFlags;

//////////////////////////////////////////////////////////////////////////////
// Color Definitions
//////////////////////////////////////////////////////////////////////////////

#define FILL_COLOR 				0x00FF
#define BACKGROUND_COLOR		0x8A22

// THRESHOLDS
#define ORIENTATION_THRESHOLD   7500

#define MAX_ACC_VALUE 	11000
#define MIN_ACC_VALUE	5000

// ***************************************************************************
// Global variable declarations
// ***************************************************************************

extern bool g_bGlobalFlags[NUM_FLAGS];

#endif /* DEFINITIONS_HPP_ */
