/*
 * hardware.hpp
 *
 *  Created on: Oct 5, 2016
 *      Author: fabian
 */

#ifndef HARDWARE_HPP_
#define HARDWARE_HPP_

#include "msp.h"
#include "Definitions.hpp"

void EnableInterruptions();
void DisableInterruptions();
void DisableWDT();
void ConfigP1LED();
void ConfigP2LED(const uint16_t LEDMask);
void ConfigS2ButtonInterrupt();
void ConfigS1ButtonInterrupt();
void ConfigP2PWM();
void ConfigTimer32(uint16_t MaxCount);
void ConfigTimerA();


#endif /* HARDWARE_HPP_ */