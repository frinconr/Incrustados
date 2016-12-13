/*
 * Music.hpp
 *
 *  Created on: Dec 07, 2016
 *      Author: Fabian Melendez
 *      		Felipe Rincón
 */

#ifndef MUSIC_HPP_
#define MUSIC_HPP_

#include "hardware.hpp"

#define ClockFrequency 187500

uint8_t g_u8NoteCountDown;
uint8_t g_u8NextNoteIndex;

typedef enum {
	C_4 = (718),
	A_4 = (426),
	B_4 = (380),
	C_5 = (358),
	D_5 = (319),
	E_5 = (284)
} eNoteFrequency;

// At Black - 144.
typedef enum {
	Black = 410,
	Corchea = 205,
} eNoteLength;

struct tNote {
   eNoteFrequency Note; 	// Note to play
   eNoteLength Value;		// Time of note
};

#define NUM_NOTES 4
tNote* g_nSong = new tNote[NUM_NOTES];

void InitMusicArray() {
	// First note
	g_nSong[0].Note = E_5;
	g_nSong[0].Value = Black;

	// Second note
	g_nSong[1].Note = B_4;
	g_nSong[1].Value = Corchea;

	// ThirdNote
	g_nSong[2].Note = C_5;
	g_nSong[2].Value = Corchea;

	// FourthNote
	g_nSong[3].Note = D_5;
	g_nSong[3].Value = Corchea;

	g_u8NextNoteIndex = 0;
	g_u8NoteCountDown = 0;
	ConfigBuzzer(g_nSong[g_u8NextNoteIndex].Note);
}


void ChangeNote() {
	g_u8NoteCountDown++;

	if(g_u8NoteCountDown >= g_nSong[g_u8NextNoteIndex].Value) {
		g_u8NextNoteIndex += g_u8NextNoteIndex % NUM_NOTES;
		g_u8NoteCountDown = 0;

		ConfigBuzzer(g_nSong[g_u8NextNoteIndex].Note);
	}
}


#endif /* MUSIC_HPP_ */
