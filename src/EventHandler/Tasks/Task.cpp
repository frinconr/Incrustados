#include "Task.hpp"

/*
 * Constructor of class
 * This basic method sets the TaskID and both
 * flags for the Task being ran.
 */
Task::Task()
{
    m_u8TaskID = m_u8NextTaskID;
    m_u8NextTaskID++;
    m_bIsFinished = false;
    m_bActive = true;
}

/*
 * Revive the Task. This sets the active flag to true and re-enables the Task
 * to be run by the Scheduler
 */
void Task::Revive(void){
	m_bActive = true;
}

/*
 * Kill the Task. This sets the active flat to false, and disables the Task
 * from being run by the Scheduler.
 */
void Task::Kill(void){
	m_bActive = false;
}


