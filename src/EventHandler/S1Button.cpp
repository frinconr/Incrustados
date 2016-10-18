/*
 * S1Button.cpp
 *
 *  Created on: Oct 5, 2016
 *      Author: fabian
 */

#include "S1Button.hpp"


S1Button::S1Button() {
	// Configure S1 Button
	ConfigS1ButtonInterrupt();

	// Debug
	P2->DIR |= BIT1;
}

uint8_t S1Button::run(void) {
	// Destructor
	g_bGlobalFlags[Debounce_Flag] = true;

	// Do Something
	P2->OUT ^= BIT1;

	return NO_ERR;
}

