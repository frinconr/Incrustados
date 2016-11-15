/*
 * Scheduler.cpp
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

#include "Scheduler.hpp"

//////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////

/*
 * Constructor
 *
 * It initializes the indexes and clears the schedulers.
 */
Scheduler::Scheduler() {
	// Initialize indexes for Schedule
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    m_u8NextSlot = 0;

    // Initialize indexes for nextSchedule
    m_u8NextScheduleOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    m_u8NextScheduleNextSlot = 0;

    // For the recurring events
    m_u8RecEventsOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    m_u8RecEventsNextSlot = 0;

    // For the Message slots, initialize indexes
    m_tNextMessageSlot = 0;
    m_tFirstMessageSlot = 0;

    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS-1U); index++)
    {
    	// Initialize Schedule
        m_tSchedule[index] = (uintptr_t) 0;
        m_tNextSchedule[index] = (uintptr_t) 0;
        // Initialize recurring events
        m_tRecurringEvents[index].t_Task = 0;
        m_tRecurringEvents[index].t_Task->i_u16TickInterval = 0;
    }
    return;
}


//////////////////////////////////////////////////////////////////////////////
// Attaching methods
//////////////////////////////////////////////////////////////////////////////


/*! Single shot attach. This adds the task pointer to the next Scheduler to run
 * in the next cycle.
 *
 * \param: i_ToAttach: A pointer to a Task (inherited class from Task class).
 * \return: Error code NO_ERR or RET_ERR.
 !*/
uint8_t Scheduler::Attach(Task * i_ToAttach)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((m_u8NextScheduleOpenSlots>0) && (m_u8NextScheduleNextSlot < NUMBER_OF_SLOTS))
    {
        m_tNextSchedule[m_u8NextScheduleNextSlot] = i_ToAttach;
        m_u8NextScheduleOpenSlots--;
        m_u8NextScheduleNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}


/*!
 * Recursive or delayed attach.
 *
 * This function attaches a function that either runs in a number of cycles and stops,
 * or continuous to run recursively with the same latency.
 *
 * \param i_ToAttach: A pointer to a Task (inherited class from Task class).
 * \param u_u16TickInverval: Latency or delay of task to run.
 * \param b_OneShot: A boolen to see if task should run recursively or just one time
 */
