/*
 * Sprite.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
 */

#include <Sprite.hpp>

Sprite::Sprite() {
	// Generate random sprite type
	this->m_Type = static_cast<eSpriteTypes> (rand() % NUM_SPRITE_TYPES);
	this->SetColor();
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

/* Sprite Paint
 *
 * Paints the four blocks in the corresponding color of the
 * sprite.
 *
 */
void Sprite::Paint() {
	for(uint8_t i=0; i<NUM_BLOCKS; i++){
		this->PaintSegment(i,this->m_Color);
	}
}

/* Define Color
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


/* Sprite Delete
 *
 * This method erases from the screen the current position
 * of the sprite.
 *
 */
void Sprite::Delete() {
	for(int i=0; i<NUM_BLOCKS; i++){
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
	l_PaintArea.xMax = MIN_ARENA_X + m_Blocks[i_SegmentNum].Horizontal + SEGMENT_WIDTH;
	l_PaintArea.yMin = MIN_ARENA_Y + m_Blocks[i_SegmentNum].Horizontal - SEGMENT_HEIGHT;
	l_PaintArea.yMax = MIN_ARENA_Y + m_Blocks[i_SegmentNum].Horizontal;

	// Paint the rectangle
	Graphics_fillRectangleOnDisplay(this->a_GraphicsContext.display, &l_PaintArea, i_Color);
}
