/*
 * Area.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: fabian
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
	// 1-> Horizontal
	// 0-> Vertical
	bool CheckCollision(Sprite* i_CurrentSprite);
	bool CheckHorizontalCollision(Sprite* i_CurrentSprite,eGlobalFlags i_Direction);
	void PaintMatrix();
	uint8_t CheckRows();

private:
	// Game Matrix
	uint16_t m_u16GameMatrix[NUM_X_SQUARES][NUM_Y_SQUARES];
	// Score
	uint8_t m_u8Score;
	// Area of the Arena
	Graphics_Rectangle m_ArenaArea;
	// Area of the Score Screen
	Graphics_Rectangle m_ScoreArea;
	void UpdateScore();
	void ClearMatrix();
	uint16_t GetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical);
	void SetMatrixValue(uint8_t i_Horizontal, uint8_t i_Vertical, uint16_t i_Value);
	void UpdateMatrix(Sprite* i_CurrentSprite);
	bool LineComplete(uint8_t i_Row);
	void CopyUpperRow(uint8_t i_RefRow);
	void DownARow(uint8_t i_RefRow);
};

#endif /* ARENA_HPP_ */
