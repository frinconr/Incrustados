/*
 * Sprite.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
 */

#ifndef SPRITE_HPP_
#define SPRITE_HPP_

class Sprite {
public:
	// Point Coordinate Structure
	struct Point {
	   uint8_t Horizontal;
	   uint8_t Vertical;
	};

	// Default constructor
	Sprite();
	// Typed constructor (selects which sprite to create
	Sprite(uint8_t i_u8Type);
	// Destructor
	virtual ~Sprite();

	void Paint();
	void Delete();
	void Move();
	void RotateLeft();
	void RotateRight();
	uint8_t GetLowestYCoordinate();

	static Arena

private:
	// Member blocks, all figures are represented by 4 squares.
	Point m_Blocks[4];
	// Type of Sprite
	uint8_t m_Type;
};

#endif /* SPRITE_HPP_ */
