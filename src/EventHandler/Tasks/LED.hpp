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
        LED(uint16_t LEDMask);
        virtual uint8_t run(void);
        uint16_t mLEDMask;

        // Process Message
        void ProcessMessage(Task::Message);

        enum MessageTypes {
            GO_FASTER,
            NUM_TYPES
        };
    protected:
    private:
};

#endif /* LED_HPP_ */
