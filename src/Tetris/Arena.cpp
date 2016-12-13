/*
 * Arena.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */

#include <Arena.hpp>

/* Arena Constructor
 *
 * It creates an Arena instance. Initializes the arena area
 * and the score area for painting. Also resets the score,
 * lost condition and matrix values to background color.
 *
 */

Arena::Arena() {

	this->m_ArenaArea.xMin = MIN_ARENA_X;
	this->m_ArenaArea.xMax = MAX_ARENA_X;
	this->m_ArenaArea.yMin = MIN_ARENA_Y;
	this->m_ArenaArea.yMax = MAX_ARENA_Y;

	this->m_ScoreArea.xMin = MIN_SCORE_X;
	this->m_ScoreArea.xMax = MAX_SCORE_X;
	this->m_ScoreArea.yMin = 0;
	this->m_ScoreArea.yMax = MAX_HEIGHT;

	this->m_u16Score = 0;
	this->m_m8Level = 0;
	this->m_bLost = false;

	this->PaintArena();

}

Arena::~Arena() {
	// TODO Auto-generated destructor stub
}

/* PaintArena
 *
 *	Paints at the screen all the arena and score area.
 */

void Arena::PaintArena(){
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_ArenaArea, BACKGROUND_COLOR);
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_ScoreArea, ARENA_COLOR);

	Graphics_Rectangle l_ArenaBase;
	l_ArenaBase.xMin = 0;
	l_ArenaBase.xMax = 127;
	l_ArenaBase.yMin = MAX_ARENA_Y+1;
	l_ArenaBase.yMax = 127;

	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &l_ArenaBase, FILL_COLOR);

	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *)"SCORE", AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT/3, TRANSPARENT_TEXT);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *)"LEVEL", AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT*2/3, TRANSPARENT_TEXT);

	this->UpdateScore();
	this->UpdateLevel();
	this->ClearMatrix();
}

/* LostScreen
 *
 *	Display the final score and a message to the player who lost.
 *
 */

void Arena::LostScreen(){
	Graphics_Rectangle m_PaintArea;

	// Define square to paint:
	m_PaintArea.xMin = 0;
	m_PaintArea.xMax = 127;
	m_PaintArea.yMin = 0;
	m_PaintArea.yMax = 127;

	// Paint the rectangle
	Graphics_fillRectangleOnDisplay(Arena::m_GraphicsContext->display, &m_PaintArea, LOST_COLOR);

	char l_ScoreString [8];
	sprintf(l_ScoreString, "%d", m_u16Score);

	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *)"YOU LOST!", AUTO_STRING_LENGTH, SCREEN_SIZE/2, SCREEN_SIZE/2-16, TRANSPARENT_TEXT);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *)"SCORE: ", AUTO_STRING_LENGTH, SCREEN_SIZE/2, SCREEN_SIZE/2, TRANSPARENT_TEXT);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *) l_ScoreString, AUTO_STRING_LENGTH, SCREEN_SIZE/2, SCREEN_SIZE/2+16, TRANSPARENT_TEXT);
}

/* UpdateScore
 *
 *	Update the score at the screen.
 */


void Arena::UpdateScore(){
	char string[8];
	sprintf(string, "%d", m_u16Score);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *) string, AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT/3+8, OPAQUE_TEXT);
}


/* UpdateLavel
 *
 * Updates level value at the screen.
 *
 */

void Arena::UpdateLevel(){
	m_m8Level++;
	char string[8];
	sprintf(string, "%d", m_m8Level);
	Graphics_drawStringCentered(Arena::m_GraphicsContext,(int8_t *) string, AUTO_STRING_LENGTH, MAX_SCORE_X/2, MAX_HEIGHT*2/3+8, OPAQUE_TEXT);
}

/* ClearMatrix
 *
 * Resets all matrix values to background color. It means they are free to occupy.
 *
 */

void Arena::ClearMatrix(){
	for(int i=0;i<NUM_X_SQUARES;i++){
		for(int j=0;j<NUM_Y_SQUARES;j++){
				this->m_u16GameMatrix[i][j]= BACKGROUND_COLOR;
		}
	}
}

/* CheckCollision
 *
 *	Checks collision of a Sprite at descending movement.
 *	Basically uses the coordinates of the sprite in the screen to check
 *	if its corresponding index at matrix are occupied. Returns true if
 *	finds a collision else false.
 *
 *	Also sets lost boolean if the screen is complete.
 *
 *	@param i_CurrentSprite: Sprite object to check coordinates.
 */

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

/* CheckHorizontal Collision
 *
 *	Checks collision of a Sprite depending on the movement direction:
 *	Left,Right, rotate clockwise or counterclockwise.
 *	Basically uses the coordinates of the sprite in the screen to check
 *	id its corresponding index at matrix are occupied. Returns true if
 *	finds a collision else false.
 *
 *	@param i_CurrentSprite: Sprite object to check coordinates.
 *	@param i_Direction: Kind of movement that will perform the sprite.
 */

