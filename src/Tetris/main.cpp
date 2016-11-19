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

/* MSPWare definitions */
#include "msp.h"

/* DriverLib Include */
extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
	#include <stdio.h>
}

/* Local Includes */
#include "main.hpp"


//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
    Setup();

    while(1){
    	__wfe();
        if()
        {
        	g_MainScheduler.m_u64Ticks = g_u64SystemTicks;
        	g_MainScheduler.Run();
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
	// Configure PWM
	// ****************************
	ConfigP2PWM();

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	ConfigTimer32(3000); // 3000 = 1ms


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
		g_u64SystemTicks++;
		return;
	}
}
