#include "Task.hpp"

Task::Task()
{
    m_u8TaskID = m_u8NextTaskID;
    m_u8NextTaskID++;
    m_bIsFinished = false;
    m_bActive = true;
}


void Task::Revive(void){
	m_bActive = true;
}

void Task::Kill(void){
	m_bActive = false;
}


