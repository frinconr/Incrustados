#define __NOP __nop
#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "LED.hpp"
#include "S1Button.hpp"
#include "S2Button.hpp"
#include "Definitions.hpp"
#include "hardware.hpp"


// Static task ID counter
uint8_t Task::m_u8NextTaskID = 0;
// System ticks counter
volatile static uint64_t SystemTicks = 0;
// Global flags
bool g_bGlobalFlags[NUM_FLAGS];
// Scheduler instance
Scheduler g_MainScheduler;
// Global button task used by scheduler
S1Button ButtonTaskS1;
S2Button ButtonTaskS2;

void main(void)
{
    LED BlinkLED1 = LED::LED(LED1Mask);

    // Start Task Inactive, add to scheduler as a delayed task
    g_MainScheduler.attach(&ButtonTaskS1, DebounceTime, true);
    ButtonTaskS1.Kill();
    g_MainScheduler.attach(&ButtonTaskS2, DebounceTime, true);
    ButtonTaskS2.Kill();

    // LED BlinkLED2 = LED::LED(LED2Mask);

    Setup();
    g_MainScheduler.attach(&BlinkLED1, 10, true);

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
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	P1->DIR |= BIT0;

	ConfigP2PWM();
	ConfigTimerA();

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
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


	// Initialize flags
	g_bGlobalFlags[Debounce_Flag_S1] = true;
	g_bGlobalFlags[Debounce_Flag_S2] = true;
	return;
}

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
