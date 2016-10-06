#include "Scheduler.hpp"

Scheduler::Scheduler()
{
	// Initialize indexes for Schedule
    mOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    mNextSlot = 0;

    // Initialize indexes for nextSchedule
    mNextScheduleOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    mNextScheduleNextSlot = 0;

    // For the recurring events
    mRecEventsOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS-1U);
    mRecEventsNextSlot = 0;

    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS-1U); index++)
    {
    	// Init Schedule
        Schedule[index] = (uintptr_t) 0;
        NextSchedule[index] = (uintptr_t) 0;
        // Init recurring events
        RecurringEvents[index].t_Task = 0;
        RecurringEvents[index].i_u16TickInterval = 0;
    }
    return;
}

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

uint8_t Scheduler::attach(Task * i_ToAttach, uint16_t i_u16TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mRecEventsOpenSlots>0) && (mRecEventsNextSlot < NUMBER_OF_SLOTS))
    {
    	// Initialize Task pointer
        RecurringEvents[mRecEventsNextSlot].t_Task = i_ToAttach;
        // Initialize counters
        RecurringEvents[mRecEventsNextSlot].i_u16TickInterval = i_u16TickInterval;
        RecurringEvents[mRecEventsNextSlot].i_u16CounterToRun = i_u16TickInterval;
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

uint8_t Scheduler::run(void)
{
    uint8_t NextTaskSlot = 0U;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

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
    return l_u8ReturnCode;
}


uint8_t Scheduler::AddRecurringEvents(void) {
	for(uint8_t index = 0; index < mRecEventsNextSlot; index++)
	{
		if(RecurringEvents[index].i_u16CounterToRun == 0)
		{
			// Reset count to execution
			RecurringEvents[index].i_u16CounterToRun = RecurringEvents[index].i_u16TickInterval;
			// Add task to NextSchedule
			attach(RecurringEvents[index].t_Task);
		}
		else
		{
			RecurringEvents[index].i_u16CounterToRun--;
		}
	}
	return(NO_ERR);
}


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


uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
