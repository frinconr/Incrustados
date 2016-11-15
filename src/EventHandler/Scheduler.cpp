#include "Scheduler.hpp"

//////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////

Scheduler::Scheduler() {
	// Initialize indexes for Schedule
    mOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    mNextSlot = 0;

    // Initialize indexes for nextSchedule
    mNextScheduleOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    mNextScheduleNextSlot = 0;

    // For the recurring events
    mRecEventsOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    mRecEventsNextSlot = 0;

    // For the Message slots, initialize indexes
    mNextMessageSlot = 0;
    mFirstMessageSlot = 0;

    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS-1U); index++)
    {
    	// Initialize Schedule
        Schedule[index] = (uintptr_t) 0;
        NextSchedule[index] = (uintptr_t) 0;
        // Initialize recurring events
        RecurringEvents[index].t_Task = 0;
        RecurringEvents[index].t_Task->i_u16TickInterval = 0;
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
uint8_t Scheduler::attach(Task * i_ToAttach)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mNextScheduleOpenSlots>0) && (mNextScheduleNextSlot < NUMBER_OF_SLOTS))
    {
        NextSchedule[mNextScheduleNextSlot] = i_ToAttach;
        mNextScheduleOpenSlots--;
        mNextScheduleNextSlot++;
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
uint8_t Scheduler::attach(Task * i_ToAttach, uint16_t i_u16TickInterval, bool b_OneShot)
{
    uint8_t l_ErrorCode = NO_ERR;
    // Check if we have open slots
    if((mRecEventsOpenSlots>0) && (mRecEventsNextSlot < NUMBER_OF_SLOTS))
    {
    	// Initialize Task pointer
        RecurringEvents[mRecEventsNextSlot].t_Task = i_ToAttach;
        // Initialize counters
        RecurringEvents[mRecEventsNextSlot].t_Task->i_u16TickInterval = i_u16TickInterval-1;
        RecurringEvents[mRecEventsNextSlot].i_u16CounterToRun = i_u16TickInterval-1;
        RecurringEvents[mRecEventsNextSlot].b_OneShot = b_OneShot;
        // Update indexes
        mRecEventsOpenSlots--;
        mRecEventsNextSlot++;
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
uint8_t Scheduler::run(void)
{
    uint8_t NextTaskSlot = 0U;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    // Calculate the Next Schedule
    CalculateNextSchedule();

    while(NextTaskSlot < (NUMBER_OF_SLOTS-1U))
    {
        NextTask = static_cast<Task *> (Schedule[NextTaskSlot]);
        if(NextTask != ((uintptr_t) 0))
        {
            NextTask->run();
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
	for(uint8_t index = 0; index < mRecEventsNextSlot; index++)
	{
		// Check that Task is active
		if(RecurringEvents[index].t_Task->m_bActive)
		{
			if(RecurringEvents[index].i_u16CounterToRun == 0)
			{
				// Reset count to execution
				RecurringEvents[index].i_u16CounterToRun = RecurringEvents[index].t_Task->i_u16TickInterval;

				// Add task to NextSchedule
				attach(RecurringEvents[index].t_Task);

				if(RecurringEvents[index].b_OneShot) {
					// Disable the one shot interrupt
					RecurringEvents[index].t_Task->m_bActive = false;
				}
			}
			else
			{
				// Reduce count-down
				RecurringEvents[index].i_u16CounterToRun--;
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
		Schedule[index] = NextSchedule[index];
		NextSchedule[index] = (uintptr_t) 0;
	}

	// Update indexes
	mOpenSlots = mNextScheduleOpenSlots;
	mNextSlot = mNextScheduleNextSlot;

	mNextScheduleOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
	mNextScheduleNextSlot= 0;

    return(NO_ERR);
}

/*!
 * TODO!
 *
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
uint8_t Scheduler::AddMessage(Task* sender, Task* receiver, int Type, int data=0) {
	// If we still have room for messages:
    if(mFirstMessageSlot != mNextMessageSlot+1) {
    	// Fill message information
        MessageQueue[mNextMessageSlot].Sender = sender;
        MessageQueue[mNextMessageSlot].Receiver = receiver;
        MessageQueue[mNextMessageSlot].Type = Type;
        MessageQueue[mNextMessageSlot].Data = data;

        // Update index of next message
        mNextMessageSlot = (mNextMessageSlot+1)% MAX_MSJS;
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
	while(mFirstMessageSlot != mNextMessageSlot) {
		MessageQueue[mFirstMessageSlot].Receiver->ProcessMessage(MessageQueue[mFirstMessageSlot]);
		mFirstMessageSlot = (mFirstMessageSlot+1)%MAX_MSJS;
	}
    return(NO_ERR);
}
