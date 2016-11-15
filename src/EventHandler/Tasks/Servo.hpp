/*
 * Servo.hpp
 *
 *  Created on: Nov 14, 2016
 *      Author: Felipe
 */

#ifndef TASKS_SERVO_HPP_
#define TASKS_SERVO_HPP_

#define __NOP __nop

#include "Definitions.hpp"
#include "msp.h"
#include "Task.hpp"
#include "hardware.hpp"
#include "Scheduler.hpp"

class Servo : public Task
{
public:
	Servo();
	virtual ~Servo();
	virtual uint8_t Run(void);
	void ProcessMessage(Task::Message);

	enum MessageTypes {
		UPDATE_NEEDED,
		NUM_TYPES
	};

private:
	uint16_t a_CurrentValue;

};

#endif /* TASKS_SERVO_HPP_ */
