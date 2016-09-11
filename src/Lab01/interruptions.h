/*
 * interruptions.h
 *
 *  Created on: Sep 10, 2016
 *      Author: fabian
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

#endif /* INTERRUPTIONS_H_ */
