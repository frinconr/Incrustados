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

#define 	MAX_HEIGHT 		128
#define 	MIN_HEIGHT		128

#define 	MIN_ARENA_X		28
#define 	MAX_ARENA_X		128

#define 	ARENA_WIDTH		100
#define 	ARENA_HEIGHT	120

#define 	MIN_ARENA_Y		4
#define 	MAX_ARENA_Y		124

#define 	SEGMENT_WIDTH	10
#define		SEGMENT_HEIGHT	10

#define 	NUM_X_SQUARES 	10
#define		NUM_Y_SQUARES	12

#define 	VERTICAL_JUMP	2

#define		LEFT_TH			4000
#define		RIGHT_TH	   	12000

#define 	MIN_SCORE_X		0
#define 	MAX_SCORE_X		27


//////////////////////////////////////////////////////////////////////////////
// Global Flags
//////////////////////////////////////////////////////////////////////////////

typedef enum {

	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_DOWN,
	ROTATE_CLOCKWISE,
	ROTATE_CONTERCLOCKWISE,
	CHANGE_SPRITE,
	// Total flag count
	NUM_FLAGS
} eGlobalFlags;

//////////////////////////////////////////////////////////////////////////////
// Color Definitions
//////////////////////////////////////////////////////////////////////////////

#define BACKGROUND_COLOR	0xFFFF
#define FILL_COLOR			0x0000
#define ARENA_COLOR			0xFD20

// THRESHOLDS

// ***************************************************************************
// Global variable declarations
// ***************************************************************************

extern bool g_bGlobalFlags[NUM_FLAGS];

#endif /* DEFINITIONS_HPP_ */
