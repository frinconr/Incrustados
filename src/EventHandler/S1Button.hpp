/*
 * S1Button.hpp
 *
 *  Created on: Oct 5, 2016
 *      Author: fabian
 */

#ifndef S1BUTTON_HPP_
#define S1BUTTON_HPP_

#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Definitions.hpp"
#include "hardware.hpp"


class S1Button : public Task
{
    public:
		S1Button();
        virtual uint8_t run(void);
};

#endif /* S1BUTTON_HPP_ */
