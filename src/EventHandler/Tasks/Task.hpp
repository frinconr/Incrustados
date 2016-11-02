#ifndef TASK_HPP_
#define TASK_HPP_

#define __NOP __nop
#include "msp.h"
#include "Definitions.hpp"

#define NO_ERR 0
#define RET_ERR 1

class Task
{
	public:
		// Message between tasks structure
		struct Message {
			Task * Receiver;
			Task * Sender;
			uint8_t Type;
			int  	Data;
		};
		Task();
		uint8_t m_u8TaskID;
		uint16_t i_u16TickInterval;
		bool m_bActive;

		virtual uint8_t     run(void){return(0);};
		bool                IsTaskFinished(void){return m_bIsFinished;};
		uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
        virtual void        ProcessMessage(Task::Message) {/*DO NOTHING*/};
        void 				Revive(void);
        void				Kill(void);

	private:
	   static uint8_t m_u8NextTaskID;
	   uint8_t m_u8Priority;
	protected:
	   bool m_bIsFinished;
};

#endif /* TASK_HPP_ */
