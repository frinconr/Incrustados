/*
 * S2Button.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Felipe
 */

#include "S2Button.hpp"


S2Button::S2Button() {
	// Configure S2 Button
	ConfigS2ButtonInterrupt();


}

uint8_t S2Button::run(void) {

	g_bGlobalFlags[Debounce_Flag_S2] = true;
	TIMER_A0->CCR[2] += 250;
	if(TIMER_A0->CCR[2]>3000){
		TIMER_A0->CCR[2] = 1500;
	}

	return (NO_ERR);
}


