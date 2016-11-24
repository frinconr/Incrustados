/*
 * Sprite.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */

#include <Sprite.hpp>

/* Sprite Constructor
 *
 * It creates a Sprite instance, generating a random type. It sets the
 * color of the sprite and makes the initial placement.
 *
 */
Sprite::Sprite() {
	// Generate random sprite type
	this->m_Type = static_cast<eSpriteTypes> (rand() % NUM_SPRITE_TYPES);

	// Set to color of sprite
	this->SetColor();

	// Set the initial position
	this->SetInitialPosition();

	// Paint current Sprite
	this->Paint();
}

/* Sprite Constructor
 *
 * It creates a Sprite instance, taking the type given as a parameter.
 * It sets the color of the sprite and makes the initial placement.
 *
 */
Sprite::Sprite(eSpriteTypes i_Type){
	// Set the type
	this->m_Type = i_Type;

	// Set to color of sprite
	this->SetColor();

	// Set the initial position
	this->SetInitialPosition();

	// Paint current Sprite
	this->Paint();
}

/* SetInitialPosition
 *
 * This method places the Sprite in a hardcoded initial position, it places the blocks and
 * creates the figure that should be represented
*/
void Sprite::SetInitialPosition() {
	// Set orientation as East, at half the screen, as high as posible
	Point l_InitialPosition;
	l_InitialPosition.Horizontal = (ARENA_WIDTH/2);
	l_InitialPosition.Vertical = SEGMENT_HEIGHT;

	// Set position as east
	this->SetOrientationEast(l_InitialPosition);
}

/* Define Color.
 *
 * Takes the value of m_Type and maps to the SpriteColors
 * enum depending on the type of the sprite.
 *
 */
void Sprite::SetColor() {
	switch(this->m_Type) {
		case oBlock:
			this->m_Color = COLOR_GREEN;
			break;
		case iBlock:
			this->m_Color = COLOR_BLUE;
			break;
		case sBlock:
			this->m_Color = COLOR_RED;
			break;
		case zBlock:
			this->m_Color = COLOR_YELLOW;
			break;
		case tBlock:
			this->m_Color = COLOR_PURPLE;
			break;
		case lBlock:
			this->m_Color = COLOR_CYAN;
			break;
		case jBlock:
			this->m_Color = COLOR_ORANGE;
			break;
		default:
			this->m_Color = COLOR_WHITE;
			break;
	}
}


/* Sprite Paint
 *
 * Paints the four blocks in the corresponding color of the
 * sprite.
 *
 */
void Sprite::Paint() {
	// Iterate through all segments and paint as fill color
	for(uint8_t i=0; i<NUM_BLOCKS; i++){
		// Call the paint segment method with the background color
		this->PaintSegment(i,this->m_Color);
	}
}

/* Sprite Delete
 *
 * This method erases from the screen the current position
 * of the sprite.
 *
 */
void Sprite::Delete() {
	// Iterate through all segments and paint as background color
	for(int i=0; i<NUM_BLOCKS; i++){
		// Call the paint segment method with the bacground color
		this->PaintSegment(i,BACKGROUND_COLOR);
	}
}

/* PaintSegment
 *
 * This function paints (also deletes, depending on the color given as as a
 * parameter.
 *
 * @param i_SegmentNum: Number of segment to paint.
 * @param i_Color: Color to paint
 *
 */
void Sprite::PaintSegment(uint8_t i_SegmentNum, uint16_t i_Color){

	// Define square to paint:
	m_PaintArea.xMin = MIN_ARENA_X + m_Blocks[i_SegmentNum].Horizontal;
	m_PaintArea.xMax = MIN_ARENA_X + m_Blocks[i_SegmentNum].Horizontal + (SEGMENT_WIDTH-1);
	m_PaintArea.yMin = MIN_ARENA_Y + m_Blocks[i_SegmentNum].Vertical - (SEGMENT_HEIGHT-1);
	m_PaintArea.yMax = MIN_ARENA_Y + m_Blocks[i_SegmentNum].Vertical;

	// Paint the rectangle
	Graphics_fillRectangleOnDisplay(Sprite::m_GraphicsContext->display, &m_PaintArea, i_Color);
}

