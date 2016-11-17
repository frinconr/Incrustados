/*
 * Scheduler.hpp
 *
 *	Header file for the Scheduler class. This class is a real time operating system
 *	scheduler, it works along with the Task class, and provides the logic to run
 *	tasks (as one shots, and recurring executions), and a messaging system to commu-
 *	nicate Tasks.
 *
 *  Created on: Aug 31, 2016
 *      Author: Fabian Melendez
 *      		Felipe Rincon
 */

#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#define __NOP __nop
#include "msp.h"
#include "Definitions.hpp"
#include "Task.hpp"

/* 	Local Definitions  */
#define NUMBER_OF_SLOTS 256
#define NULL            0

class Scheduler
{
public:
	// Constructor
    Scheduler();
    uint64_t m_u64Ticks;

    // Method to attach tasks
    uint8_t Attach(Task * i_tToAttach);
    // Method to attach periodic tasks
    uint8_t Attach(Task * i_tToAttach, uint16_t i_u16TickInterval, bool OneShot=false);
    // Method to execute tasks
    uint8_t Run(void);
    // For the messages
    uint8_t AddMessage(Task* i_tSender, Task* i_tReceiver, int i_iType, int i_iData);

    // Structure to handle repeating events
    struct RepeatingTask {
		uint16_t i_u16TickInterval;
		uint16_t i_u16CounterToRun;
		bool b_OneShot;
		Task * t_Task;
	};

private:
    // For the current Schedule
    uint8_t m_u8OpenSlots;
    uint8_t m_u8NextSlot;
    Task *  m_tSchedule[NUMBER_OF_SLOTS];

    // For the next schedule
    uint8_t m_u8NextScheduleOpenSlots;
	uint8_t m_u8NextScheduleNextSlot;
    Task *  m_tNextSchedule[NUMBER_OF_SLOTS];

    // For recurring tasks
    RepeatingTask m_tRecurringEvents[NUMBER_OF_SLOTS];
    uint8_t m_u8RecEventsOpenSlots;
    uint8_t m_u8RecEventsNextSlot;

    // For the message queue
    Task::Message m_tMessageQueue[MAX_MSJS];
    uint8_t m_tNextMessageSlot;
    uint8_t m_tFirstMessageSlot;

    // Methods for calculating the next schedule and sorting by priority
    uint8_t CalculateNextSchedule(void);
    // Internal sorting of events
    uint8_t SortScheduleByPriority(Task * i_pSchedule);
    // To process the message queue
    uint8_t ProcessMessages();
    // Method to process recurring events
    uint8_t AddRecurringEvents();
};



#endif /* TASKS_SCHEDULER_HPP_ */
