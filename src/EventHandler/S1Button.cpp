/*
 * S1Button.cpp
 *
 *  Created on: Oct 5, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */

#include "S1Button.hpp"


S1Button::S1Button(Scheduler* scheduler, Task* receiver) {
	// Add attributes
	m_Scheduler = scheduler;
	m_Receiver = receiver;

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

	// Send message, of type 0 to the receiver
	m_Scheduler->AddMessage(this, m_Receiver, 0, 0);

	return NO_ERR;
}

