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
		zBlock,
		tBlock,
		lBlock,
		jBlock,
		// Total type count
		NUM_SPRITE_TYPES
	} eSpriteTypes;

	/* Documentation for Sprite Types and Orientations
	 *
	 *  **********************************************************
	 *  oBlock:
	 *  **********************************************************
	 *
 	 *  	+ - +  + - +
	 *  	| 0 |  | 1 |
	 *  	+ - +  * - +
	 *  	+ - +  + - +
	 *  	| 2 |  | 3 |
	 *  	+ - +  + - +
	 *
	 *  **********************************************************
	 *  iBlock:
	 *  **********************************************************
	 *
	 *      + - +  + - +  + - +  + - +   (EAST/WEST)
	 *  	| 0 |  | 1 |  | 2 |  | 3 |
	 *  	+ - +  + - +  * - +  + - +
	 *
	 *
	 *		+ - +	(NORTH/SOUTH)
	 *		| 0 |
	 *		+ - +
	 *		+ - +
	 *		| 1 |
	 *		* - +
	 *		+ - +
	 *		| 2 |
	 *		+ - +
	 *		+ - +
	 *		| 3 |
	 *		+ - +
	 *
	 *  **********************************************************
	 *	sBlock:
	 *	**********************************************************
	 *
	 *  	       + - +  + - +		(EAST/WEST)
	 *  	       | 0 |  | 1 |
	 *  	       + - +  * - +
	 *  	+ - +  + - +
	 *      | 2 |  | 3 |
	 *      + - +  + - +
	 *
	 *
	 *             + - +			(NORTH/SOUTH)
	 *  	       | 0 |
	 *  	       + - +
	 *  	       + - +  + - +
	 *             | 1 |  | 2 |
	 *             + - +  * - +
	 *                    + - +
	 *                    | 3 |
	 *                    + - +
	 *
	 *  **********************************************************
	 *  zBlock:
	 *  **********************************************************
	 *
	 *  	+ - +  + - +			(EAST/WEST)
	 *  	| 0 |  | 1 |
	 *  	+ - +  * - +
	 *  	       + - +  + - +
	 *  	       | 2 |  | 3 |
	 *  	       + - +  + - +
	 *
	 *  	      		  + - +		(NORTH/SOUTH)
	 *  	      		  | 0 |
	 *                    + - +
	 *  	       + - +  + - +
	 *             | 1 |  | 2 |
	 *             + - +  * - +
	 *             + - +
	 *             | 3 |
	 *             + - +
	 *
	 *  **********************************************************
	 *  tBlock:
	 *  **********************************************************
	 *
	 *             + - +		(EAST)
	 *             | 0 |
	 *             * - +
	 *      + - +  + - + + - +
	 *	    | 1 |  | 2 | | 3 |
	 *      + - +  + - + + - +
	 *
	 *
	 *      + - +  + - + + - +	(WEST)
	 *	    | 0 |  | 1 | | 2 |
	 *      + - +  * - + + - +
	 *             + - +
	 *             | 3 |
	 *             + - +
	 *
	 *             + - +		(NORTH)
	 *             | 0 |
	 *             + - +
	 *      + - +  + - +
	 *	    | 1 |  | 2 |
	 *      + - +  * - +
	 *             + - +
	 *             | 3 |
	 *             + - +
	 *
	 *             + - +
	 *             | 0 |		(SOUTH)
	 *             + - +
	 *             + - +  + - +
	 *	           | 1 |  | 2 |
	 *             * - +  + - +
	 *             + - +
	 *             | 3 |
	 *             + - +
	 *
	 * 	**********************************************************
	 *  lBlock:
	 *  **********************************************************
	 *
	 *      + - + + - + + - + 	(EAST)
	 *	    | 0 | | 1 | | 2 |
	 *      + - + * - + + - +
	 *      + - +
	 *      | 3 |
	 *      + - +
	 *   				+ - +	(WEST)
	 *                  | 0 |
	 *                  + - +
	 *      + - + + - + + - +
	 *	    | 1 | | 2 | | 3 |
	 *      + - + * - + + - +
	 *
	 *
	 *      + - +				(NORTH)
	 *	    | 0 |
	 *      + - +
	 *      + - +
	 *      | 1 |
	 *      * - +
	 *      + - + + - +
	 *      | 2 | | 3 |
	 *      + - + + - +
	 *
	 *      + - + + - +			(SOUTH)
	 *	    | 0 | | 1 |
	 *      + - + + - +
	 *            + - +
	 *            | 2 |
	 *            * - +
	 *            + - +
	 *            | 3 |
	 *            + - +
	 *
	 *	**********************************************************
	 *  jBlock:
	 *  **********************************************************
	 *
	 *      + - + + - + + - +  	(EAST)
	 *	    | 0 | | 1 | | 2 |
	 *      + - + * - + + - +
	 *                  + - +
	 *                  | 3 |
	 *                  + - +
	 *
	 *		+ - +				(WEST)
	 *      | 0 |
	 *      + - +
	 *      + - + + - + + - +
	 *	    | 1 | | 2 | | 3 |
	 *      + - + * - + + - +
	 *
	 *
	 *      + - + + - +			(NORTH)
	 *      | 0 | | 1 |
	 *      + - + + - +
	 *      + - +
	 *	    | 2 |
	 *      * - +
	 *      + - +
	 *      | 3 |
	 *      + - +
	 *
	 *            + - +			(SOUTH)
	 *            | 0 |
	 *            + - +
	 *            + - +
	 *	          | 1 |
	 *            * - +
	 *      + - + + - +
	 *      | 2 | | 3 |
	 *      + - + + - +
	 */


	// ENUM with the sprite colors
	typedef enum {
		COLOR_GREEN = 	0x07E0,
		COLOR_BLUE = 	0x001F,
		COLOR_RED =		0xF800,
		COLOR_YELLOW = 	0xFFE0,
		COLOR_PURPLE = 	0x780F,
		COLOR_CYAN = 	0x07FF,
		COLOR_ORANGE =  0xFD20,
		COLOR_WHITE = 	0xFFFF
	} eSpriteColors;

	// ENUM with the sprite positions
	typedef enum {
		NORTH,
		EAST,
		SOUTH,
		WEST
	} eSpriteOrientation;

	// Default constructor
	Sprite();
	// Typed constructor (selects which sprite to create
	Sprite(eSpriteTypes i_Type);

	// Paint Sprite
	void Paint();
	// Delete Sprite
	void Delete();

	// Move Sprite
	void MoveDown();
	void MoveRight();
	void MoveLeft();

	// Rotate Sprite
	/*
	void RotateLeft();*/
	void RotateClockwise();
	void RotateCounterClockwise();

	// Member blocks, all figures are represented by 4 squares.
	Point m_Blocks[4];
	// Type of Sprite
	eSpriteTypes m_Type;
	// Color of Sprite
	eSpriteColors m_Color;
	// Sprite current orientation
	eSpriteOrientation m_Orientation;
private:


	// Painting area
	Graphics_Rectangle m_PaintArea;

	// Place Sprite
	void SetInitialPosition();
	// Function to paint a segment
	void PaintSegment(uint8_t i_SegmentNum, uint16_t i_Color);
	// Define Color
	void SetColor();

	// Set Sprite Orientation
	void SetOrientationEast(Point i_Center);
	void SetOrientationWest(Point i_Center);
	void SetOrientationNorth(Point i_Center);
	void SetOrientationSouth(Point i_Center);
};

#endif /* SPRITE_HPP_ */
