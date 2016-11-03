/*
 * Acelerometer.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: fabian
 */

#ifndef TASKS_ACELEROMETER_HPP_
#define TASKS_ACELEROMETER_HPP_

#include "Task.hpp"
#include "Scheduler.hpp"
#include "ScreenPainter.hpp"

class Accelerometer: public Task {
public:
	Accelerometer(Scheduler * , Task*);
	virtual ~Accelerometer();
	virtual uint8_t run(void);


};

#endif /* TASKS_ACELEROMETER_HPP_ */