uint8_t Scheduler::Attach(Task * i_ToAttach, uint16_t i_u16TickInterval, bool b_OneShot)
{
    uint8_t l_ErrorCode = NO_ERR;
    // Check if we have open slots
    if((m_u8RecEventsOpenSlots>0) && (m_u8RecEventsNextSlot < NUMBER_OF_SLOTS))
    {
    	// Initialize Task pointer
        m_tRecurringEvents[m_u8RecEventsNextSlot].t_Task = i_ToAttach;
        // Initialize counters
        m_tRecurringEvents[m_u8RecEventsNextSlot].t_Task->i_u16TickInterval = i_u16TickInterval-1;
        m_tRecurringEvents[m_u8RecEventsNextSlot].i_u16CounterToRun = i_u16TickInterval-1;
        m_tRecurringEvents[m_u8RecEventsNextSlot].b_OneShot = b_OneShot;
        // Update indexes
        m_u8RecEventsOpenSlots--;
        m_u8RecEventsNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}

//////////////////////////////////////////////////////////////////////////////
// Processing methods
//////////////////////////////////////////////////////////////////////////////

/*! RUN
 *
 * This is the function that executes the Tasks.
 *
 * First, it calculates the Task Schedule to run, passing Tasks from the NextScheule
 * to the Schedule, and cleaning the NextSchedule.
 *
 * Then, it takes each task on the Schedule and executes the run method.
 */
uint8_t Scheduler::Run(void)
{
    uint8_t NextTaskSlot = 0U;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    // Calculate the Next Schedule
    CalculateNextSchedule();

    while(NextTaskSlot < (NUMBER_OF_SLOTS-1U))
    {
        NextTask = static_cast<Task *> (m_tSchedule[NextTaskSlot]);
        if(NextTask != ((uintptr_t) 0))
        {
            NextTask->Run();
            NextTaskSlot++;
        }
        else
        {
            NextTaskSlot++;
        }
    }

    // Process message Queue
    ProcessMessages();

    return l_u8ReturnCode;
}

/*!
 * AddRecurringEvents
 *
 * This function passes the recursive Tasks to run each time the have to,
 * For this the
 */
uint8_t Scheduler::AddRecurringEvents(void) {
	for(uint8_t index = 0; index < m_u8RecEventsNextSlot; index++)
	{
		// Check that Task is active
		if(m_tRecurringEvents[index].t_Task->m_bActive)
		{
			if(m_tRecurringEvents[index].i_u16CounterToRun == 0)
			{
				// Reset count to execution
				m_tRecurringEvents[index].i_u16CounterToRun = m_tRecurringEvents[index].t_Task->i_u16TickInterval;

				// Add task to NextSchedule
				Attach(m_tRecurringEvents[index].t_Task);

				if(m_tRecurringEvents[index].b_OneShot) {
					// Disable the one shot interrupt
					m_tRecurringEvents[index].t_Task->m_bActive = false;
				}
			}
			else
			{
				// Reduce count-down
				m_tRecurringEvents[index].i_u16CounterToRun--;
			}
		}
	}
	return(NO_ERR);
}

/*! CalculateNextSchedule
 *
 * This function passes the functions from the NextSchedule to the Schedule,
 * then it cleans the NextSchedule.
 */
uint8_t Scheduler::CalculateNextSchedule(void)
{
	// Add recurring events to next schedule
	AddRecurringEvents();

	// Overwrite Schedule, and clean Next Schedule
	for(uint8_t index = 0; index < (NUMBER_OF_SLOTS-1U); index++)
	{
		m_tSchedule[index] = m_tNextSchedule[index];
		m_tNextSchedule[index] = (uintptr_t) 0;
	}

	// Update indexes
	m_u8OpenSlots = m_u8NextScheduleOpenSlots;
	m_u8NextSlot = m_u8NextScheduleNextSlot;

	m_u8NextScheduleOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
	m_u8NextScheduleNextSlot= 0;

    return(NO_ERR);
}

/*!
 * This function takes the Schedule array and sorts the Task pointers
 * depending on their priority.
 */
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}

////////////////////////////////////////////////////////////////////////////////
// Messages Methods
////////////////////////////////////////////////////////////////////////////////

/*!
 *  This function ads a message to the Message Queue.
 */
uint8_t Scheduler::AddMessage(Task* i_tSender, Task* i_tReceiver, int i_iType, int i_iData=0) {
	// If we still have room for messages:
    if(m_tFirstMessageSlot != m_tNextMessageSlot+1) {
    	// Fill message information
        m_tMessageQueue[m_tNextMessageSlot].Sender = i_tSender;
        m_tMessageQueue[m_tNextMessageSlot].Receiver = i_tReceiver;
        m_tMessageQueue[m_tNextMessageSlot].Type = i_iType;
        m_tMessageQueue[m_tNextMessageSlot].Data = i_iData;

        // Update index of next message
        m_tNextMessageSlot = (m_tNextMessageSlot+1)% MAX_MSJS;
    } else {
    	return(RET_ERR);
    }

    return(NO_ERR);
}

/*! ProcessMessages
 *
 *	Loop through all messages and run the messages on each Task.
 */
uint8_t Scheduler::ProcessMessages(){
	// If the pointers are different, we still have messages
	while(m_tFirstMessageSlot != m_tNextMessageSlot) {
		m_tMessageQueue[m_tFirstMessageSlot].Receiver->ProcessMessage(m_tMessageQueue[m_tFirstMessageSlot]);
		m_tFirstMessageSlot = (m_tFirstMessageSlot+1)%MAX_MSJS;
	}
    return(NO_ERR);
}