/* MoveDown
 *
 * This function moves the position of the segment block, down
 * on a VERTICAL_JUMP number of pixels.
 *
 * It verifies that the movement doesn't overflow the screen.
 *
 */
void Sprite::MoveDown(){
	// Boolean to see if we can move left
	bool l_bCanBeMoved = true;

	// Loop through segments to see if we can move
	for(int i=0; i<NUM_BLOCKS; i++){
		if(m_Blocks[i].Vertical + VERTICAL_JUMP > 127) {
			l_bCanBeMoved = false;
			break;
		}
	}

	if(l_bCanBeMoved){
		// Iterate through all segments and change position
		for(int i=0; i<NUM_BLOCKS; i++){
			m_Blocks[i].Horizontal = m_Blocks[i].Horizontal + VERTICAL_JUMP;
		}
	}
}

/* MoveRight
 *
 * This function moves the position of the segment block, right
 * on a SEGMENT_WIDTH number of pixels.
 *
 * It verifies that the movement doesn't overflow the screen.
 *
 */
void Sprite::MoveRight(){
	// Boolean to see if we can move left
	bool l_bCanBeMoved = true;

	// Loop through segments to see if we can move
	for(int i=0; i<NUM_BLOCKS; i++){
		if(m_Blocks[i].Horizontal + SEGMENT_WIDTH > 127) {
			l_bCanBeMoved = false;
			break;
		}
	}

	if(l_bCanBeMoved){
		// Iterate through all segments and change position
		for(int i=0; i<NUM_BLOCKS; i++){
			m_Blocks[i].Horizontal = m_Blocks[i].Horizontal + SEGMENT_WIDTH;
		}
	}

}

/* MoveLeft
 *
 * This function moves the position of the segment block, left
 * on a SEGMENT_WIDTH number of pixels.
 *
 * It verifies that the movement doesn't underflow the screen.
 *
 */
