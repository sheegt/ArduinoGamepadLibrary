/*
	GamepadButtonMapping.h
*/

#ifndef GamepadButtonMapping_H
#define GamepadButtonMapping_H

/*************************
	Declaration of CONSTs
**************************/

// GAMEPAD LAYOUTS
const byte GL_NOT_SET 			= 0;
const byte GL_GENERIC		 	= 1;
const byte GL_SEGA_SMS_2BTN 	= 2;
const byte GL_SEGA_GENESIS_3BTN = 3;
const byte GL_SEGA_GENESIS_6BTN = 4;
const byte GL_SEGA_SATURN_CP	= 5;
const byte GL_ATARI_2600_1BTN 	= 6;
const byte GL_ATARI_7800_1BTN 	= 7;
const byte GL_ATARI_7800_2BTN 	= 8;
const byte GL_ATARI_2600_KEY 	= 9;
const byte GL_NINTENDO_NES 		= 10;
const byte GL_NINTENDO_SNES		= 11;

/****************************************************************
	Button mapping to the corresponding Bit. Begining with 0	
*****************************************************************/

// Mapping for the NINTENDO NES
const byte NES_BTN_A 			= 0;
const byte NES_BTN_B 			= 1;
const byte NES_BTN_SELECT 		= 2;
const byte NES_BTN_START 		= 3;
const byte NES_BTN_UP 			= 4;
const byte NES_BTN_DOWN 		= 5;
const byte NES_BTN_LEFT 		= 6;
const byte NES_BTN_RIGHT 		= 7;

// Mapping for the NINTENDO SNES; Including the SNES MOUSE
const byte SNES_BTN_B			= 0;
const byte SNES_BTN_Y			= 1;
const byte SNES_BTN_SELECT		= 2;
const byte SNES_BTN_START		= 3;
const byte SNES_BTN_UP			= 4;
const byte SNES_BTN_DOWN		= 5;
const byte SNES_BTN_LEFT		= 6;
const byte SNES_BTN_RIGHT		= 7;
const byte SNES_BTN_A			= 8;
const byte SNES_BTN_X			= 9;
const byte SNES_BTN_L			= 10;
const byte SNES_BTN_R			= 11;
/****************************************************************** 
SNES Clock-Cycle 13 (=12) - 16 (=15) are always HIGH,
except if a mouse is plugged in, then Clock-Cycle 16 (=15) is LOW 
******************************************************************/
const byte SNES_M_BTN_L			= 8;
const byte SNES_M_BTN_R			= 9;
const byte SNES_M_SEN_B1		= 10; // Bit 1 of 2 for the mouse sensitivity level. 3 Levels possible
const byte SNES_M_SEN_B2		= 11; // Bit 2 of 2 for the mouse sensitivity level. 3 Levels possible
const byte SNES_M_FOUND			= 15;	// If this Bit is low a Mouse is plugged in
const byte SNES_M_DIRY			= 16;
const byte SNES_M_DIRX			= 24;

// Mapping for SEGA MASTER SYSTEM & GENESIS (MEGA DRIVE) & Sega Saturn
const byte SEGA_BTN_UP 			= 0;
const byte SEGA_BTN_DOWN 		= 1;
const byte SEGA_BTN_LEFT 		= 2;
const byte SEGA_BTN_RIGHT 		= 3;
const byte SEGA_BTN_A 			= 4;
const byte SEGA_BTN_B 			= 5;
const byte SEGA_BTN_C 			= 6;
const byte SEGA_BTN_START 		= 7;
const byte SEGA_BTN_X		 	= 8;
const byte SEGA_BTN_Y		 	= 9;
const byte SEGA_BTN_Z		 	= 10;
const byte SEGA_BTN_MODE	 	= 11;
const byte SEGA_BTN_R			= 12;
const byte SEGA_BTN_L			= 13;

// Mapping for ATARI 2600 & 7800 1BTN / 2BTN / KEYBOARD 
const byte ATARI_BTN_UP 		= 0;
const byte ATARI_BTN_DOWN 		= 1;
const byte ATARI_BTN_LEFT 		= 2;
const byte ATARI_BTN_RIGHT 		= 3;
const byte ATARI_BTN_1 			= 4;
const byte ATARI_BTN_2 			= 5;
const byte ATARI_KEY_1 			= 6;
const byte ATARI_KEY_2 			= 7;
const byte ATARI_KEY_3 			= 8;
const byte ATARI_KEY_4 			= 9;
const byte ATARI_KEY_5 			= 10;
const byte ATARI_KEY_6 			= 11;
const byte ATARI_KEY_7 			= 12;
const byte ATARI_KEY_8 			= 13;
const byte ATARI_KEY_9 			= 14;
const byte ATARI_KEY_0 			= 15;
const byte ATARI_KEY_A 			= 16;
const byte ATARI_KEY_H 			= 17;

#endif