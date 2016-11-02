#define __NOP __nop

/* MSPWare definitions */
#include "msp.h"

/* DriverLib Include */
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>

/* Local Includes */
#include "main.hpp"
#include "Scheduler.hpp"
#include "LED.hpp"
#include "S1Button.hpp"
#include "S2Button.hpp"
#include "Definitions.hpp"
#include "hardware.hpp"


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

// Global button task used by scheduler
S1Button ButtonTaskS1;
S2Button ButtonTaskS2;

/* Graphics Context */
Graphics_Context g_sContext;

/* BlinkingLED1 Task */
LED BlinkLED1 = LED::LED(LED2Mask);
/* Button Task */
S1Button ButtonTask(&g_MainScheduler, &BlinkLED1);


//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////

>>>>>>> 3d25f86de88fcaff7fbbe99dd3ca171f76447566

void main(void)
{
    // Start Task Inactive, add to scheduler as a delayed task

    g_MainScheduler.attach(&ButtonTaskS1, DebounceTime, true);
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

	// - Re-enable interrupts
	__disable_irq();
	TIMER32_1->LOAD = 0x002DC6C0; //~1s ---> a 3Mhz
	// TIMER32_1->LOAD = 0x00000BB8; //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
	__enable_irq();

	ConfigTimer32(3000); // 3000 = 1ms
	ConfigTimerA();

	// ****************************
	/* Initializes display */
	Crystalfontz128x128_Init();

	/* Set default screen orientation */
	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
	GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

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
}
