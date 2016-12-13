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

uint16_t g_u16NoteCountDown;
uint16_t g_u16NextNoteIndex;

typedef enum {
	C_4 = (1146), // 261.63 Hz
	E_4 = (910),  // 329.63 Hz
	A_4 = (682),  // 440 Hz
	B_4 = (607),  // 493.88 Hz
	C_5 = (573),  // 523.25 Hz
	D_5 = (511),  // 587.33 Hz
	E_5 = (455)   // 659.25 Hz
} eNoteFrequency;

// At Black - 144.
typedef enum {
	Black = 250,
	Corchea = 125,
} eNoteLength;

struct tNote {
   eNoteFrequency Note; 	// Note to play
   eNoteLength Value;		// Time of note
};

#define NUM_NOTES 9
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
	g_nSong[3].Value = Black;

	// FifthNote
	g_nSong[4].Note = C_5;
	g_nSong[4].Value = Corchea;

	// Sixth note
	g_nSong[5].Note = B_4;
	g_nSong[5].Value = Black;

	// 7th
	g_nSong[6].Note = A_4;
	g_nSong[6].Value = Black;

	// 8th
	g_nSong[7].Note = C_5;
	g_nSong[7].Value = Black;

	// 9th
	g_nSong[8].Note = E_5;
	g_nSong[8].Value = Black;

	g_u16NextNoteIndex = 0;
	g_u16NoteCountDown = g_nSong[g_u16NextNoteIndex].Value;
	ConfigBuzzer(g_nSong[g_u16NextNoteIndex].Note);
}


void ChangeNote() {
	g_u16NoteCountDown--;

	if(g_u16NoteCountDown == 0) {
		g_u16NextNoteIndex = (g_u16NextNoteIndex+1) % NUM_NOTES;

		g_u16NoteCountDown = g_nSong[g_u16NextNoteIndex].Value;
		ConfigBuzzer(g_nSong[g_u16NextNoteIndex].Note);
	}
}


#endif /* MUSIC_HPP_ */
