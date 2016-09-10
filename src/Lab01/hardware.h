/*
 * hardware.h
 *
 *  Created on: Sep 10, 2016
 *      Author: fabian
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

// Lab01 definitions
#include "lab01.h"

// MSP library
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void EnableInterruptions();
void DisableInterruptions();
void ConfigP2LEDOutput();
void ConfigP1ButtonInterrupt();
void ConfigTimerA0UpMode();


#endif /* HARDWARE_H_ */
