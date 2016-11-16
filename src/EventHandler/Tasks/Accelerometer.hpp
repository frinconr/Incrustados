/*
 * Acelerometer.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: fabian
 */

#ifndef TASKS_ACELEROMETER_HPP_
#define TASKS_ACELEROMETER_HPP_

#define __NOP __nop
#include "Task.hpp"
#include "Scheduler.hpp"
#include "ScreenPainter.hpp"

class Accelerometer: public Task {
public:
	Accelerometer(Scheduler * , Task*);
	virtual ~Accelerometer();
	virtual uint8_t Run(void);


};

#endif /* TASKS_ACELEROMETER_HPP_ */
