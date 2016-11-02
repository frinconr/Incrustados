/*
 * ScreenPainter.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: fabian
 */

#include <ScreenPainter.hpp>

ScreenPainter::ScreenPainter(Graphics_Context* Context) {
	this->a_GraphicsContext = Context;
	// Make the painting area width the maximum
	a_PaintArea.xMin = 0;
	a_PaintArea.xMax = 127;

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
	// Check is we have to do something
	if(this->a_LastValue != this->a_CurrentValue) {
		if(this->a_LastValue < this->a_CurrentValue) {
			// Rectangle should be filled
			// Overwrite the painting
			a_PaintArea.yMin = this->a_LastValue;
			a_PaintArea.yMax = this->a_CurrentValue;

			// Paint
			Graphics_fillRectangleOnDisplay(a_GraphicsContext->display, &a_PaintArea, FILL_COLOR);
		} else {
			// Rectangle should be erased
			// Overwrite the painting
			a_PaintArea.yMax = this->a_LastValue;
			a_PaintArea.yMin = this->a_CurrentValue;

			// Paint
			Graphics_fillRectangleOnDisplay(a_GraphicsContext->display, &a_PaintArea, BACKGROUND_COLOR);
		}
	}
	return (NO_ERR);
}