void Sprite::MoveLeft(){
	// Boolean to see if we can move left
	bool l_bCanBeMoved = true;

	// Loop through segments to see if we can move
	for(int i=0; i<NUM_BLOCKS; i++){
		if(m_Blocks[i].Horizontal - SEGMENT_WIDTH < 0) {
			l_bCanBeMoved = false;
			break;
		}
	}

	if(l_bCanBeMoved) {
		// Iterate through all segments and change position
		for(int i=0; i<NUM_BLOCKS; i++){
			m_Blocks[i].Horizontal = m_Blocks[i].Horizontal - SEGMENT_WIDTH;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// ROTATE Clockwise
///////////////////////////////////////////////////////////////////////////////////////
void Sprite::RotateClockwise() {
	// Center point for next orientation
	Point l_NextOrientationCenter;

	// boolean to see if it can rotate
	bool l_CanRotateRight = true;

	switch(this->m_Type) {
		case oBlock:
			/////////////////////////////
			// oBlock
			/////////////////////////////
			l_NextOrientationCenter = this->m_Blocks[1];
			break;

		case iBlock:
			/////////////////////////////
			// iBlock
			/////////////////////////////
			if(this->m_Orientation == EAST || this->m_Orientation == WEST){
				l_NextOrientationCenter = this->m_Blocks[2];
			} else {
				l_NextOrientationCenter = this->m_Blocks[1];
			}
			break;
		case sBlock:
			/////////////////////////////
			// sBlock
			/////////////////////////////
			if(this->m_Orientation == EAST || this->m_Orientation == WEST){
				l_NextOrientationCenter = this->m_Blocks[1];
			} else {
				l_NextOrientationCenter = this->m_Blocks[2];
			}
			break;
		case zBlock:
			/////////////////////////////
			// zBlock
			/////////////////////////////
			if(this->m_Orientation == EAST || this->m_Orientation == WEST){
				l_NextOrientationCenter = this->m_Blocks[1];
			} else {
				l_NextOrientationCenter = this->m_Blocks[2];
			}
			break;
		case tBlock:
			/////////////////////////////
			// tBlock
			/////////////////////////////
			if(this->m_Orientation == NORTH){
				l_NextOrientationCenter = this->m_Blocks[0];
			} else if(this->m_Orientation == EAST) {
				l_NextOrientationCenter = this->m_Blocks[2];
			} else if(this->m_Orientation == SOUTH) {
				l_NextOrientationCenter = this->m_Blocks[1];
			} else if(this->m_Orientation == WEST) {
				l_NextOrientationCenter = this->m_Blocks[1];
			}
			break;
		case lBlock:
			/////////////////////////////
			// lBlock
			/////////////////////////////
			if(this->m_Orientation == NORTH){
				l_NextOrientationCenter = this->m_Blocks[1];
			} else if(this->m_Orientation == EAST) {
				l_NextOrientationCenter.Horizontal = this->m_Blocks[1].Horizontal;
				l_NextOrientationCenter.Vertical = this->m_Blocks[1].Vertical+SEGMENT_HEIGHT;
			} else if(this->m_Orientation == SOUTH) {
				l_NextOrientationCenter = this->m_Blocks[2];
			} else if(this->m_Orientation == WEST) {
				l_NextOrientationCenter = this->m_Blocks[2];
			}
			break;
		case jBlock:
			/////////////////////////////
			// j Block
			/////////////////////////////
			if(this->m_Orientation == NORTH){
				l_NextOrientationCenter = this->m_Blocks[2];
			} else if(this->m_Orientation == EAST) {
				l_NextOrientationCenter= this->m_Blocks[1];
			} else if(this->m_Orientation == SOUTH) {
				l_NextOrientationCenter.Horizontal = this->m_Blocks[1].Horizontal;
				l_NextOrientationCenter.Vertical = this->m_Blocks[1].Vertical+SEGMENT_HEIGHT;
			} else if(this->m_Orientation == WEST) {
				l_NextOrientationCenter = this->m_Blocks[2];
			}
			break;
		default:
			break;
	}

	// Call the SetOrientatin, depending on current orientation
	switch(this->m_Orientation) {
		case NORTH:
			// Rotate to East orientation
			this->SetOrientationEast(l_NextOrientationCenter);
			break;
		case EAST:
			// Rotate to South orientation
			this->SetOrientationSouth(l_NextOrientationCenter);
			break;
		case SOUTH:
			// Rotate to West orientation
			this->SetOrientationWest(l_NextOrientationCenter);
			break;
		case WEST:
			// Rotate to North orientation
			this->SetOrientationNorth(l_NextOrientationCenter);
			break;

	}
}
///////////////////////////////////////////////////////////////////////////////////////
// ROTATE LEFT
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// ORIENTATION EAST
///////////////////////////////////////////////////////////////////////////////////////

/* SetOrientationEast
 *
 * This method places the Sprite in an horizontal initial position, it places the blocks and
 * creates the figure that should be represented
*/
void Sprite::SetOrientationEast(Point i_Point) {
	// Set orientation as East.
	this->m_Orientation = EAST;

	// Set Position
	switch(this->m_Type) {
		/////////////////////////////////////////////////////////////////////////////////////// O BLOCK - EAST
		case oBlock:
			/////////////////////////////
			// Square Block
			/////////////////////////////
			/* Will start centered
			 *
			 * Layout:
			 *
			 *  	+ - +  + - +
			 *  	| 0 |  | 1 |
			 *  	+ - +  * - +
			 *  	+ - +  + - +
			 *  	| 2 |  | 3 |
			 *  	+ - +  + - +
			 */
			// First block, upper left
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, upper right
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, down left
			m_Blocks[2].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, down right
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;
			break;

		/////////////////////////////////////////////////////////////////////////////////////// I BLOCK - EAST
		case iBlock:
			/////////////////////////////
			// I Block
			/////////////////////////////
			/*
			 *
			 *      + - +  + - +  + - +  + - +
			 *  	| 0 |  | 1 |  | 2 |  | 3 |
			 *  	+ - +  + - +  * - +  + - +
			 *
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - 2*SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal -+ SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// S BLOCK - EAST
		case sBlock:
			/////////////////////////////
			// s Block
			/////////////////////////////
			/*  	       + - +  + - +
			 *  	       | 0 |  | 1 |
			 *  	       + - +  * - +
			 *  	+ - +  + - +
			 *      | 2 |  | 3 |
			 *      + - +  + - +
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal - 2*SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// Z BLOCK - EAST
		case zBlock:
			/////////////////////////////
			// S Block
			/////////////////////////////
			/*
			 *
			 *  	+ - +  + - +
			 *  	| 0 |  | 1 |
			 *  	+ - +  * - +
			 *  	       + - +  + - +
			 *  	       | 2 |  | 3 |
			 *  	       + - +  + - +
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal+ SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// T BLOCK - EAST
		case tBlock:
			/////////////////////////////
			// T Block
			/////////////////////////////
			/*
			 *             + - +
			 *             | 0 |
			 *             * - +
			 *      + - +  + - + + - +
			 *	    | 1 |  | 2 | | 3 |
			 *      + - +  + - + + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		///////////////////////////////////////////////////////////////////////////////////////// L BLOCK - EAST
		case lBlock:
			/////////////////////////////
			// L Block
			/////////////////////////////
			/*
			 *      + - +  + - + + - +
			 *	    | 0 |  | 1 | | 2 |
			 *      + - +  * - + + - +
			 *      + - +
			 *      | 3 |
			 *      + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// J BLOCK - EAST
		case jBlock:
			/////////////////////////////
			// J Block
			/////////////////////////////
			/*
			 *      + - +  + - + + - +
			 *	    | 0 |  | 1 | | 2 |
			 *      + - +  * - + + - +
			 *                   + - +
			 *                   | 3 |
			 *                   + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;
		default:
			/* Not needed */
			break;
	}
}



///////////////////////////////////////////////////////////////////////////////////////
// ORIENTATION WEST
///////////////////////////////////////////////////////////////////////////////////////

/* SetOrientationWest
 *
 * This method places the Sprite in an horizontal initial position, it places the blocks and
 * creates the figure that should be represented
*/
void Sprite::SetOrientationWest(Point i_Point) {
	// Set orientation as West.
	this->m_Orientation = WEST;

	// Set Position
	switch(this->m_Type) {
		/////////////////////////////////////////////////////////////////////////////////////// O BLOCK - WEST
		case oBlock:
			/////////////////////////////
			// Square Block
			/////////////////////////////
			/*
			 *  	+ - +  + - +
			 *  	| 0 |  | 1 |
			 *  	+ - +  * - +
			 *  	+ - +  + - +
			 *  	| 2 |  | 3 |
			 *  	+ - +  + - +
			 * Nothing to be done. Only east is required initial, then it behaves the same
			 * */
			break;

		/////////////////////////////////////////////////////////////////////////////////////// I BLOCK - WEST
		case iBlock:
			/////////////////////////////
			// I Block
			/////////////////////////////
			/*
			 *      + - +  + - +  + - +  + - +
			 *  	| 0 |  | 1 |  | 2 |  | 3 |
			 *  	+ - +  + - +  * - +  + - +
			 *
			 * Same as East
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - 2*SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// S BLOCK - WEST
		case sBlock:
			/////////////////////////////
			// s Block
			/////////////////////////////
			/*  	       + - +  + - +
			 *  	       | 0 |  | 1 |
			 *  	       + - +  * - +
			 *  	+ - +  + - +
			 *      | 2 |  | 3 |
			 *      + - +  + - +
			 *
			 *  Same as East
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal - 2*SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// Z BLOCK - WEST
		case zBlock:
			/////////////////////////////
			// S Block
			/////////////////////////////
			/*
			 *
			 *  	+ - +  + - +
			 *  	| 0 |  | 1 |
			 *  	+ - +  * - +
			 *  	       + - +  + - +
			 *  	       | 2 |  | 3 |
			 *  	       + - +  + - +
			 *
			 *  Same as East
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal+ SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// T BLOCK - WEST
		case tBlock:
			/////////////////////////////
			// T Block
			/////////////////////////////
			/*
			 *      + - +  + - + + - +
			 *	    | 0 |  | 1 | | 2 |
			 *      + - +  * - + + - +
  			 *             + - +
			 *             | 3 |
			 *             + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		///////////////////////////////////////////////////////////////////////////////////////// L BLOCK - WEST
		case lBlock:
			/////////////////////////////
			// L Block
			/////////////////////////////
			/*
			 * 			         + - +
			 *                   | 0 |
			 *                   + - +
			 *      + - +  + - + + - +
			 *	    | 1 |  | 2 | | 3 |
			 *      + - +  * - + + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// J BLOCK - WEST
		case jBlock:
			/////////////////////////////
			// J Block
			/////////////////////////////
			/*		+ - +
			 *      | 0 |
			 *      + - +
			 *      + - +  + - + + - +
			 *	    | 1 |  | 2 | | 3 |
			 *      + - +  * - + + - +
			 *
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical;

			break;
		default:
			/* Not needed */
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// ORIENTATION NORTH
///////////////////////////////////////////////////////////////////////////////////////

/* SetOrientationWest
 *
 * This method places the Sprite in an horizontal initial position, it places the blocks and
 * creates the figure that should be represented
*/
void Sprite::SetOrientationNorth(Point i_Point) {
	// Set orientation as North.
	this->m_Orientation = NORTH;

	// Set Position
	switch(this->m_Type) {
		/////////////////////////////////////////////////////////////////////////////////////// O BLOCK - NORTH
		case oBlock:
			/////////////////////////////
			// Square Block
			/////////////////////////////
			/*
			 *  	+ - +  + - +
			 *  	| 0 |  | 1 |
			 *  	+ - +  * - +
			 *  	+ - +  + - +
			 *  	| 2 |  | 3 |
			 *  	+ - +  + - +
			 * Nothing to be done. Only east is required initial, then it behaves the same.
			 * */
			break;

		/////////////////////////////////////////////////////////////////////////////////////// I BLOCK - NORTH
		case iBlock:
			/////////////////////////////
			// I Block
			/////////////////////////////
			/*
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
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + 2*SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// S BLOCK - NORTH
		case sBlock:
			/////////////////////////////
			// s Block
			/////////////////////////////
			/*
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
			 *  Same as East
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// Z BLOCK - NORTH
		case zBlock:
			/////////////////////////////
			// S Block
			/////////////////////////////
			/*
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
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// T BLOCK - NORTH
		case tBlock:
			/////////////////////////////
			// T Block
			/////////////////////////////
			/*
			 *             + - +		(NORTH)
			 *             | 0 |
			 *             + - +
			 *      + - +  + - +
			 *	    | 1 |  | 2 |
			 *      + - +  * - +
			 *             + - +
			 *             | 3 |
			 *             + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		///////////////////////////////////////////////////////////////////////////////////////// L BLOCK - NORTH
		case lBlock:
			/////////////////////////////
			// L Block
			/////////////////////////////
			/*
			 *      + - +				(NORTH)
			 *	    | 0 |
			 *      + - +
			 *      + - +
			 *      | 1 |
			 *      * - +
			 *      + - +  + - +
			 *      | 2 |  | 3 |
			 *      + - +  + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// J BLOCK - NORTH
		case jBlock:
			/////////////////////////////
			// J Block
			/////////////////////////////
			/*
			 *      + - +  + - +		(NORTH)
			 *      | 0 |  | 1 |
			 *      + - +  + - +
			 *      + - +
			 *	    | 2 |
			 *      * - +
			 *      + - +
			 *      | 3 |
			 *      + - +
			 *
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;
		default:
			/* Not needed */
			break;
	}
}



///////////////////////////////////////////////////////////////////////////////////////
// ORIENTATION SOUTH
///////////////////////////////////////////////////////////////////////////////////////

/* SetOrientationSouth
 *
 * This method places the Sprite in a South orientation, it places the blocks and
 * creates the figure that should be represented, based on the Point given as a reference.
*/
void Sprite::SetOrientationSouth(Point i_Point) {
	// Set orientation as South.
	this->m_Orientation = SOUTH;

	// Set Position
	switch(this->m_Type) {
		/////////////////////////////////////////////////////////////////////////////////////// O BLOCK - SOUTH
		case oBlock:
			/////////////////////////////
			// Square Block
			/////////////////////////////
			/*
			 *  	+ - +  + - +
			 *  	| 0 |  | 1 |
			 *  	+ - +  * - +
			 *  	+ - +  + - +
			 *  	| 2 |  | 3 |
			 *  	+ - +  + - +
			 * Nothing to be done. Only east is required initial, then it behaves the same.
			 * */
			break;

		/////////////////////////////////////////////////////////////////////////////////////// I BLOCK - SOUTH
		case iBlock:
			/////////////////////////////
			// I Block
			/////////////////////////////
			/*
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
			 */
			// First block, upper most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, upper center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, down center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, down-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + 2*SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// S BLOCK - SOUTH
		case sBlock:
			/////////////////////////////
			// s Block
			/////////////////////////////
			/*
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
			 *  Same as East
			 */
			// First block, upper left segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right down segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// Z BLOCK - SOUTH
		case zBlock:
			/////////////////////////////
			// S Block
			/////////////////////////////
			/*
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
			 */
			// First block, upper right segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, right middle segment
			m_Blocks[1].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, left center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, left down segment
			m_Blocks[3].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// T BLOCK - SOUTH
		case tBlock:
			/////////////////////////////
			// T Block
			/////////////////////////////
			/*
			 *      + - +			(SOUTH)
			 *      | 0 |
			 *      + - +
			 *      + - +  + - +
			 *	    | 1 |  | 2 |
			 *      * - +  + - +
			 *      + - +
			 *      | 3 |
			 *      + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal + SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		///////////////////////////////////////////////////////////////////////////////////////// L BLOCK - SOUTH
		case lBlock:
			/////////////////////////////
			// L Block
			/////////////////////////////
			/*
			 *      + - + + - +			(SOUTH)
			 *	    | 0 | | 1 |
			 *      + - + + - +
			 *            + - +
			 *            | 2 |
			 *            * - +
			 *            + - +
			 *            | 3 |
			 *            + - +
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal;
			m_Blocks[2].Vertical = i_Point.Vertical;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;

		/////////////////////////////////////////////////////////////////////////////////////// J BLOCK - SOUTH
		case jBlock:
			/////////////////////////////
			// J Block
			/////////////////////////////
			/*
			 *     		  + - +		(SOUTH)
			 *            | 0 |
			 *            + - +
			 *            + - +
			 *	          | 1 |
			 *            * - +
			 *      + - + + - +
			 *      | 2 | | 3 |
			 *      + - + + - +
			 *
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = i_Point.Horizontal;
			m_Blocks[0].Vertical = i_Point.Vertical - SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = i_Point.Horizontal;
			m_Blocks[1].Vertical = i_Point.Vertical;

			// Third block, right center segment
			m_Blocks[2].Horizontal = i_Point.Horizontal - SEGMENT_WIDTH;
			m_Blocks[2].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = i_Point.Horizontal;
			m_Blocks[3].Vertical = i_Point.Vertical + SEGMENT_HEIGHT;

			break;
		default:
			/* Not needed */
			break;
	}
}

