/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: Fabian Melendez
 *      		Felipe Rincon
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

/* C++ Standard libs */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

/* MSPWare definitions */
#include "msp.h"

/* DriverLib Include */
extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
	#include <stdio.h>
}

/* Local Includes */
#include "Definitions.hpp"
#include "hardware.hpp"
#include "Sprite.hpp"
#include "Music.hpp"

void Setup(void);

#endif /* MAIN_HPP_ */
