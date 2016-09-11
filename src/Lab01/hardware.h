/*
 * hardware.h
 *
 *  Created on: Sep 10, 2016
 *      Author: fabian
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_


// MSP library
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* DriverLib Includes */
#include "driverlib.h"

#include "include/HAL_OPT3001.h"
#include "include/HAL_I2C.h"

void EnableInterruptions();
void DisableInterruptions();
void ConfigP2LEDOutput(const uint16_t LEDMask);
void ConfigP1ButtonInterrupt();
void ConfigTimerA0UpMode(const uint16_t LEDMask);
void ConfigLUXI2C();

#endif /* HARDWARE_H_ */
