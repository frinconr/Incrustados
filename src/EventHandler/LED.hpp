/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef LED_HPP_
#define LED_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Definitions.hpp"


class LED : public Task
{
    public:
        LED();
        virtual uint8_t run(void);
    protected:
    private:
};

#endif /* LED_HPP_ */
