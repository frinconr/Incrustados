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

/* ADC results buffer */
static uint16_t g_u16ResultsBuffer[2];

// Graphics Context
Graphics_Context g_GraphicsContext;
// Initialize static variable of sprite class
Graphics_Context* Sprite::m_GraphicsContext = &g_GraphicsContext;

// Global Flags
bool g_bGlobalFlags[NUM_FLAGS];

//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// **********************************
	// SetUp
	// **********************************
	Setup();
	// **********************************


	// **********************************
	// Create Sprite
	Sprite CurrentSprite (Sprite::lBlock);


    while(1){
    	__wfe();

    	if(g_bGlobalFlags[CHANGE_SPRITE]) {
    		g_bGlobalFlags[CHANGE_SPRITE] = false;

    		// **********************************
			// Create Sprite
			CurrentSprite.Delete();
			// **********************************

    		if(g_u64GlobalTicks < 20) {
    			CurrentSprite.MoveDown();
    		} else {
    			CurrentSprite.RotateClockwise();
    		}

    		CurrentSprite.Paint();
    		g_u64GlobalTicks++;
    	}
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
	ConfigTimer32(65530); // 30000 = 1ms

	// ****************************
	//       CONFIG SCREEN
	// ****************************
    ConfigScreen(Sprite::m_GraphicsContext);

	// ****************************
	//       CONFIG ADC14
	// ****************************
    ConfigADC14();

	// ****************************
	// Re-enable interruptions
	EnableInterruptions();
	// ****************************

	g_bGlobalFlags[NUM_FLAGS] = true;
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
		g_bGlobalFlags[CHANGE_SPRITE] = true;
		return;
	}
	/* This interrupt is fired whenever a conversion is completed and placed in
	 * ADC_MEM1. This signals the end of conversion and the results array is
	 * grabbed and placed in resultsBuffer */
	void ADC14_IRQHandler(void)
	{
	    uint64_t status;

	    status = MAP_ADC14_getEnabledInterruptStatus();
	    MAP_ADC14_clearInterruptFlag(status);

	    /* ADC_MEM1 conversion completed */
	    if(status & ADC_INT1)
	    {
	        /* Store ADC14 conversion results */
	    	g_u16ResultsBuffer[0] = ADC14_getResult(ADC_MEM0);
	    	g_u16ResultsBuffer[1] = ADC14_getResult(ADC_MEM1);


	    }
	}
}
