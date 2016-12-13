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
uint16_t g_u16GlobalTicks = 0;
uint16_t g_u16LatenceSpeed;

/* ADC results buffer */
static uint16_t g_u16ResultsBuffer[2];

// Graphics Context
Graphics_Context g_GraphicsContext;
// Initialize static variable of sprite class

Graphics_Context* Arena::m_GraphicsContext = &g_GraphicsContext;
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
	// Create the Arena
	Arena TetrisArena;

	// **********************************
	// Create Sprite

	Sprite CurrentSprite;

    while(1){
    	__wfe();


    	if(g_bGlobalFlags[MOVE_RIGHT]) {
    		g_bGlobalFlags[MOVE_RIGHT] = false;

    		if(!TetrisArena.CheckHorizontalCollision(&CurrentSprite,MOVE_RIGHT)){
    			CurrentSprite.Delete();
    			CurrentSprite.MoveRight();
    			CurrentSprite.Paint();
    		}
    	}

    	if(g_bGlobalFlags[MOVE_LEFT]) {
			g_bGlobalFlags[MOVE_LEFT] = false;

			if(!TetrisArena.CheckHorizontalCollision(&CurrentSprite,MOVE_LEFT)){
				CurrentSprite.Delete();
				CurrentSprite.MoveLeft();
				CurrentSprite.Paint();
			}
    	}

    	if(g_bGlobalFlags[MOVE_DOWN] && g_bGlobalFlags[ROTATE_COUNTERCLOCKWISE]) {
			g_bGlobalFlags[ROTATE_COUNTERCLOCKWISE]  = false;

			CurrentSprite.Delete();
			CurrentSprite.RotateCounterClockwise();

			if(TetrisArena.CheckHorizontalCollision(&CurrentSprite,ROTATE_COUNTERCLOCKWISE)){
				CurrentSprite.RotateClockwise();
			}

			CurrentSprite.Paint();

    	}

    	if(g_bGlobalFlags[ROTATE_CLOCKWISE] && g_bGlobalFlags[MOVE_DOWN]) {
    		g_bGlobalFlags[ROTATE_CLOCKWISE] = false;

    		CurrentSprite.Delete();
			CurrentSprite.RotateClockwise();

			if(TetrisArena.CheckHorizontalCollision(&CurrentSprite,ROTATE_CLOCKWISE)){
				CurrentSprite.RotateCounterClockwise();
			}

			CurrentSprite.Paint();
    	}

    	if(g_bGlobalFlags[MOVE_DOWN]){
    		g_bGlobalFlags[MOVE_DOWN] = false;

    		CurrentSprite.Delete();

    		if(TetrisArena.CheckCollision(&CurrentSprite)){
    			TetrisArena.PaintFromLine(TetrisArena.CheckRows());
    			CurrentSprite = Sprite::Sprite();
    		}else{
    			CurrentSprite.MoveDown();
    			CurrentSprite.Paint();
    		}
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
	//       CONFIG BUTTONS
	// ****************************
    ConfigS1ButtonInterrupt();
    ConfigS2ButtonInterrupt();

	// ****************************
	// Re-enable interruptions
	EnableInterruptions();
	// ****************************

	for(uint8_t i=0; i<NUM_FLAGS; i++){
		g_bGlobalFlags[i] = false;
	}

	// Start Music
	//InitMusicArray();


	g_u16LatenceSpeed = 150;



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
	/* Timer32 Interruption
	 *
	 * Sets the MOVE_DOWN FLAG
	 *
	 */
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0;

		g_u16GlobalTicks = (g_u16GlobalTicks+1) % g_u16LatenceSpeed;
		if(g_u16GlobalTicks == 0)
			g_bGlobalFlags[MOVE_DOWN] = true;

		// Change Note
		//ChangeNote();

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

	    	g_bGlobalFlags[MOVE_LEFT] = (g_u16ResultsBuffer[0]<LEFT_TH);
	    	g_bGlobalFlags[MOVE_RIGHT] = (g_u16ResultsBuffer[0]>RIGHT_TH);
	    	g_bGlobalFlags[MOVE_DOWN] = (g_u16ResultsBuffer[1]==0);

	    }
	}

	void PORT5_IRQHandler(void){
			P5->IFG &= ~BIT1;
			g_bGlobalFlags[ROTATE_CLOCKWISE] = true;
	}

	void PORT3_IRQHandler(void){
			P3->IFG &= ~BIT5;
			g_bGlobalFlags[ROTATE_COUNTERCLOCKWISE] = true;
	}
}
