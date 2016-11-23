/*
 * main.cpp
 *
 * 	This is the main file for the proyect. Contains the global variable
 * 	functions and the Scheduler and Task objects.
 *
 *  Created on: Nov 14, 2016
 *      Author: Felipe Rincon
 *      		Fabian Melendez
 */


#define __NOP __nop

/* Local Includes */
#include "main.hpp"

/* Global Variables */
uint32_t g_u64GlobalTicks = 0;

// Graphics Context
Graphics_Context g_GraphicsContext;

//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
	Sprite::eSpriteTypes SpriteType = Sprite::oBlock;
	// Create Sprite
	Sprite CurrentSprite = Sprite::Sprite(SpriteType);
    Setup();

    while(1){
    	__wfe();
    };
}

// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void)
{
	// Seed random number generator
	srand (time(NULL));

	// ****************************
	//         DEVICE CONFIG
	// ****************************
	// - Disable Interruptions
	// - Disable WDT
	DisableInterruptions();
	DisableWDT();

	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	ConfigP1LED();

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	ConfigTimer32(3000); // 3000 = 1

	// ****************************
	//       CONFIG SCREEN
	// ****************************
    ConfigScreen(&g_GraphicsContext);

	// ****************************
	// Re-enable interruptions
	EnableInterruptions();
	// ****************************

	// ****************************
	// Initialize global flags
	// ****************************
	return;
}

//////////////////////////////////////////////////////////////////////////////
// Interruptions
//////////////////////////////////////////////////////////////////////////////

extern "C"
{
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0;
		g_u64GlobalTicks++;
		return;
	}
}
