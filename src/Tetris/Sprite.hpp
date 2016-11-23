/*
 * Sprite.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
 */

#ifndef SPRITE_HPP_
#define SPRITE_HPP_

/* C++ Standard libs */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

/* MSPWare definitions */
#include "msp.h"

extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
	#include <stdio.h>
}
/* Local Definitions */
#include "Definitions.hpp"

#define NUM_BLOCKS 4

class Sprite {
public:
	// Static Graphic Context
	static Graphics_Context* m_GraphicsContext;

	// Point Coordinate Structure, represents the position of a segment,
	// by indicating the down-left corner
	struct Point {
	   uint8_t Horizontal; 	// FROM 0 to 99 (Discrete 10 levels)
	   uint8_t Vertical;	// FROM 0 to 119 (Continous, in jumps of velocity).
	};

	// ENUM For the types of blocks
	typedef enum {
		oBlock,
		iBlock,
		sBlock,
		tBlock,
		lBlock,
		// Total type count
		NUM_SPRITE_TYPES
	} eSpriteTypes;

	// ENUM with the sprite colors
	typedef enum {
		COLOR_GREEN = 	0x07E0,
		COLOR_BLUE = 	0x001F,
		COLOR_RED =		0xF800,
		COLOR_YELLOW = 	0xFFE0,
		COLOR_GRAY = 	0xC618
	} eSpriteColors;

	// ENUM with the sprite positions
	typedef enum {
		North,
		EAST,
		SOUTH,
		WEST
	} eSpriteOrientation;

	// Default constructor
	Sprite();
	// Typed constructor (selects which sprite to create
	Sprite(eSpriteTypes i_Type);
	// Destructor
	virtual ~Sprite();

	// Paint Sprite
	void Paint();
	// Delete Sprite
	void Delete();

	// Place Sprite
	void SetInitialPosition();

	// Move Sprite
	void MoveDown();
	void MoveRight();
	void MoveLeft();

	// Rotate Sprite
	void RotateLeft();
	void RotateRight();

private:
	// Type of Sprite
	eSpriteTypes m_Type;
	// Color of Sprite
	eSpriteColors m_Color;
	// Sprite current orientation
	eSpriteOrientation m_Orientation;
	// Member blocks, all figures are represented by 4 squares.
	Point m_Blocks[4];

	// Function to paint a segment
	void PaintSegment(uint8_t i_SegmentNum, uint16_t i_Color);
	// Define Color
	void SetColor();
	// Set Sprite Position
	void SetSpritePositionNorth(Point i_Center);
	void SetSpritePositionEast(Point i_Center);
	void SetSpritePositionSouth(Point i_Center);
	void SetSpritePositionWest(Point i_Center);


};

#endif /* SPRITE_HPP_ */
