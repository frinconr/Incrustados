/*
 * Area.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
 */

#ifndef ARENA_HPP_
#define ARENA_HPP_

class Arena {

public:
	Arena();
	virtual ~Arena();

private:
	// Game Matrix
	uint16_t m_u16GameMatrix[NUM_X_SQUARES][NUM_Y_SQUARES];
	// Score
	uint8_t m_u8Score;
};

#endif /* ARENA_HPP_ */
