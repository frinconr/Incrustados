/*
 * interruptions.h
 *
 *	Header file for the service interruptions res-
 *	ponse functions.
 *
 *  Created on: Sep 10, 2016
 *      Authors: Fabián Meléndez / Felipe Rincón
 */


#ifndef INTERRUPTIONS_H_
#define INTERRUPTIONS_H_

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


// Local includes
#include "msp.h"
#include "lab01.h"

void TA0_0_ISR(void);
void S1_PORT1_ISR(void);
void ADC14_IRQHandler(void);

#endif /* INTERRUPTIONS_H_ */
