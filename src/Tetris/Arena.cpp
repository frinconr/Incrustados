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
	this->m_bLost = false;

	this->PaintArena();

}

Arena::~Arena() {
	// TODO Auto-generated destructor stub
}

void Arena::PaintArena(){
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_ArenaArea, BACKGROUND_COLOR);
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_ScoreArea, SCORE_COLOR);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *)"SCORE", AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT/2, TRANSPARENT_TEXT);
	this->UpdateScore();
	this->ClearMatrix();
}

void Arena::LostScreen(){
	Graphics_Rectangle m_PaintArea;

	// Define square to paint:
	m_PaintArea.xMin = 0;
	m_PaintArea.xMax = 127;
	m_PaintArea.yMin = 0;
	m_PaintArea.yMax = 127;

	// Paint the rectangle
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_PaintArea, FILL_COLOR);
}


void Arena::UpdateScore(){
	char string[8];
	sprintf(string, "%d", m_u8Score);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *) string, AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT/2+8, TRANSPARENT_TEXT);
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
				if(i_CurrentSprite->m_Blocks[0].Vertical==10){
					SetLost(true);
				}

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
	uint8_t i_u8UpperVertical = 0;

	for(int i=0; i<NUM_BLOCKS; i++){

		i_u8Vertical = (uint8_t)(i_CurrentSprite->m_Blocks[i].Vertical)/SEGMENT_HEIGHT;
		i_u8UpperVertical = (uint8_t)(i_CurrentSprite->m_Blocks[i].Vertical-SEGMENT_HEIGHT+1)/SEGMENT_HEIGHT;
		i_u8Horizontal = i_CurrentSprite->m_Blocks[i].Horizontal/SEGMENT_WIDTH;

		if(i_u8Horizontal<NUM_X_SQUARES){

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
					i_u8Horizontal++;

					break;
				case MOVE_LEFT:
					i_u8Horizontal--;
					break;
				default:
					break;
			}

			if(GetMatrixValue(i_u8Horizontal, i_u8Vertical) != BACKGROUND_COLOR){
				return true;
			}

			if(i_u8UpperVertical && GetMatrixValue(i_u8Horizontal, i_u8UpperVertical) != BACKGROUND_COLOR){return true;}

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

void Arena::PaintFromLine(uint8_t i_Row){

	if(i_Row == NUM_Y_SQUARES ){return;}

	Graphics_Rectangle m_PaintArea;

	for(int j=i_Row;j>=0;j--){
		for(int i=0;i<NUM_X_SQUARES;i++){

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

void Arena::PaintSegment(uint8_t i_Horizontal, uint8_t i_Vertical){
	Graphics_Rectangle m_PaintArea;

	// Define square to paint:
	m_PaintArea.xMin = MIN_ARENA_X + i_Horizontal*SEGMENT_WIDTH;
	m_PaintArea.xMax = MIN_ARENA_X + i_Horizontal*SEGMENT_WIDTH + (SEGMENT_WIDTH-1);
	m_PaintArea.yMin = MIN_ARENA_Y + (i_Vertical+1)*SEGMENT_HEIGHT - (SEGMENT_HEIGHT-1);
	m_PaintArea.yMax = MIN_ARENA_Y + (i_Vertical+1)*SEGMENT_HEIGHT;

	// Paint the rectangle
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_PaintArea, GetMatrixValue(i_Horizontal,i_Vertical));
}


void Arena::UpdateMatrix(Sprite* i_CurrentSprite){

	uint8_t i_u8Vertical = 0;
	uint8_t i_u8Horizontal = 0;

	for(int i=0; i<NUM_BLOCKS; i++){
		i_u8Vertical = (i_CurrentSprite->m_Blocks[i].Vertical + VERTICAL_JUMP)/10 - 1;
		i_u8Horizontal = i_CurrentSprite->m_Blocks[i].Horizontal/10;
		SetMatrixValue(i_u8Horizontal,i_u8Vertical,i_CurrentSprite->m_Color);
		PaintSegment(i_u8Horizontal,i_u8Vertical);
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
			i++;
		}else if(this->LineComplete(i)){
			this->DownARow(i);
			i++;
		}
	}
	return i_u8FirstLineFound;
}

bool Arena::LineComplete(uint8_t i_Row){
	bool b_LineComplete = true;
	for(int x=0; x<NUM_X_SQUARES; x++){
		if(this->m_u16GameMatrix[x][i_Row]==BACKGROUND_COLOR){
			b_LineComplete = false;
		}
	}
	return b_LineComplete;
}

void Arena::CopyUpperRow(uint8_t i_RefRow){
	for(int x=0; x<NUM_X_SQUARES; x++){
		if(i_RefRow == 0){
			this->m_u16GameMatrix[x][i_RefRow] = BACKGROUND_COLOR;
		}else{
			this->m_u16GameMatrix[x][i_RefRow] = this->m_u16GameMatrix[x][i_RefRow-1];
		}
	}
}
void Arena::DownARow(uint8_t i_RefRow){
	for(int i=i_RefRow; i>=0; i--){
		this->CopyUpperRow(i);
	}
}

void Arena::SetLost(bool i_State){
	this->m_bLost = i_State;
}

bool Arena::GetLost(){
	return this->m_bLost;
}

bool Arena::PlayerLost(){
	return GetLost();
}

