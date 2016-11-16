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
#include "Scheduler.hpp"
#include "LED.hpp"
#include "S1Button.hpp"
#include "S2Button.hpp"
#include "Definitions.hpp"
#include "hardware.hpp"
#include "ScreenPainter.hpp"
#include "Servo.hpp"

//////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////

/* Static task ID counter */
uint8_t Task::m_u8NextTaskID = 0;
/* System ticks counter */
volatile static uint64_t g_u64SystemTicks = 0;
/* Global flags */
bool g_bGlobalFlags[NUM_FLAGS];
/* Scheduler instance */
Scheduler g_MainScheduler;

/* Graphics Context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t g_u8ResultsBuffer[3];

/* Global Servo task */
Servo g_sServo = Servo::Servo();
ScreenPainter g_sPainter = ScreenPainter::ScreenPainter(&g_sContext,&g_MainScheduler, &g_sServo);

//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
    // Start Task Inactive, add to scheduler as a delayed task

    //g_MainScheduler.attach(&ButtonTaskS1, DebounceTime, true);
    g_MainScheduler.Attach(&g_sPainter, 1);
    g_MainScheduler.Attach(&g_sServo, DebounceTime, true);

    Setup();
    //g_MainScheduler.attach(&BlinkLED1, LED1Latency);

    while(1){
    	__wfe();
        if(g_u64SystemTicks != g_MainScheduler.m_u64Ticks)
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
	//ConfigP1LED();

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
	ConfigTimerA();

	// ****************************
	ConfigADC14();

	// ****************************
	// Re-enable interruptions
	EnableInterruptions();
	// ****************************

	// ****************************
	// Initialize global flags
	// ****************************

	g_bGlobalFlags[Debounce_Flag_S1] = true;
	g_bGlobalFlags[Debounce_Flag_S2] = true;
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


	void PORT1_IRQHandler(void) {


		if(P1->IFG & BIT1){
			P1->IFG &= ~BIT1;
				if(g_bGlobalFlags[Debounce_Flag_S1]) {
					g_bGlobalFlags[Debounce_Flag_S1] = false;
					//ButtonTaskS1.Revive();
				}

		}
		if(P1->IFG & BIT4){
					P1->IFG &= ~BIT4;
						if(g_bGlobalFlags[Debounce_Flag_S2]) {
							g_bGlobalFlags[Debounce_Flag_S2] = false;
							//ButtonTaskS2.Revive();
						}
				}
	}

	void ADC14_IRQHandler(void)
	{
	    uint64_t l_u64Status;

	    l_u64Status = MAP_ADC14_getEnabledInterruptStatus();
	    MAP_ADC14_clearInterruptFlag(l_u64Status);

	    /* ADC_MEM2 conversion completed */
	    if(l_u64Status & ADC_INT2)
	    {
	        /* Store ADC14 conversion results */
	        g_u8ResultsBuffer[0] = ADC14_getResult(ADC_MEM0);
	        g_u8ResultsBuffer[1] = ADC14_getResult(ADC_MEM1);
	        g_u8ResultsBuffer[2] = ADC14_getResult(ADC_MEM2);

	        g_sPainter.SetValue(g_u8ResultsBuffer[2], g_u8ResultsBuffer[1]);
	    }
	}
}
