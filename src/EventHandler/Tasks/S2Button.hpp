/*
 * S2Button.hpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Felipe
 */

#ifndef TASKS_S2BUTTON_HPP_
#define TASKS_S2BUTTON_HPP_

#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Definitions.hpp"
#include "hardware.hpp"


class S2Button : public Task
{
    public:
		S2Button();
        virtual uint8_t run(void);
};



#endif /* TASKS_S2BUTTON_HPP_ */
