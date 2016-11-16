/*
 * ScreenPainter.hpp
 *
 * 		This file contains the header for the ScreenPainter Task. Its in
 * 		charge of painting the LCD screen, with the value received from the
 * 		accelerometer.
 *
 *  Created on: Nov 1, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */


/* Local Defintions */
#ifndef TASKS_SCREENPAINTER_HPP_
#define TASKS_SCREENPAINTER_HPP_
#define __NOP __nop

/* Local Includes  */
#include "Definitions.hpp"
#include "msp.h"
#include "Task.hpp"
#include "hardware.hpp"
#include "Scheduler.hpp"

/// Class inherits from task
class ScreenPainter : public Task {

public:
	ScreenPainter(Graphics_Context*,Scheduler* scheduler, Task* receiver);
	virtual ~ScreenPainter();
	virtual uint8_t Run(void);
	void ProcessMessage(Task::Message);

    enum MessageTypes {
		UPDATE_NEEDED,
		NUM_TYPES
	};

    void SetValue(uint16_t Value, uint16_t Orientation);

    Scheduler * m_Scheduler;
    Task * m_Receiver;

private:
    // We store current and las value of height painted in the screen
    uint16_t a_LastValue;
    uint16_t a_CurrentValue;

    // For receiving the value from the accelerometer
    uint16_t m_Measure;

    // For painting the screen:
    Graphics_Context* a_GraphicsContext;
    Graphics_Rectangle a_PaintArea;

    // Booleans to save position and when the value has changed
    bool a_PositionUp;
    bool ValueChanged;

};

#endif /* TASKS_SCREENPAINTER_HPP_ */
