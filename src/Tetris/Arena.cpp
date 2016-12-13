/*
 * Area.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
 */

#include <Arena.hpp>

Arena::Arena() {

	this->m_ArenaArea.xMin = MIN_ARENA_X;
	this->m_ArenaArea.xMax = MAX_ARENA_X;
	this->m_ArenaArea.yMin = MIN_ARENA_Y;
	this->m_ArenaArea.yMax = MAX_ARENA_Y;

	this->m_ScoreArea.xMin = MIN_SCORE_X;
	this->m_ScoreArea.xMax = MAX_SCORE_X;
	this->m_ScoreArea.yMin = 0;
	this->m_ScoreArea.yMax = MAX_HEIGHT;

	this->m_u8Score = 0;

	this->PaintArena();

}

Arena::~Arena() {
	// TODO Auto-generated destructor stub
}

void Arena::PaintArena(){
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_ArenaArea, BACKGROUND_COLOR);
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_ScoreArea, ARENA_COLOR);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *)"SCORE", AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT/2, OPAQUE_TEXT);
	this->UpdateScore();
	this->ClearMatrix();
}

void Arena::UpdateScore(){
	char string[8];
	sprintf(string, "%d", m_u8Score);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *) string, AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT/2+8, OPAQUE_TEXT);
}

void Arena::ClearMatrix(){
	for(int i=0;i<NUM_X_SQUARES;i++){
		for(int j=0;j<NUM_Y_SQUARES;j++){
				this->m_u16GameMatrix[i][j]= BACKGROUND_COLOR;
		}
	}
}

bool Arena::CheckCollision(Sprite* i_CurrentSprite){

	if(i_CurrentSprite->m_Blocks[0].Vertical%SEGMENT_HEIGHT == 0 ){

		uint8_t i_u8Vertical = 0;
		uint8_t i_u8Horizontal = 0;

		for(int i=0; i<NUM_BLOCKS; i++){
			i_u8Vertical = (i_CurrentSprite->m_Blocks[i].Vertical)/10;
			i_u8Horizontal = i_CurrentSprite->m_Blocks[i].Horizontal/10;
			if(i_u8Vertical == NUM_Y_SQUARES){
				this->UpdateMatrix(i_CurrentSprite);
				return true;
			}else if(GetMatrixValue(i_u8Horizontal, i_u8Vertical) != BACKGROUND_COLOR){
				this->UpdateMatrix(i_CurrentSprite);
				return true;
			}
		}
	}
	return false;
}

bool Arena::CheckHorizontalCollision(Sprite* i_CurrentSprite, eGlobalFlags i_Direction){

	uint8_t i_u8Vertical = 0;
	uint8_t i_u8Horizontal = 0;

	for(int i=0; i<NUM_BLOCKS; i++){

		i_u8Vertical = (uint8_t)(i_CurrentSprite->m_Blocks[i].Vertical)/10;
		i_u8Horizontal = i_CurrentSprite->m_Blocks[i].Horizontal/10;

		if(i_u8Horizontal>=0 && i_u8Horizontal<10){

			if(i_u8Vertical >= NUM_Y_SQUARES){
				return true;
			}

			if(i_u8Horizontal == 0 && i_Direction == MOVE_LEFT){
				return false;
			}

			if(i_u8Horizontal == NUM_X_SQUARES-1  && i_Direction == MOVE_RIGHT){
				return false;
			}

			switch(i_Direction){
				case MOVE_RIGHT:
					i_u8Horizontal = i_u8Horizontal + 1;
					break;
				case MOVE_LEFT:
					i_u8Horizontal = i_u8Horizontal - 1;
					break;
				default:
					break;
			}

			if(GetMatrixValue(i_u8Horizontal, i_u8Vertical) != BACKGROUND_COLOR){
				return true;
			}
		}else{return true;}
	}

	return false;
}



uint16_t Arena::GetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical){
	return this->m_u16GameMatrix[i_Horizontal][i_Vertical];
}

void Arena::SetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical, uint16_t i_Value){
	this->m_u16GameMatrix[i_Horizontal][i_Vertical] = i_Value;
}

void Arena::PaintMatrix(){
	Graphics_Rectangle m_PaintArea;

	for(int i=0;i<NUM_X_SQUARES;i++){
			for(int j=0;j<NUM_Y_SQUARES;j++){
				if(GetMatrixValue(i,j)!= BACKGROUND_COLOR){
					// Define square to paint:
					m_PaintArea.xMin = MIN_ARENA_X + i*SEGMENT_WIDTH;
					m_PaintArea.xMax = MIN_ARENA_X + i*SEGMENT_WIDTH + (SEGMENT_WIDTH-1);
					m_PaintArea.yMin = MIN_ARENA_Y + (j+1)*SEGMENT_HEIGHT - (SEGMENT_HEIGHT-1);
					m_PaintArea.yMax = MIN_ARENA_Y + (j+1)*SEGMENT_HEIGHT;

					// Paint the rectangle
					Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_PaintArea, GetMatrixValue(i,j));
				}
			}
		}
}

void Arena::UpdateMatrix(Sprite* i_CurrentSprite){

	uint8_t i_u8Vertical = 0;
	uint8_t i_u8Horizontal = 0;

	for(int i=0; i<NUM_BLOCKS; i++){
		i_u8Vertical = (i_CurrentSprite->m_Blocks[i].Vertical + VERTICAL_JUMP)/10 - 1;
		i_u8Horizontal = i_CurrentSprite->m_Blocks[i].Horizontal/10;
		SetMatrixValue(i_u8Horizontal,i_u8Vertical,i_CurrentSprite->m_Color);
	}
}

uint8_t Arena::CheckRows(){

	uint8_t i_u8FirstLineFound=NUM_Y_SQUARES;
	bool	b_FirstLineFound = true;

	for(int i=NUM_Y_SQUARES-1; i>=0; i--){

		if(this->LineComplete(i) && b_FirstLineFound){
			b_FirstLineFound = false;
			i_u8FirstLineFound = i;
			this->DownARow(i);
		}else if(this->LineComplete(i)){
			this->DownARow(i);
		}
	}
	return i_u8FirstLineFound;
}

bool Arena::LineComplete(uint8_t i_Row){
	bool b_LineComplete = true;
	for(int Column=0; Column<NUM_X_SQUARES; Column++){
		if(this->m_u16GameMatrix[i_Row][Column]==BACKGROUND_COLOR){
			b_LineComplete = false;
		}
	}
	return b_LineComplete;
}

void Arena::CopyUpperRow(uint8_t i_RefRow){
	for(int Column=0; Column<NUM_X_SQUARES; Column++){
		if(i_RefRow == 0){
			this->m_u16GameMatrix[i_RefRow][Column] = BACKGROUND_COLOR;
		}else{
			this->m_u16GameMatrix[i_RefRow][Column] = this->m_u16GameMatrix[i_RefRow-1][Column];
		}
	}
}
void Arena::DownARow(uint8_t i_RefRow){
	for(int i=i_RefRow; i>=0; i--){
		this->CopyUpperRow(i);
	}
}


