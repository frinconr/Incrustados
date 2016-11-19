/*
 * Sprite.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
 */

#include <Sprite.hpp>

Sprite::Sprite() {
	// TODO Auto-generated constructor stub

}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}



uint8_t Sprite::GetLowestYCoordinate() {

	uint8_t minY = 128;

	for(int i=0; i<4; i++) {
		if(m_Blocks[i].Vertical < minY)
			minY = m_Blocks[i].Vertical;
	}

	return minY;
}
