/*
 * hardware.c
 *
 *  Created on: Sep 10, 2016
 *      Author: Fabián Meléndez
 */


#include "hardware.h"


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


//////////////////////////////////////////////////////////////////////////////
// Ports Configs
//////////////////////////////////////////////////////////////////////////////

void ConfigP2LEDOutput(const uint16_t LEDMask) {
	// - - - - - - - - - - - - - -
	// P2 Config
	// - - - - - - - - - - - - - -
	// This is the LED port

	// - Configure P2.LED_MASK as output
	P2->DIR |= LEDMask;
	// Clean Port0
	P2->OUT = 0;
}

void ConfigP1ButtonInterrupt() {
	// - - - - - - - - - - - - - -
	// P1 Config
	// - - - - - - - - - - - - - -
	// This is the button interrupt port

	// - Configure P1.4 (S2) as input

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
// Ports Configs
//////////////////////////////////////////////////////////////////////////////

void ConfigTimerA0UpMode (const uint16_t LoadValue) {
	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.

	// Set clock as SMCLK (3MHz), divider by 4, and enable interruption
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__2 | TIMER_A_CTL_IE;
	// Load value of 15000 = (0x3A98)
	TIMER_A0->CCR[0] = LoadValue;
	// Set mode as: UpMode (counts to CCR, interrupts and counter goes back to 0).
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	// Set interruption in NVIC
	NVIC_SetPriority(TA0_N_IRQn,1);
	NVIC_EnableIRQ(TA0_N_IRQn);
}

//////////////////////////////////////////////////////////////////////////////
// I2C Configurations
//////////////////////////////////////////////////////////////////////////////

void ConfigLUXI2C () {
	/* Initialize I2C communication */
	Init_I2C_GPIO();
	I2C_init();

	/* Initialize OPT3001 digital ambient light sensor */
	OPT3001_init();

	__delay_cycles(100000);
}
