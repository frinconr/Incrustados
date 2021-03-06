/*
 * Arena.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: Fabian Meléndez
 *      		Felipe Rincón
 */

#ifndef ARENA_HPP_
#define ARENA_HPP_

/* MSPWare definitions */
#include "msp.h"

/* C++ Standard libs */
#include <stdio.h>      /* printf, scanf, puts, NULL */

extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
	#include <stdio.h>
}

/* Local Definitions */
#include "Definitions.hpp"
#include "Sprite.hpp"

class Arena {

public:
	Arena();
	virtual ~Arena();
	void PaintArena();

	// Static Graphic Context
	static Graphics_Context* m_GraphicsContext;
	//Check Collisions
	bool CheckCollision(Sprite* i_CurrentSprite);
	bool CheckHorizontalCollision(Sprite* i_CurrentSprite,eGlobalFlags i_Direction);
	// Paint the matrix from an specific line
	void PaintFromLine(uint8_t i_Row);
	// Check if any row is complete.
	uint8_t CheckRows();
	// Return if player lost the game
	bool PlayerLost();
	// Paints the lost screen
	void LostScreen();
	void UpdateLevel();
private:
	// Game Matrix
	uint16_t m_u16GameMatrix[NUM_X_SQUARES][NUM_Y_SQUARES];
	// Score
	uint16_t m_u16Score;
	uint8_t m_m8Level;
	// Lost Boolean
	bool m_bLost;
	// Area of the Arena
	Graphics_Rectangle m_ArenaArea;
	// Area of the Score Screen
	Graphics_Rectangle m_ScoreArea;
	// Update Score
	void UpdateScore();
	// Manage matrix values
	void ClearMatrix();
	uint16_t GetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical);
	void SetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical, uint16_t i_Value);
	void UpdateMatrix(Sprite* i_CurrentSprite);
	bool LineComplete(uint8_t i_Row);
	void CopyUpperRow(uint8_t i_RefRow);
	void DownARow(uint8_t i_RefRow);

	// Paint a Segment
	void PaintSegment(uint8_t i_Horizontal, uint8_t i_Vertical);
	// Manage m_bLost boolean
	bool GetLost();
	void SetLost(bool i_State);
};

#endif /* ARENA_HPP_ */
