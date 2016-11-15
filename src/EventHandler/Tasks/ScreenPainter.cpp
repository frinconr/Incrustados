/*
 * ScreenPainter.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: fabian
 */

#include <ScreenPainter.hpp>

ScreenPainter::ScreenPainter(Graphics_Context* Context, Scheduler* scheduler, Task* receiver) {
	// Save Context object pointer
	this->a_GraphicsContext = Context;

	// Add attributes
	m_Scheduler = scheduler;
	m_Receiver = receiver;

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

uint8_t  ScreenPainter::run(void) {
	// Check
	if(ValueChanged) {
		// Clean flag
		a_LastValue = a_CurrentValue;
		this->ValueChanged = false;

		if(a_Measure > 11000) {
			a_CurrentValue = 0;
		} else if (a_Measure < 5000) {
			a_CurrentValue = 127;
		} else {
			a_CurrentValue = (int) -1*(a_Measure)*127/6000+232;
		}

		// Change orientation
		ChangeScreenOrientation(a_PositionUp);
		m_Scheduler->AddMessage(this, m_Receiver, 0, a_Measure);
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

void ScreenPainter::SetValue(uint16_t Value, uint16_t orientation) {

	a_Measure = Value;
	ValueChanged = true;
	a_PositionUp = (orientation < ORIENTATION_THRESHOLD);
}
