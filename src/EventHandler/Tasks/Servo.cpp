/*
 * Servo.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: Felipe
 */

#include <Servo.hpp>

Servo::Servo() {
	a_CurrentValue = 2000;
}

Servo::~Servo() {
}

void Servo::ProcessMessage(Task::Message msj) {
	switch(msj.Type){
		case UPDATE_NEEDED:
			this->Revive();
			this->a_CurrentValue = msj.Data;
			break;
		default:
			break;
	}

}

uint8_t Servo::run(void) {


	if(a_CurrentValue > 11000) {
		TIMER_A0->CCR[2] = 3000;
	  } else if (a_CurrentValue < 5000) {
		  TIMER_A0->CCR[2] = 1000;
	  } else {
		  TIMER_A0->CCR[2] = (a_CurrentValue - 2000)/3;
	  }

	return (NO_ERR);
}