bool Arena::CheckHorizontalCollision(Sprite* i_CurrentSprite, eGlobalFlags i_Direction){

	uint8_t i_u8Vertical = 0;
	uint8_t i_u8Horizontal = 0;
	uint8_t i_u8UpperVertical = 0;

	for(int i=0; i<NUM_BLOCKS; i++){

		i_u8Vertical = (uint8_t)(i_CurrentSprite->m_Blocks[i].Vertical-1)/SEGMENT_HEIGHT;
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


			if(i_u8UpperVertical && GetMatrixValue(i_u8Horizontal, i_u8UpperVertical) != BACKGROUND_COLOR){
				if(i_CurrentSprite->m_Blocks[i].Vertical%SEGMENT_HEIGHT == 0) {
					return false;
				}
				return true;
			}

		}else{return true;}
	}

	return false;
}

/* GetMatrixValue
 *
 *	Gets a value from the matrix. It takes the horizontal
 *	and vertical index.
 *
 *	@param i_Horizontal: Horizontal index from matrix.
 *	@param i_Vertical: Vertical index from matrix.
 */

uint16_t Arena::GetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical){
	return this->m_u16GameMatrix[i_Horizontal][i_Vertical];
}

/* SetMatrixValue
 *
 *	Sets a value into the matrix.
 *
 *	@param i_Horizontal: Horizontal index from matrix.
 *	@param i_Vertical: Vertical index from matrix.
 *	@param i_Value: Value to save into matrix.
 */


void Arena::SetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical, uint16_t i_Value){
	this->m_u16GameMatrix[i_Horizontal][i_Vertical] = i_Value;
}

/* PaintFromLine
 *
 * Paints all the segments from the game matrix,
 * staring from an specific row. The index row will be determined
 * by function CheckRows.
 *
 *
 * @param i_Row: Row index from which start painting the matrix at the screen
 *
 */

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

/* PaintSegment
 *
 * This function paints one segment depending on its
 * vertical and horizontal position at the Game Matrix.
 *
 * @param i_Horizontal: Horizontal index in Game Matrix.
 * @param i_Vertical: Horizontal index in Game Matrix.
 *
 */

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

/* UpdateMatrix
 *
 * This function updates the values at the matrix
 * when some collision happens. It checks all the blocks
 * of the sprite and saves the color at relative position
 * in the game matrix.
 *
 * @param i_CurrentSprite: Sprite to add to the game matrix.
 *
 *
 */


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

/* CheckRows
 *
 * This function checks which row of the matrix is complete.
 * If it finds filled lines it will down the matrix one row
 * for each line found.
 * The method returns the index of first completed line found from
 * bottom to top.
 * Also updates the score value.
 */

uint8_t Arena::CheckRows(){

	uint8_t i_u8FirstLineFound=NUM_Y_SQUARES;
	uint8_t l_u8NumberOfLinesDeleted = 0;
	bool	b_FirstLineFound = true;

	for(int i=NUM_Y_SQUARES-1; i>=0; i--){

		if(this->LineComplete(i) && b_FirstLineFound){
			b_FirstLineFound = false;
			i_u8FirstLineFound = i;
			this->DownARow(i);
			i++;
			// Update Score
			l_u8NumberOfLinesDeleted++;
			m_u16Score += ONE_ROW_DELETED;
		}else if(this->LineComplete(i)){
			this->DownARow(i);
			i++;
			l_u8NumberOfLinesDeleted++;
			m_u16Score += ONE_ROW_DELETED + l_u8NumberOfLinesDeleted*MULTIPLE_ROWS_BONUS;
		}
	}

	if(i_u8FirstLineFound)
		UpdateScore();
	return i_u8FirstLineFound;
}

/* LineComplete
 *
 * Returns true if all the line is complete.
 * Otherwise returns false.
 *
 * @param i_Row: Row index to check in matrix.
 *
 */

bool Arena::LineComplete(uint8_t i_Row){
	bool b_LineComplete = true;
	for(int x=0; x<NUM_X_SQUARES; x++){
		if(this->m_u16GameMatrix[x][i_Row]==BACKGROUND_COLOR){
			b_LineComplete = false;
		}
	}
	return b_LineComplete;
}

/* CopyUpperRow
 *
 * Copies the row above. If is the top line it fills
 * the line with empty spaces(background color).
 *
 * @param i_RefRow: Row index to copy the row above.
 *
 *
 */

void Arena::CopyUpperRow(uint8_t i_RefRow){
	for(int x=0; x<NUM_X_SQUARES; x++){
		if(i_RefRow == 0){
			this->m_u16GameMatrix[x][i_RefRow] = BACKGROUND_COLOR;
		}else{
			this->m_u16GameMatrix[x][i_RefRow] = this->m_u16GameMatrix[x][i_RefRow-1];
		}
	}
}

/* DownARow
 *
 * Copies the row above. If is the top line it fills
 * the line with empty spaces(background color).
 *
 * @param i_RefRow: Row index from which start bringing
 * 					down the game matrix.
 *
 */
void Arena::DownARow(uint8_t i_RefRow){
	for(int i=i_RefRow; i>=0; i--){
		this->CopyUpperRow(i);
	}
}

/* SetLost
 *
 * Sets m_bLost parameter.
 *
 * @param i_State: True or false.
 *
 */

void Arena::SetLost(bool i_State){
	this->m_bLost = i_State;
}

/* GetLost
 *
 * Returns m_bLost parameter.
 *
 */

bool Arena::GetLost(){
	return this->m_bLost;
}

/* PlayerLost
 *
 * Public method that returns m_bLost parameter.
 *
 */

bool Arena::PlayerLost(){
	return GetLost();
}

