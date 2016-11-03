/*
 * ScreenPainter.hpp
 *
 *  Created on: Nov 1, 2016
 *      Authors: Fabian Meléndez
 *      		 Felipe Rincón
 */

/* Local Defintions */


#ifndef TASKS_SCREENPAINTER_HPP_
#define TASKS_SCREENPAINTER_HPP_
#define __NOP __nop

#include "Definitions.hpp"
#include "msp.h"
#include "Task.hpp"
#include "hardware.hpp"
#include "Scheduler.hpp"


class ScreenPainter : public Task
{

public:
	ScreenPainter(Graphics_Context*);
	virtual ~ScreenPainter();
	virtual uint8_t run(void);
	void ProcessMessage(Task::Message);

    enum MessageTypes {
		UPDATE_NEEDED,
		NUM_TYPES
	};

private:
    uint16_t a_LastValue;
    uint16_t a_CurrentValue;
    Graphics_Context* a_GraphicsContext;
    Graphics_Rectangle a_PaintArea;

};

#endif /* TASKS_SCREENPAINTER_HPP_ */
