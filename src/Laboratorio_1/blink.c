/******************************************************************************
*
* MSP432 blink.c template - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////////////////////////////////

#include "msp.h"
#include "lab01.h"

//////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////
volatile uint16_t g_u16TimerCounter = 0;
volatile uint16_t g_u16TimerCounter_ADC14 = 0;


// Array for storing the samples of ONE A/D conversion
volatile int16_t g_i16ADCResults[NUM_SAMPLES];

// Array for storing the historic of A/D measures
volatile uint16_t g_u16SamplesArray[MAX_SAMPLES];

// Index for storing in the
volatile uint8_t g_u8ADCIndex;

// Index for storing data from ADC14 conversion
volatile uint8_t g_u8ADCMEMIndex = 0;

// Index for calculating maximum on ADC14 data
volatile uint8_t g_i8GetMaxIndex;

// Most significant value of ADC14 results
volatile int16_t g_i16MaxADCResult;



//////////////////////////////////////////////////////////////////////////////
// Function Definitions
//////////////////////////////////////////////////////////////////////////////


/** SetUp
 *
 * Configures the device.
 * - General device config.
 * - Ports configuration
 * - Interruptions Configuration.
 */
void SetUp() {
	// Disable Interruptions
	__disable_irq();

    // ****************************
    //         DEVICE CONFIG
    // ****************************
    // - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // ****************************
    //         PORTS CONFIG
    // ****************************

	// - - - - - - - - - - - - - -
	// P2 Config
	// - - - - - - - - - - - - - -
	// This is the LED port

	// - Configure P2.LED_MASK as output
	P2->DIR |= LED_MASK;
	// Clean Port0
	P2->OUT = 0;

	// - - - - - - - - - - - - - -
	// P4 Config
	// - - - - - - - - - - - - - -
	// This is the microphone port
	P4->SEL1 |= BIT3;
	P4->SEL0 |= BIT3;



	// Enable ADC interrupt on the NVIC
	NVIC_SetPriority(ADC14_IRQn,3);
	NVIC_EnableIRQ(ADC14_IRQn);

	// Turn on ADC14, extend sampling time to avoid overflow of results
	/*
	 * ADC14_CTL0_ON 					Turns on de ADC14
	 * ADC14_CTL0_MSC 					Enables ADC14 multiple sample and conversion
	 * ADC14_CTL0_SHT0__192 			Sets the ADC14 sample-and-hold time
	 * ADC14_CTL0_SHP 					SAMPCON signal is sourced from the sampling timer
	 * ADC14_CTL0_CONSEQ_3 				ADC14 conversion sequence mode select-> Repeat-single-channel
	 */
	ADC14->CTL0 = 	ADC14_CTL0_ON |
	            	ADC14_CTL0_MSC |
					ADC14_CTL0_SHT0__192 |
					ADC14_CTL0_SHP |
					ADC14_CTL0_CONSEQ_3;

	ADC14->CTL1 = ADC14_CTL1_RES_3 | ADC14_CTL1_DF;

	ADC14->MCTL[0] |= ADC14_MCTLN_INCH_10;    // ref+=AVcc, channel = A10



	ADC14->IER0 = ADC14_IER0_IE3;           // Enable ADC14IFG.3
	SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

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



	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.

	// Set clock as SMCLK (3MHz), divider by 4, and enable interruption
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__2 | TIMER_A_CTL_IE;
	// Load value of 15000 = (0x3A98)
	TIMER_A0->CCR[0] = 0x3A98;
	// Set mode as: UpMode (counts to CCR, interrupts and counter goes back to 0).
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	// Set interruption in NVIC
	NVIC_SetPriority(TA0_N_IRQn,1);
	NVIC_EnableIRQ(TA0_N_IRQn);


}

/** Enable Interruptions
 *	- Enables all interruptions
*/
void EnableInterruptions() {
	// Enable all interruptions
	__enable_irq();
}


/** InitialBlinking
 *
 * Creates an initial blinking to confirm correct product configuration.
 * - Blinks BLINKING_ITERATIONS
 * - Using a loop delay of BLINKING_DELAY
 * - LED color/power is defined by LED_MASK
 */
void InitialBlinking() {
	// Counter for blinking iterations
	uint8_t l_u8IterationCounter;
	// Counter for delay between iterations
	uint16_t l_u16DelayCounter;

	for(l_u8IterationCounter = 0; l_u8IterationCounter < 2*BLINKING_ITERATIONS-1; l_u8IterationCounter++) {
		// Toggle P2
		P2->OUT ^= LED_MASK;

		// Blinking delay
		for(l_u16DelayCounter = 0; l_u16DelayCounter < BLINKING_DELAY; l_u16DelayCounter++);
	}
}


/* FillSamplesArray
 *
 * Uses the last_sample argument and
 *
 */

//////////////////////////////////////////////////////////////////////////////
// Interruptions
//////////////////////////////////////////////////////////////////////////////

// **********************************
// Interrupt service routine for
// Timer A0
//
// T = 1 ms
// **********************************
void TA0_0_ISR(void) {
	// Clear interruption flag
	TIMER_A0->CTL ^= TIMER_A_CTL_IFG;

	// - Divide the clock further, to achieve human readable times.
	if(g_u16TimerCounter > 0){
		// - Toggle P2.00
		P2->OUT ^= LED_MASK;
		g_u16TimerCounter--;

	} else {
		P2->OUT = 0;
    }
	// Activate ADC14 conversion each 200ms
	g_u16TimerCounter_ADC14 = (g_u16TimerCounter_ADC14 + 1)  % 20;
	if(g_u16TimerCounter_ADC14 == 0){
		ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC;
	}


	return;
}


// **********************************
// Interrupt service routine for
// Timer A0
//
// T = 1 ms
// **********************************
/* Port1 ISR */
void S1_PORT1_ISR(void)
{
	// Clear interrupt flag
	P1->IFG &= ~BIT1;

	// This will help with the debouncing
	if(g_u16TimerCounter == 0) {
		// Preload timing for TA0_0_ISR to turn LED on
		g_u16TimerCounter = TIMERA0_COUNT_01s;
	}
}


// **********************************
// Interrupt service routine for
// ADC14
// **********************************
void ADC14_IRQHandler(void)
{


    if (ADC14->IFGR0 & ADC14_IFGR0_IFG3)
    {
    	g_i16ADCResults[g_u8ADCMEMIndex] = ADC14->MEM[0]; // Move A0 results, IFG is cleared
        g_u8ADCMEMIndex++;

        if(g_u8ADCMEMIndex == NUM_SAMPLES){

        	g_u8ADCMEMIndex 	= 0;
        	g_i16MaxADCResult 	= 0;

        	for(g_i8GetMaxIndex=0;g_i8GetMaxIndex<NUM_SAMPLES;g_i8GetMaxIndex++){
        		if(g_i16MaxADCResult < abs(g_i16ADCResults[g_i8GetMaxIndex]) ){
        			g_i16MaxADCResult = abs(g_i16ADCResults[g_i8GetMaxIndex]);
        		}
        	}
        	// **** End Conversion Flag ***
        }else{
        	ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC;
        }

        __no_operation();                   							// Set Breakpoint1 here
        // Start conversion-software trigger

    }
}

//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////

void main(void) {
	// Call SetUp routine.
	SetUp();

    // Initial Blinking
	InitialBlinking();

	// Enable Interruptions
	EnableInterruptions();

	while(1) {
		// Wait For Events
		__wfe();
	}
}


