/*
 * S1Button.hpp
 *
 *  Created on: Oct 5, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */

#ifndef S1BUTTON_HPP_
#define S1BUTTON_HPP_

#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Definitions.hpp"
#include "hardware.hpp"
#include "Scheduler.hpp"


class S1Button : public Task
{
    public:
		S1Button(Scheduler * scheduler, Task * Receiver);
        virtual uint8_t run(void);

        // Attributes
        Scheduler * m_Scheduler;
        Task * m_Receiver;
};

#endif /* S1BUTTON_HPP_ */
