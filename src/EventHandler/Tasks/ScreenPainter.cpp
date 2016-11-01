/*
 * ScreenPainter.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: fabian
 */

#include <ScreenPainter.hpp>

ScreenPainter::ScreenPainter() {
	// TODO Auto-generated constructor stub

}

ScreenPainter::~ScreenPainter() {
	// TODO Auto-generated destructor stub
}


void ScreenPainter::ProcessMessage(Task::Message msj) {
	switch(msj.Type){
		case UPDATE_NEEDED:
			// When receiving a message to paint
			this->Revive();
			break;
		default:
			break;
	}

}

