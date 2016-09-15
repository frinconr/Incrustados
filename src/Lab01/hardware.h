/*
 * hardware.h
 *
 *	Header file for a local hardware abstraction
 *	layer. General configurations for the MSP432
 *	are defined here.
 *
 *  Created on: Sep 10, 2016
 *      Authors: Fabián Meléndez / Felipe Rincón
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
void ConfigADC14Mic();

#endif /* HARDWARE_H_ */
