/*
 * ScreenPainter.cpp
 *
 * 		This file contains the source code for the ScreenPainter Task. Its in
 * 		charge of painting the LCD screen, with the value received from the
 * 		accelerometer. Also it sends the latest value to the Servo Task.
 *
 *  Created on: Nov 1, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */

#include <ScreenPainter.hpp>

ScreenPainter::ScreenPainter(Graphics_Context* I_Context,
		                     Scheduler* i_Scheduler,
		                     Task* i_tRreceiver) {
	// Save Context object pointer
	this->a_GraphicsContext = I_Context;

	// Add attributes
	m_Scheduler = i_Scheduler;
	m_Receiver = i_tRreceiver;

	// Configure the painting
	ConfigScreen(a_GraphicsContext);

	// Make the painting area width the maximum
	a_PaintArea.xMin = 0;
	a_PaintArea.xMax = 127;

	ValueChanged = false;

	// Set painting values to half the screen
	a_CurrentValue=64;
	a_LastValue=64;

	// Paint the lower part of the screen
	a_PaintArea.yMax = 64;
	a_PaintArea.yMin = 0;

	// Paint the lowert part of the screen.
	Graphics_fillRectangleOnDisplay(a_GraphicsContext->display, &a_PaintArea, FILL_COLOR);

	// Paint the upper part of the screen
	a_PaintArea.yMax = 127;
	a_PaintArea.yMin = 64;

	// Paint the lowert part of the screen.
	Graphics_fillRectangleOnDisplay(a_GraphicsContext->display, &a_PaintArea, BACKGROUND_COLOR);
}

ScreenPainter::~ScreenPainter() {
	// TODO Auto-generated destructor stub
}


void ScreenPainter::ProcessMessage(Task::Message msj) {
	switch(msj.Type){
		case UPDATE_NEEDED:
			// When receiving a message to paint
			this->Revive();
			// Update the last value
			this->a_LastValue = this->a_CurrentValue;
			// Get the new value
			this->a_CurrentValue = msj.Data;
			break;
		default:
			break;
	}

}

/*
 * This function executes the painting. First it checks if we have a new value if so,
 * it calculates the height at which the screen must be painted (mapping the ADC14
 * measure to a screen pixel height (0-127).
 */
uint8_t  ScreenPainter::Run(void) {
	// Check
	if(ValueChanged) {
		// Clean flag
		a_LastValue = a_CurrentValue;
		this->ValueChanged = false;

		// Saturate the height option
		if(m_Measure > MAX_ACC_VALUE) {
			a_CurrentValue = 0;
		} else if (m_Measure < MIN_ACC_VALUE) {
			a_CurrentValue = 127;
		} else {
			a_CurrentValue = (int) -1*(m_Measure)*127/6000+232;
		}

		// Change orientation
		ChangeScreenOrientation(a_PositionUp);

		// Send reciever a message with the measure.
		m_Scheduler->AddMessage(this, m_Receiver, 0, m_Measure);
	}
	// Check if we have to do something
	if(a_LastValue != a_CurrentValue) {
		if(a_LastValue < a_CurrentValue) {
			// Rectangle should be filled
			// Overwrite the painting
			a_PaintArea.yMin = a_LastValue;
			a_PaintArea.yMax = a_CurrentValue;

			// Paint
			Graphics_fillRectangleOnDisplay(a_GraphicsContext->display, &a_PaintArea, FILL_COLOR);
		} else {
			// Rectangle should be erased
			// Overwrite the painting
			a_PaintArea.yMin = a_CurrentValue;
			a_PaintArea.yMax = a_LastValue;

			// Paint
			Graphics_fillRectangleOnDisplay(a_GraphicsContext->display, &a_PaintArea, BACKGROUND_COLOR);
		}
	}
	return (NO_ERR);
}

/*
 * This function is used to get the value from the ADC14. It receives a value with the Z axis
 * measure and a value with the Y axis measure. With this, we get the measure and the orien-
 * tation of the screen.
 */
void ScreenPainter::SetValue(uint16_t Value, uint16_t Orientation) {
	//
	m_Measure = Value;
	ValueChanged = true;
	a_PositionUp = (Orientation < ORIENTATION_THRESHOLD);
}
