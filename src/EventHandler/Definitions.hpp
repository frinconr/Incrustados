/*
 * Definitions.hpp
 *
 *  Created on: Oct 5, 2016
 *      Author: fabian
 */

#ifndef DEFINITIONS_HPP_
#define DEFINITIONS_HPP_

// Needed (?)
#define __NOP __nop

// Interrupt priorities
#define TimerA0_Priority      1
#define S2Button_Priority     2
#define Timer32_Priority      0

// LED MASK
#define LED1Mask BIT0
#define LED2Mask BIT2

#define DebounceTime 5

#define MAX_MSJS 20

typedef enum {
	Debounce_Flag,
	// Total flag count
	NUM_FLAGS
} eGlobalFlags;

// Global variable declarations
extern bool g_bGlobalFlags[NUM_FLAGS];

#endif /* DEFINITIONS_HPP_ */