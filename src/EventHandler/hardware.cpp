/*
 * hardware.cpp
 *
 *  Created on: Oct 5, 2016
 *      Author: fabian
 */

#include "Definitions.hpp"
#include "msp.h"

/** Enable Interruptions
 *	- Enables all interruptions
*/
void EnableInterruptions() {
	// Enable all interruptions
	__enable_irq();
}

/** Disable Interruptions
 *	- Enables all interruptions
*/
void DisableInterruptions() {
	// Disable Interruptions
	__disable_irq();
}


/** Disable Watch Dog Timer
 *	- Disbales WDT
*/
void DisableWDT() {
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
}

//////////////////////////////////////////////////////////////////////////////
// Ports Configs
//////////////////////////////////////////////////////////////////////////////

void ConfigP1LED() {
	// - - - - - - - - - - - - - -
	// P1 Config
	// - - - - - - - - - - - - - -
	// This is the LED port for P1

	// - Configure P1.0 as output
	P1->DIR |= BIT0;
	// Clean Port0
	P1->OUT = 0;
}

void ConfigP2LED(const uint16_t LEDMask) {
	// - - - - - - - - - - - - - -
	// P2 Config
	// - - - - - - - - - - - - - -
	// This is the LED port

	// - Configure P2.LED_MASK as output,
	// Only 3 bits are allowed (RGB)
	P2->DIR |= (LEDMask % 8);
	// Clean Port0
	P2->DIR &= ~(LEDMask % 8);
}

void ConfigS2ButtonInterrupt() {
	// - - - - - - - - - - - - - -
	// P1 Config
	// - - - - - - - - - - - - - -
	// This is the button interrupt port

	// - Configure P1.4 (S2) as input

	// Configuring P1.4 (switch) as input with pull-up
	// resistor. Rest of pins are configured as output low.
	// Notice intentional '=' assignment since all P1 pins are being
	// deliberately configured

	// Configure P1.4 as input
	P1->DIR &= ~(uint8_t) BIT4;
	// Enable pull-up resistor (P1.1 output high)
	P1->OUT = BIT4;
	P1->REN = BIT4;
	// Configure as a GPIO
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	// Interrupt on high-to-low transition
	P1->IES = BIT4;
	// Clear all P1 interrupt flags
	P1->IFG = 0;
	// Enable interrupt for P1.1
	P1->IE = BIT4;

	// Enable Port 1 interrupt on the NVIC
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_EnableIRQ(PORT1_IRQn);
}

void ConfigS1ButtonInterrupt() {
	// - - - - - - - - - - - - - -
	// P1 Config
	// - - - - - - - - - - - - - -
	// This is the button interrupt port

	// - Configure P1.1 (S1) as input

	// Configuring P1.1 (switch) as input with pull-up
	// resistor. Rest of pins are configured as output low.
	// Notice intentional '=' assignment since all P1 pins are being
	// deliberately configured

	// Configure P1.1 as input
	P1->DIR &= ~(uint8_t) BIT1;
	// Enable pull-up resistor (P1.1 output high)
	P1->OUT = BIT1;
	P1->REN = BIT1;
	// Configure as a GPIO
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	// Interrupt on high-to-low transition
	P1->IES = BIT1;
	// Clear all P1 interrupt flags
	P1->IFG = 0;
	// Enable interrupt for P1.1
	P1->IE = BIT1;

	// Enable Port 1 interrupt on the NVIC
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_EnableIRQ(PORT1_IRQn);
}


//////////////////////////////////////////////////////////////////////////////
// Timers Config
//////////////////////////////////////////////////////////////////////////////

void ConfigTimer32 (uint32_t load) {
	//TIMER32_1->LOAD = 0x002DC6C0; //~1s ---> a 3Mhz
	TIMER32_1->LOAD = 0x00000BB8; //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
}
