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
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
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
		case tBlock:
			this->m_Color = COLOR_YELLOW;
			break;
		case lBlock:
			this->m_Color = COLOR_GRAY;
			break;
		default:
			this->m_Color = COLOR_GRAY;
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
		// Call the paint segment method with the bacground color
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
	Graphics_Rectangle l_PaintArea;
	// Define square to paint:
	l_PaintArea.xMin = MIN_ARENA_X + m_Blocks[i_SegmentNum].Horizontal;
	l_PaintArea.xMax = MIN_ARENA_X + m_Blocks[i_SegmentNum].Horizontal + (SEGMENT_WIDTH-1);
	l_PaintArea.yMin = MIN_ARENA_Y + m_Blocks[i_SegmentNum].Vertical - (SEGMENT_HEIGHT-1);
	l_PaintArea.yMax = MIN_ARENA_Y + m_Blocks[i_SegmentNum].Vertical;

	// Paint the rectangle
	Graphics_fillRectangleOnDisplay(this->m_GraphicsContext.display, &l_PaintArea, i_Color);
}

/* SetInitialPosition
 *
 * This method places the Sprite in a hardcoded initial position, it places the blocks and
 * creates the figure that should be represented
*/
void Sprite::SetInitialPosition() {
	//
	switch(this->m_Type) {
		case oBlock:
			/////////////////////////////
			// Square Block
			/////////////////////////////
			/* Will start centered
			 *
			 * Layout:
			 *  	[0][1]
			 *  	[2][3]
			 *
			 *  	   |
			 */
			// First block, upper left
			m_Blocks[0].Horizontal = (ARENA_WIDTH/2)- SEGMENT_WIDTH;
			m_Blocks[0].Vertical = SEGMENT_HEIGHT;

			// Second segment, upper right
			m_Blocks[1].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[1].Vertical = SEGMENT_HEIGHT;

			// Third block, down left
			m_Blocks[2].Horizontal = (ARENA_WIDTH/2)-SEGMENT_WIDTH;
			m_Blocks[2].Vertical = 2*SEGMENT_HEIGHT;

			// Fourth block, down right
			m_Blocks[3].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[3].Vertical = 2*SEGMENT_HEIGHT;
			break;

		case iBlock:
			/////////////////////////////
			// I Block
			/////////////////////////////
			/* Will start horizontal
			 *
			 *  [0][1][2][3]
			 *
			 *  	  |
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = (ARENA_WIDTH/2)- 2*SEGMENT_WIDTH;
			m_Blocks[0].Vertical = SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = (ARENA_WIDTH/2)- SEGMENT_WIDTH;
			m_Blocks[1].Vertical = SEGMENT_HEIGHT;

			// Third block, right center segment
			m_Blocks[2].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[2].Vertical = SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = (ARENA_WIDTH/2)+ SEGMENT_WIDTH;
			m_Blocks[3].Vertical = SEGMENT_HEIGHT;

			break;

		case sBlock:
			/////////////////////////////
			// S Block
			/////////////////////////////
			/* Will start horizontal
			 *
			 *  	[0][1]
			 *  	   [2][3]
			 *
			 *  	   |
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = (ARENA_WIDTH/2)- SEGMENT_WIDTH;
			m_Blocks[0].Vertical = SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[1].Vertical = SEGMENT_HEIGHT;

			// Third block, right center segment
			m_Blocks[2].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[2].Vertical = 2*SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = (ARENA_WIDTH/2) + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = 2*SEGMENT_HEIGHT;

			break;

		case tBlock:
			/////////////////////////////
			// T Block
			/////////////////////////////
			/* Will start horizontal
			 *
			 *  	 [0]
			 *    [1][2][3]
			 *
			 *    	 |
			 */
			// First block, left most segment
			m_Blocks[0].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[0].Vertical = SEGMENT_HEIGHT;

			// Second segment, left center segment
			m_Blocks[1].Horizontal = (ARENA_WIDTH/2) - SEGMENT_WIDTH;
			m_Blocks[1].Vertical = 2*SEGMENT_HEIGHT;

			// Third block, right center segment
			m_Blocks[2].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[2].Vertical = 2*SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = (ARENA_WIDTH/2) + SEGMENT_WIDTH;
			m_Blocks[3].Vertical = 2*SEGMENT_HEIGHT;

			break;
		case lBlock:
			/////////////////////////////
			// L Block
			/////////////////////////////
			/* Will start horizontal
			 *
			 *          [0]
			 *    [1][2][3]
			 *
			 *    	    |
			 */
			// First block, upper segment
			m_Blocks[0].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[0].Vertical = SEGMENT_HEIGHT;

			// Second segment, left-most segment
			m_Blocks[1].Horizontal = (ARENA_WIDTH/2) - 2*SEGMENT_WIDTH;
			m_Blocks[1].Vertical = 2*SEGMENT_HEIGHT;

			// Third block, down center segment
			m_Blocks[2].Horizontal = (ARENA_WIDTH/2) - SEGMENT_WIDTH;
			m_Blocks[2].Vertical = 2*SEGMENT_HEIGHT;

			// Fourth block, right-most segment
			m_Blocks[3].Horizontal = (ARENA_WIDTH/2);
			m_Blocks[3].Vertical = 2*SEGMENT_HEIGHT;

			break;
		default:
			/* Not needed */
			break;
	}
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
