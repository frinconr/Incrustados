/*
 * Scheduler.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: Fabian Melendez
 */

#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#define __NOP __nop
#include "msp.h"
#include "Definitions.hpp"
#include "Task.hpp"

#define NUMBER_OF_SLOTS 256
#define NULL            0

class Scheduler
{
public:
    Scheduler();
    uint64_t ticks;
    uint8_t attach(Task * i_ToAttach);
    uint8_t attach(Task * i_ToAttach, uint16_t i_u16TickInterval, bool OneShot=false);
    uint8_t run(void);
    uint8_t AddRecurringEvents();

    // For the messages
    uint8_t AddMessage(Task*, Task*, int Type, int* data);
    uint8_t ProcessMessages();

    // Structure to handle repeating events
    struct RepeatingTask {
		uint16_t i_u16TickInterval;
		uint16_t i_u16CounterToRun;
		bool b_OneShot;
		Task * t_Task;
	};

private:
    uint8_t mOpenSlots;
    uint8_t mNextSlot;
    Task * Schedule[NUMBER_OF_SLOTS];

    uint8_t mNextScheduleOpenSlots;
	uint8_t mNextScheduleNextSlot;
    Task * NextSchedule[NUMBER_OF_SLOTS];

    // For recurring tasks
    RepeatingTask RecurringEvents[NUMBER_OF_SLOTS];
    uint8_t mRecEventsOpenSlots;
    uint8_t mRecEventsNextSlot;

    uint8_t CalculateNextSchedule(void);
    uint8_t SortScheduleByPriority(Task * i_pSchedule);

    // For the message queue
    Task::Message MessageQueue[MAX_MSJS];
    uint8_t mMessageNextSlot;
    uint8_t mFirstMessageSlot;
};



#endif /* TASKS_SCHEDULER_HPP_ */