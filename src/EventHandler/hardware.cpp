/*
 * hardware.cpp
 *
 *  Created on: Oct 5, 2016
 *      Author: fabian
 */
/* DriverLib Include */
extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
	#include <stdio.h>
}
#include "hardware.hpp"


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
	P2->OUT &= ~(LEDMask % 8);
}

void ConfigP2PWM (){
	// Port P2.5 configured as an output
	P2->DIR |= BIT5;
	// Select primary module function
	P2->SEL0 |= BIT5;
	P2->SEL1 &= ~(BIT5);
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
	P1->OUT |= BIT4;
	P1->REN |= BIT4;
	// Configure as a GPIO
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	// Interrupt on high-to-low transition
	P1->IES |= BIT4;
	// Clear all P1 interrupt flags
	P1->IFG = 0;
	// Enable interrupt for P1.1
	P1->IE |= BIT4;

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
	P1->OUT |= BIT1;
	P1->REN |= BIT1;
	// Configure as a GPIO
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	// Interrupt on high-to-low transition
	P1->IES |= BIT1;
	// Clear all P1 interrupt flags
	P1->IFG = 0;
	// Enable interrupt for P1.1
	P1->IE |= BIT1;

	// Enable Port 1 interrupt on the NVIC
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_EnableIRQ(PORT1_IRQn);
}


//////////////////////////////////////////////////////////////////////////////
// Timers Configuration
//////////////////////////////////////////////////////////////////////////////

void ConfigTimer32 (uint16_t load) {
	//TIMER32_1->LOAD = 0x002DC6C0; //~1s ---> a 3Mhz
	TIMER32_1->LOAD = 0x00000BB8; //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
}

void ConfigTimerA (){
	TIMER_A0->CCR[0] = 30000;            			// PWM Period
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; 	// CCR2 reset/set
    TIMER_A0->CCR[2] = 2000;                 		// CCR2 PWM duty cycle 5% for -90  initial position
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | 		// SMCLK
    				TIMER_A_CTL_ID__2 |				// Divide by 4
            		TIMER_A_CTL_MC__UP |            // Up mode
					TIMER_A_CTL_CLR;                // Clear TAR
}

//////////////////////////////////////////////////////////////////////////////
// ADC14 Configuration
//////////////////////////////////////////////////////////////////////////////

void ConfigADC14(){

	/* Set the core voltage level to VCORE1 */
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

	/* Set 2 flash wait states for Flash bank 0 and 1*/
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

	/* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);

	/* Initializing ADC (ADCOSC/64/8) */
	MAP_ADC14_enableModule();
	MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_32, ADC_DIVIDER_8,0);

	/* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2 (A11, A13, A14)  with no repeat)
		 * with internal 2.5v reference */
	MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
	MAP_ADC14_configureConversionMemory(ADC_MEM0,
			ADC_VREFPOS_AVCC_VREFNEG_VSS,
			ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);

	MAP_ADC14_configureConversionMemory(ADC_MEM1,
			ADC_VREFPOS_AVCC_VREFNEG_VSS,
			ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);

	MAP_ADC14_configureConversionMemory(ADC_MEM2,
			ADC_VREFPOS_AVCC_VREFNEG_VSS,
			ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);

	/* Enabling the interrupt when a conversion on channel 2 (end of sequence)
	 *  is complete and enabling conversions */
	MAP_ADC14_enableInterrupt(ADC_INT2);

	/* Enabling Interrupts */
	MAP_Interrupt_enableInterrupt(INT_ADC14);
	MAP_Interrupt_enableMaster();

	/* Setting up the sample timer to automatically step through the sequence
	 * convert.
	 */
	MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

	/* Triggering the start of the sample */
	MAP_ADC14_enableConversion();
	MAP_ADC14_toggleConversionTrigger();
}


void ConfigScreen(Graphics_Context* Context) {
	// ****************************
	// Screen configure
	// ****************************
	/* Initializes display */
	Crystalfontz128x128_Init();

	/* Set default screen orientation */
	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

	/* Initializes graphics context */
	Graphics_initContext(Context, &g_sCrystalfontz128x128);
	Graphics_setForegroundColor(Context, FILL_COLOR);
	Graphics_setBackgroundColor(Context, BACKGROUND_COLOR);
	GrContextFontSet(Context, &g_sFontFixed6x8);
}

void ChangeScreenOrientation(bool UpPosition) {
	// Change the screen orientation
	if(UpPosition) {
		Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
	} else {
		Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);
	}
}


