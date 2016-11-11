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


//////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////

/* Static task ID counter */
uint8_t Task::m_u8NextTaskID = 0;
/* System ticks counter */
volatile static uint64_t SystemTicks = 0;
/* Global flags */
bool g_bGlobalFlags[NUM_FLAGS];
/* Scheduler instance */
Scheduler g_MainScheduler;

/* Graphics Context */
Graphics_Context g_sContext;

/* BlinkingLED1 Task */
LED BlinkLED1 = LED::LED(LED2Mask);
/* Button Task */
S1Button ButtonTaskS1(&g_MainScheduler, &BlinkLED1);
S2Button ButtonTaskS2;

/* ADC results buffer */
static uint16_t resultsBuffer[3];

ScreenPainter g_sPainter = ScreenPainter::ScreenPainter(&g_sContext);
//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////


void main(void)
{
    // Start Task Inactive, add to scheduler as a delayed task

    g_MainScheduler.attach(&ButtonTaskS1, DebounceTime, true);
    g_MainScheduler.attach(&g_sPainter, 1);
    ButtonTaskS1.Kill();
    g_MainScheduler.attach(&ButtonTaskS2, DebounceTime, true);
    ButtonTaskS2.Kill();


    // LED BlinkLED2 = LED::LED(LED2Mask);

    Setup();
    g_MainScheduler.attach(&BlinkLED1, LED1Latency);

    while(1){
    	__wfe();
        if(SystemTicks != g_MainScheduler.ticks)
        {
        	g_MainScheduler.ticks = SystemTicks;
        	g_MainScheduler.run();
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
		SystemTicks++;
		return;
	}


	void PORT1_IRQHandler(void) {


		if(P1->IFG & BIT1){
			P1->IFG &= ~BIT1;
				if(g_bGlobalFlags[Debounce_Flag_S1]) {
					g_bGlobalFlags[Debounce_Flag_S1] = false;
					ButtonTaskS1.Revive();
				}

		}
		if(P1->IFG & BIT4){
					P1->IFG &= ~BIT4;
						if(g_bGlobalFlags[Debounce_Flag_S2]) {
							g_bGlobalFlags[Debounce_Flag_S2] = false;
							ButtonTaskS2.Revive();
						}
				}
	}

	void ADC14_IRQHandler(void)
	{
	    uint64_t status;

	    status = MAP_ADC14_getEnabledInterruptStatus();
	    MAP_ADC14_clearInterruptFlag(status);

	    /* ADC_MEM2 conversion completed */
	    if(status & ADC_INT2)
	    {
	        /* Store ADC14 conversion results */
	        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
	        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);
	        resultsBuffer[2] = ADC14_getResult(ADC_MEM2);

	        g_sPainter.SetValue(resultsBuffer[2], resultsBuffer[1]);
	    }
	}
}
