/*

	A library for using various console gamepads with an arduino.

	Copyright (C) 2016  Sascha Heegt

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
	
*/

#include "Arduino.h"
#include "Gamepad.h"

Gamepad::Gamepad()
{

}

void Gamepad::begin(byte gamepadLayout, byte mcPins[], bool fullPinout)
{
	this->_cfg.gamepadLayout = gamepadLayout;

	// Methods
	this->setConnector(gamepadLayout, mcPins, fullPinout);
	this->buffer.begin();

	return;
}

/*********************
	Methods for PIN
*********************/

void Gamepad::setPin(byte arrayID, byte pin, byte modePin, byte initState)
{
	_controllerPortPins[arrayID] = pin;
	pinMode(pin, modePin);
	if(modePin == OUTPUT && initState == HIGH || modePin == OUTPUT && initState == LOW) { digitalWrite(_controllerPortPins[arrayID], initState);}
}

byte Gamepad::getPin(byte arrayID)
{
	return _controllerPortPins[arrayID];
}

void Gamepad::writePinState(byte arrayID, byte state)
{
	byte pin = getPin(arrayID);	
	digitalWrite(pin, state);
}

bool Gamepad::readPinState(byte arrayID)
{
	byte pin = getPin(arrayID);
	return digitalRead(pin);	
}


/*************************
	Methods for BUTTONS
*************************/

void Gamepad::readButtons()
{
	switch(this->_cfg.gamepadLayout)
	{
		case GL_NINTENDO_NES:
			this->_readButtonsNES();
		break;
		
		case GL_NINTENDO_SNES:
			this->_readButtonsSNES();
		break;

		case GL_ATARI_2600_1BTN:
		case GL_ATARI_7800_1BTN:
		case GL_ATARI_7800_2BTN:
		case GL_ATARI_2600_KEY:
			this->_readButtonsATARI();
		break;	
		
		case GL_SEGA_SMS_2BTN:
			this->_readButtonsSEGA_SMS();
		break;
		
		case GL_SEGA_GENESIS_3BTN:
			this->_readButtonsSEGA_3BTN();
		break;

		case GL_SEGA_GENESIS_6BTN:
			this->_readButtonsSEGA_6BTN();
		break;		
		
		case GL_SEGA_SATURN_CP:
			this->_readButtonsSEGA_SATURN();
		break;	

		default:
			return;
		break;
	}
}

void Gamepad::writeButtonState(byte buttonBit, byte state, bool converseLogic, bool saveInBuffer)
{
	if(converseLogic == true)
	{
		switch(state)
		{
				case HIGH:
						state = LOW;
				break;
				case LOW:
						state = HIGH;
				break;
		}
	}
	
	if(state == HIGH && bitRead(_buttonStates, buttonBit) == LOW && saveInBuffer == true) { buffer.add(buttonBit); }
	bitWrite(_buttonStates, buttonBit, state);
}

byte Gamepad::getButtonState(byte buttonBit)
{
	return bitRead(_buttonStates, buttonBit);
}

unsigned long Gamepad::getButtonStates()
{ 
	return this->_buttonStates; 	
}

void Gamepad::setConnector(byte gamepadLayout, byte mcPins[], bool fullPinout)
{
	this->_cfg.gamepadLayout = gamepadLayout;
	this->_buttonStates = 4294967295;
	/*
	for(byte i=0;i<sizeof(mcPins);i++)
	{
		//this->setPin(i, mcPins[i], OUTPUT, LOW);
	}*/
	
	if(fullPinout == true)
	{
		this->_setConnectorFullPinout(gamepadLayout, mcPins);
	}
	else
	{
		this->_setConnectorReducedPinout(gamepadLayout, mcPins);
	}
	return;
}	

void Gamepad::_setConnectorReducedPinout(byte gamepadLayout, byte mcPins[])	
{
	switch(gamepadLayout)
	{

		// Standard setup for Atari 7800 1 BUTTON-MODE: // For Atari 2600 games or games, which only use "one" button
		case GL_ATARI_2600_1BTN:
		case GL_ATARI_7800_1BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP);		 	// UP
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// DOWN
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);			// LEFT
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP); 		// RIGHT	
			this->setPin(DSUB_P6, mcPins[4], INPUT_PULLUP);			// BOTH BUTTONS (=1 BTN) in 2600 MODE || SET TO HIGH for 7800 MODE (=2 BTN)
		break;
		
		// Standard setup for Atari 7800 2 BUTTON-MODE: // 5V on PIN 6 enables this mode, LEFT + RIGHT = HIGH ACTIVE		
		case GL_ATARI_7800_2BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// UP
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// DOWN
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);			// LEFT
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP); 		// RIGHT	
			this->setPin(DSUB_P6, mcPins[4], OUTPUT, HIGH);			// BOTH BUTTONS (=1 BTN) in 2600 MODE || SET TO HIGH for 7800 MODE (=2 BTN)
			this->setPin(DSUB_P5, mcPins[5], INPUT); 				// BUTTON RIGHT
			this->setPin(DSUB_P9, mcPins[6], INPUT);				// BUTTON LEFT
		break;
		
		// Standard setup for Atari 2600 Keyboard	
		case GL_ATARI_2600_KEY:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// FIRST ROW	 //	(1, 2, 3)
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// SECOND ROW	 //	(4, 5, 6)
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);			// THIRD ROW	 // (7, 8, 9)
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP);			// FOURTH ROW	 // (*, 0, #)	
			this->setPin(DSUB_P5, mcPins[4], OUTPUT, HIGH);			// LEFT COLUMN 	 //	(1, 4, 7, *)
			this->setPin(DSUB_P6, mcPins[5], OUTPUT, HIGH);			// RIGHT COLUMN  // (3, 6, 9, #)
			this->setPin(DSUB_P9, mcPins[6], OUTPUT, HIGH);			// MIDDLE COLUMN // (2, 5, 8, 0)
		break;
		
		// Standard setup for Sega Master System 2 BUTTON MODE, also for a generic 2 Button Controller
		case GL_GENERIC:
		case GL_SEGA_SMS_2BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// UP
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP);		 	// DOWN
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);		 	// LEFT
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP);			// RIGHT
			this->setPin(DSUB_P6, mcPins[4], INPUT_PULLUP); 		// A
			this->setPin(DSUB_P9, mcPins[5], INPUT_PULLUP);			// B
		break;
		
		// Standard setup for Sega GENESIS 3 & 6 BUTTON MODE 
		case GL_SEGA_GENESIS_3BTN:
		case GL_SEGA_GENESIS_6BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// UP | UP or Z
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// DOWN | DOWN or Y
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP); 		// LEFT | LEFT or GND | X or GND
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP);			// RIGHT | RIGHT or GND | MODE or GND	
			this->setPin(DSUB_P6, mcPins[4], INPUT_PULLUP); 		// A | A or B
			this->setPin(DSUB_P7, mcPins[5], OUTPUT, HIGH);			// SELECT			
			this->setPin(DSUB_P9, mcPins[6], INPUT_PULLUP);			// B | C or START
		break;
		
		// Standard setup for Sega SATURN
		case GL_SEGA_SATURN_CP:
			this->setPin(DSUB_P2, mcPins[0], INPUT_PULLUP); 		// D1 (Y|C|DOWN|-)
			this->setPin(DSUB_P3, mcPins[1], INPUT_PULLUP); 		// D0 (Z|B|UP|-)
			this->setPin(DSUB_P4, mcPins[2], OUTPUT, LOW);			// SELECT 0
			this->setPin(DSUB_P5, mcPins[3], OUTPUT, LOW);			// SELECT 1		
			this->setPin(DSUB_P6, mcPins[4], OUTPUT, LOW); 			// +5V (INP / DETECT!??) As long I don't know what this pin does, I set it to LOW (GND)
			this->setPin(DSUB_P7, mcPins[5], INPUT_PULLUP);			// D3 (R|ST|RIGHT|L)		
			this->setPin(DSUB_P8, mcPins[6], INPUT_PULLUP);			// D2 (X|A|LEFT|-)
		break;		

		// Standard setup for NINTENDO NES		
		case GL_NINTENDO_NES:
		case GL_NINTENDO_SNES:
			this->setPin(NES_P2_CLOCK, mcPins[0], OUTPUT, LOW); 	// DATA CLOCK
			this->setPin(NES_P3_LATCH, mcPins[1], OUTPUT, LOW); 	// DATA LATCH
			this->setPin(NES_P4_DATA, mcPins[2], INPUT_PULLUP); 	// SERIAL DATA
		break;
	}	
}

void Gamepad::_setConnectorFullPinout(byte gamepadLayout, byte mcPins[])	
{
	switch(gamepadLayout)
	{
		// Standard setup for Atari 7800 1 BUTTON-MODE: // For Atari 2600 games or games, which only use "one" button
		case GL_ATARI_2600_1BTN:
		case GL_ATARI_7800_1BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP);		 	// UP
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// DOWN
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);			// LEFT
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP); 		// RIGHT
			this->setPin(DSUB_P5, mcPins[4], OUTPUT, LOW); 			// 	
			this->setPin(DSUB_P6, mcPins[5], INPUT_PULLUP);			// BOTH BUTTONS (=1 BTN) in 2600 MODE || SET TO HIGH for 7800 MODE (=2 BTN)
			this->setPin(DSUB_P7, mcPins[6], OUTPUT, LOW); 			// 
			this->setPin(DSUB_P8, mcPins[7], OUTPUT, LOW); 			// GND
			this->setPin(DSUB_P9, mcPins[8], OUTPUT, LOW); 			// 
		break;
		
		// Standard setup for Atari 7800 2 BUTTON-MODE: // 5V on PIN 6 enables this mode, LEFT + RIGHT = HIGH ACTIVE		
		case GL_ATARI_7800_2BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// UP
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// DOWN
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);			// LEFT
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP); 		// RIGHT
			this->setPin(DSUB_P5, mcPins[4], INPUT);	 			// BUTTON RIGHT			
			this->setPin(DSUB_P6, mcPins[5], OUTPUT, HIGH);			// BOTH BUTTONS (=1 BTN) in 2600 MODE || SET TO HIGH for 7800 MODE (=2 BTN)
			this->setPin(DSUB_P7, mcPins[6], OUTPUT, HIGH); 		// +5 VCC
			this->setPin(DSUB_P8, mcPins[7], OUTPUT, LOW); 			// GND
			this->setPin(DSUB_P9, mcPins[8], INPUT);				// BUTTON LEFT
		break;
		
		// Standard setup for Atari 2600 Keyboard	
		case GL_ATARI_2600_KEY:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// FIRST ROW	 //	(1, 2, 3)
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// SECOND ROW	 //	(4, 5, 6)
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);			// THIRD ROW	 // (7, 8, 9)
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP);			// FOURTH ROW	 // (*, 0, #)	
			this->setPin(DSUB_P5, mcPins[4], OUTPUT, HIGH);			// LEFT COLUMN 	 //	(1, 4, 7, *)
			this->setPin(DSUB_P6, mcPins[5], OUTPUT, HIGH);			// RIGHT COLUMN  // (3, 6, 9, #)
			this->setPin(DSUB_P7, mcPins[6], OUTPUT, HIGH);			// +5 VCC pulls up pins 5 and 9 through 4.7k resistors
			this->setPin(DSUB_P8, mcPins[7], OUTPUT, LOW);			// PIN 8 IS NOT USED IN THIS MODE
			this->setPin(DSUB_P9, mcPins[8], OUTPUT, HIGH);			// MIDDLE COLUMN // (2, 5, 8, 0)
		break;
		
		// Standard setup for Sega Master System 2 BUTTON MODE, also for a generic 2 Button Controller
		case GL_GENERIC:
		case GL_SEGA_SMS_2BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// UP
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP);		 	// DOWN
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP);		 	// LEFT
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP);			// RIGHT
			this->setPin(DSUB_P5, mcPins[4], OUTPUT, LOW); 			// PIN 5 IS NOT USED
			this->setPin(DSUB_P6, mcPins[5], INPUT_PULLUP); 		// A
			this->setPin(DSUB_P7, mcPins[6], INPUT_PULLUP); 		// LIGHT SENSOR (LIGHT PHASER ONLY)		
			this->setPin(DSUB_P8, mcPins[7], OUTPUT, LOW);			// GND
			this->setPin(DSUB_P9, mcPins[8], INPUT_PULLUP);			// B
		break;
		
		// Standard setup for Sega GENESIS 3 & 6 BUTTON MODE 
		case GL_SEGA_GENESIS_3BTN:
		case GL_SEGA_GENESIS_6BTN:
			this->setPin(DSUB_P1, mcPins[0], INPUT_PULLUP); 		// UP | UP or Z
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// DOWN | DOWN or Y
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP); 		// LEFT | LEFT or GND | X or GND
			this->setPin(DSUB_P4, mcPins[3], INPUT_PULLUP);			// RIGHT | RIGHT or GND | MODE or GND	
			this->setPin(DSUB_P5, mcPins[4], OUTPUT, HIGH);			// +5 VCC			
			this->setPin(DSUB_P6, mcPins[5], INPUT_PULLUP); 		// A | A or B
			this->setPin(DSUB_P7, mcPins[6], OUTPUT, HIGH);			// SELECT		
			this->setPin(DSUB_P8, mcPins[7], OUTPUT, LOW);			// GND
			this->setPin(DSUB_P9, mcPins[8], INPUT_PULLUP);			// B | C or START
		break;
		
		// Standard setup for Sega SATURN
		case GL_SEGA_SATURN_CP:
			this->setPin(DSUB_P1, mcPins[0], OUTPUT, HIGH); 		// +5V VCC
			this->setPin(DSUB_P2, mcPins[1], INPUT_PULLUP); 		// D1 (Y|C|DOWN|-)
			this->setPin(DSUB_P3, mcPins[2], INPUT_PULLUP); 		// D0 (Z|B|UP|-)
			this->setPin(DSUB_P4, mcPins[3], OUTPUT, LOW);			// SELECT 0
			this->setPin(DSUB_P5, mcPins[4], OUTPUT, LOW);			// SELECT 1		
			this->setPin(DSUB_P6, mcPins[5], OUTPUT, LOW); 			// +5V (INP / DETECT!??) As long I don't know what this pin does, I set it to LOW (GND)
			this->setPin(DSUB_P7, mcPins[6], INPUT_PULLUP);			// D3 (R|ST|RIGHT|L)		
			this->setPin(DSUB_P8, mcPins[7], INPUT_PULLUP);			// D2 (X|A|LEFT|-)
			this->setPin(DSUB_P9, mcPins[8], OUTPUT, LOW);			// GND
		break;		
		
		// Standard setup for NINTENDO (S)NES		
		case GL_NINTENDO_NES:
		case GL_NINTENDO_SNES:
			this->setPin(NES_P1_5V, mcPins[0], OUTPUT, HIGH);		// +5 VCC	
			this->setPin(NES_P2_CLOCK, mcPins[1], OUTPUT, LOW); 	// DATA CLOCK
			this->setPin(NES_P3_LATCH, mcPins[2], OUTPUT, LOW); 	// DATA LATCH
			this->setPin(NES_P4_DATA, mcPins[3], INPUT_PULLUP); 	// SERIAL DATA
			this->setPin(NES_P5_NC, mcPins[4], OUTPUT, LOW); 		// PIN 5 IS NOT USED
			this->setPin(NES_P6_NC, mcPins[5], OUTPUT, LOW); 		// PIN 6 IS NOT USED
			this->setPin(NES_P7_GND, mcPins[6], OUTPUT, LOW); 		// GND
			
		break;
	}	
}

byte Gamepad::getGamepadLayout()
{
	return this->_cfg.gamepadLayout;
}

/*************************************************************************************************
	
	Methods for button / input reading
	
*************************************************************************************************/

/***********************************************
			GENERIC
***********************************************/

void Gamepad::_readButtonsGENERIC()
{
	//Generic 2 Action-Button + 4 Direction-Button Controller
	
	this->writeButtonState(0, this->readPinState(DSUB_P1));
	this->writeButtonState(1, this->readPinState(DSUB_P2));
	this->writeButtonState(2, this->readPinState(DSUB_P3));
	this->writeButtonState(3, this->readPinState(DSUB_P4));
	this->writeButtonState(4, this->readPinState(DSUB_P6));
	this->writeButtonState(5, this->readPinState(DSUB_P9));
}

/***********************************************
			NINTENDO
***********************************************/

void Gamepad::_readButtonsNES()
{
	byte clockTime = 12;

	/****************************************************************************************************
		Get the state of the 8 buttons. Button A is already available after we send a HIGH->LOW cycle on 
		the latch pin. For the rest of the 7 buttons we must send 7 HIGH->LOW cycles on the clock pin and 
		grab the state of the corresponding buttons on the serial data pin.

		
		|CYLCE	| 	BUTTON	|
		---------------------
		|	1	|	A		|
		|	2	|	B		|		
		|	3	|	SELECT	|
		|	4	|	START	|
		|	5	|	UP		|
		|	6	|	DOWN	|
		|	7	|	LEFT	|
		|	8	|	RIGHT	|
		---------------------
		
		See GamepadButtonMapping.h for some constants, which make it easier to identify the buttons.
		
		CONNECTOR_PIN 2 == CLOCK PIN
		CONNECTOR_PIN 3 == LATCH PIN
		CONNECTOR_PIN 4 == DATA 
	****************************************************************************************************/
	
	// Init the data grabbing
	this->writePinState(NES_P3_LATCH, HIGH);
	this->writePinState(NES_P3_LATCH, LOW);	
	delayMicroseconds(clockTime);

	// Button state for A
	this->writeButtonState(0, this->readPinState(NES_P4_DATA)); 
	
	// Send 2-8 Cycles (HIGH>LOW) on the clock pin and grab the corresponding button states. 
	for(byte x=1;x<8;x++)
	{
		delayMicroseconds(clockTime);
		this->writePinState(NES_P2_CLOCK, HIGH);
		this->writePinState(NES_P2_CLOCK, LOW);
		this->writeButtonState(x, this->readPinState(NES_P4_DATA));		
	}
}

void Gamepad::_readButtonsSNES()
{
	byte clockTime = 12;

	/*********************************************************************************************************************************************************
		Get the state of the 12 buttons. Button B is already available after we send a HIGH->LOW cycle on the latch pin. For the rest of the 11 buttons we 
		must send 11 HIGH->LOW cycle on the clock pin and grab the state of the corresponding button on the serial data pin.
		
		!!Note 1: A complete response cycle consists of 16 bits, from which are the last 4 bits always HIGH. For a exception see the next note.
		!!Note 2: If a mouse is plugged in, Bit 16 is LOW instead of HIGH. In this case some changes occur and 16 new Bits will be submitted from the mouse.
		
		-------------------------------------------------------------------------
		|CYLCE	| 	SNES CONTROLLER		|	SNES MOUSE							|
		-------------------------------------------------------------------------
		|	1	|	B					|	NONE (always HIGH)					|	
		|	2	|	Y					|	NONE (always HIGH)					|
		|	3	|	SELECT				|	NONE (always HIGH)					|
		|	4	|	START				|	NONE (always HIGH)					|
		|	5	|	UP					|	NONE (always HIGH)					|
		|	6	|	DOWN				|	NONE (always HIGH)					|
		|	7	|	LEFT				|	NONE (always HIGH)					|
		|	8	|	RIGHT				|	NONE (always HIGH)					|
		|	9	|	A					|	LEFT MOUSE BUTTON					|
		|	10	|	X					|	RIGHT MOUSE BUTTON					|
		|	11	|	L					|	Bit 1 MOUSE SENSETIVE LEVEL			|
		|	12	|	R					|	Bit 2 MOUSE SENSETIVE LEVEL			|
		|	13	|	NONE (always HIGH)	|	NONE (always HIGH)					|
		|	14	|	NONE (always HIGH)	|	NONE (always HIGH)					|
		|	15	|	NONE (always HIGH)	|	NONE (always HIGH)					|	
		|	16	|	NONE (always HIGH)	|	ALWAYS LOW							|		
		|	17	|						|	Y direction (LOW=UP, HIGH=DOWN)		|
		|	18	|						|	Y motion bit 6						|
		|	19	|						|	Y motion bit 5						|
		|	20	|						|	Y motion bit 4						|
		|	21	|						|	Y motion bit 3						|
		|	22	|						|	Y motion bit 2						|
		|	23	|						|	Y motion bit 1						|
		|	24	|						|	Y motion bit 0						|
		|	25	|						|	X direction (LOW=LEFT, HIGH=RIGHT)	|
		|	26	|						|	X motion bit 6						|
		|	27	|						|	X motion bit 5						|
		|	28	|						|	X motion bit 4						|
		|	29	|						|	X motion bit 3						|
		|	30	|						|	X motion bit 2						|
		|	31	|						|	X motion bit 1						|
		|	32	|						|	X motion bit 0						|
		-------------------------------------------------------------------------
		
		Sensetive levels:
		- Bit 11 LOW, Bit 12 HIGH: High sensitivity
		- Bit 11 HIGH, Bit 12 LOW: Medium sensitivity
		- Bit 11 HIGH, Bit 12 HIGH: Low sensitivity 
		
		See GamepadButtonMapping.h for some constants, which make it easier to identify the buttons.
		
		CONNECTOR_PIN 2 == CLOCK PIN
		CONNECTOR_Pin 3 == LATCH PIN
		CONNECTOR_PIN 4 == DATA PIN
		
	*********************************************************************************************************************************************************/
	
	// Init the data grabbing
	this->writePinState(NES_P3_LATCH, HIGH);
	this->writePinState(NES_P3_LATCH, LOW);	
	delayMicroseconds(clockTime);
	
	// State of button B
	this->writeButtonState(0, this->readPinState(NES_P4_DATA)); 
	
	// Send 2-16 Cycles (HIGH>LOW) on the clock pin and grab the corresponding button states. 
	for(byte x=1;x<16;x++)
	{
		this->writePinState(NES_P2_CLOCK, HIGH);
		this->writePinState(NES_P2_CLOCK, LOW);
		this->writeButtonState(x, this->readPinState(NES_P4_DATA));
	}

	// Check for Mouse
	if(this->getButtonState(SNES_M_FOUND) == LOW) 
	{
		// Cycle 17# Mouse Y-Direction LOW = UP / HIGH = DOWN
		this->writePinState(NES_P2_CLOCK, HIGH);
		this->writePinState(NES_P2_CLOCK, LOW);
		this->writeButtonState(SNES_M_DIRY, this->readPinState(NES_P4_DATA));
		
		// Cycle 18-24 Y motion bit 6 to Y motion bit 0
		for(int y=17;y<24;y++)
		{
			this->writePinState(NES_P2_CLOCK, HIGH);
			this->writePinState(NES_P2_CLOCK, LOW);
			this->writeButtonState(y, this->readPinState(NES_P4_DATA));			
		}
		
		// Cycle 25# Mouse Z-Direction LOW = LEFT / HIGH = RIGHT
		this->writePinState(NES_P2_CLOCK, HIGH);
		this->writePinState(NES_P2_CLOCK, LOW);
		this->writeButtonState(SNES_M_DIRX, this->readPinState(NES_P4_DATA));		
		
		// Z motion bit 6 to Z motion bit 0
		for(int z=25;z<32;z++)
		{
			this->writePinState(NES_P2_CLOCK, HIGH);
			this->writePinState(NES_P2_CLOCK, LOW);
			this->writeButtonState(z, this->readPinState(NES_P4_DATA));			
		}		
	}
	
	/***********************************************************************
		The bits 13-15 are used for checking if a controller is plugged in, 
		because normally they are HIGH, if one is plugged in.
	************************************************************************/
	/*
	if(this->getButtonState(12) == LOW && this->getButtonState(13) == LOW && this->getButtonState(14) == LOW)
	{
		this->setControllerState(CS_NOT_PLUGGED);
	}
	else
	{
		this->setControllerState(CS_PLUGGED);
	}*/
}

/***********************************************
			ATARI
***********************************************/

void Gamepad::_readButtonsATARI()
{
	//if(_cfg.gamepadLayout == GL_NOT_SET && _cfg.readMode == GL_NOT_SET) { return 0; }

	byte clockTime = 20;
	
	/********************************************************************************************************************
		When using an Atari 7800 Controller in 2 button mode, button  1 & 2 are normally positive logic, but to 
		create a common pattern, the parameter forceNegativeLogic from Gamepad::writeButtonState() is set to true.
		So in case of those two buttons writeButtonState will converse the states, so a HIGH will be written as LOW and a LOW as HIGH.
	*********************************************************************************************************************/
	
	//delayMicroseconds(clockTime);
	
	switch(_cfg.gamepadLayout)
	{
		case GL_ATARI_2600_1BTN:
		case GL_ATARI_7800_1BTN:
			writeButtonState(ATARI_BTN_UP, this->readPinState(DSUB_P1));
			writeButtonState(ATARI_BTN_DOWN, this->readPinState(DSUB_P2));
			writeButtonState(ATARI_BTN_LEFT, this->readPinState(DSUB_P3));
			writeButtonState(ATARI_BTN_RIGHT, this->readPinState(DSUB_P4));
			writeButtonState(ATARI_BTN_1, this->readPinState(DSUB_P6));
		break;

		case GL_ATARI_7800_2BTN:
			writeButtonState(ATARI_BTN_UP, this->readPinState(DSUB_P1));
			writeButtonState(ATARI_BTN_DOWN, this->readPinState(DSUB_P2));
			writeButtonState(ATARI_BTN_LEFT, this->readPinState(DSUB_P3));
			writeButtonState(ATARI_BTN_RIGHT, this->readPinState(DSUB_P4));
			writeButtonState(ATARI_BTN_1, this->readPinState(DSUB_P5), true);	
			writeButtonState(ATARI_BTN_2, this->readPinState(DSUB_P9), true);						
		break;

		case GL_ATARI_2600_KEY:
			//LEFT COLUMN		//	(1, 4, 7, *)
			this->writePinState(DSUB_P5, LOW);	
			delayMicroseconds(clockTime);
			writeButtonState(ATARI_KEY_1, this->readPinState(DSUB_P1));	// 1
			writeButtonState(ATARI_KEY_4, this->readPinState(DSUB_P2));	// 4
			writeButtonState(ATARI_KEY_7, this->readPinState(DSUB_P3));	// 7
			writeButtonState(ATARI_KEY_A, this->readPinState(DSUB_P4));	// *		
			this->writePinState(DSUB_P5, HIGH);

			//MIDDLE COLUMN		//	(2, 5, 8, 0)
			this->writePinState(DSUB_P9, LOW);
			delayMicroseconds(clockTime);			
			writeButtonState(ATARI_KEY_2, this->readPinState(DSUB_P1));	// 2
			writeButtonState(ATARI_KEY_5, this->readPinState(DSUB_P2));	// 5
			writeButtonState(ATARI_KEY_8, this->readPinState(DSUB_P3));	// 8
			writeButtonState(ATARI_KEY_0, this->readPinState(DSUB_P4));	// 0	
			this->writePinState(DSUB_P9, HIGH);
			
			//RIGHT COLUMN		// (3, 6, 9, #)
			this->writePinState(DSUB_P6, LOW);
			delayMicroseconds(clockTime);			
			writeButtonState(ATARI_KEY_3, this->readPinState(DSUB_P1));	// 3
			writeButtonState(ATARI_KEY_6, this->readPinState(DSUB_P2));	// 6
			writeButtonState(ATARI_KEY_9, this->readPinState(DSUB_P3));	// 9
			writeButtonState(ATARI_KEY_H, this->readPinState(DSUB_P4));	// #		
			this->writePinState(DSUB_P6, HIGH);	
		break;		
	}
}

/***********************************************
			SEGA
***********************************************/

void Gamepad::_readButtonsSEGA_SMS()
{
	/********************************************************************************
		Get the state of the 6 buttons. Just check, if the corresponding pin is LOW.
		
		-----------------------------
		|Pin	|	SMS CONTROLLER	|
		-----------------------------
		|1		|	UP				|
		|2		|	DOWN			|
		|3		|	LEFT			|
		|4		|	RIGHT			|
		|6		|	BUTTON A		|
		|9		|	BUTTON B		|
		-----------------------------
	
	********************************************************************************/
	
	delayMicroseconds(20);

	this->writeButtonState(SEGA_BTN_UP, this->readPinState(DSUB_P1));
	this->writeButtonState(SEGA_BTN_DOWN, this->readPinState(DSUB_P2));
	this->writeButtonState(SEGA_BTN_LEFT, this->readPinState(DSUB_P3));
	this->writeButtonState(SEGA_BTN_RIGHT, this->readPinState(DSUB_P4));
	this->writeButtonState(SEGA_BTN_A, this->readPinState(DSUB_P6));
	this->writeButtonState(SEGA_BTN_B, this->readPinState(DSUB_P9));
}

void Gamepad::_readButtonsSEGA_3BTN()
{
	byte clockTime = 20;

	/********************************************************************************
		Get the state of the 8 buttons. 
		
		-----------------------------------------------------------------
		|PIN	|	SELECT PIN (7) = LOW	|	SELECT PIN (7) = HIGH	|
		-----------------------------------------------------------------
		|1		|	UP						|	UP						|
		|2		|	DOWN					|	DOWN					|
		|3		|							|	LEFT					|
		|4		|							|	RIGHT					|
		|6		|	BUTTON A				|	BUTTON B				|
		|7		|	(SELECT)				|	(SELECT)				|
		|9		|	START					|	BUTTON C				|	
		-----------------------------------------------------------------
	
	********************************************************************************/		
		
	// Cycle LOW to HIGH	
	this->writePinState(DSUB_P7, LOW);
	delayMicroseconds(clockTime);
	
	this->writeButtonState(SEGA_BTN_A, this->readPinState(DSUB_P6));
	this->writeButtonState(SEGA_BTN_START, this->readPinState(DSUB_P9));

	delayMicroseconds(clockTime);	
	this->writePinState(DSUB_P7, HIGH);
	delayMicroseconds(clockTime);
	
	this->writeButtonState(SEGA_BTN_UP, this->readPinState(DSUB_P1));
	this->writeButtonState(SEGA_BTN_DOWN, this->readPinState(DSUB_P2));
	this->writeButtonState(SEGA_BTN_LEFT, this->readPinState(DSUB_P3));	
	this->writeButtonState(SEGA_BTN_RIGHT, this->readPinState(DSUB_P4));	
	this->writeButtonState(SEGA_BTN_B, this->readPinState(DSUB_P6));	
	this->writeButtonState(SEGA_BTN_C, this->readPinState(DSUB_P9));	
}

void Gamepad::_readButtonsSEGA_6BTN()
{
	byte clockTime = 20; 

	/*************************************************************************************************************************************************************************************
		Get the state of the 12 buttons. 
		
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		|		|						FIRST CYCLE						|						SECOND CYCLE					|						THIRD CYCLE						|
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		|PIN	|	SELECT PIN (7) = LOW	|	SELECT PIN (7) = HIGH	|	SELECT PIN (7) = LOW	|	SELECT PIN (7) = HIGH	|	SELECT PIN (7) = LOW	|	SELECT PIN (7) = HIGH	|
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		|1		|	UP						|	UP						|	UP						|	UP						|	LOW						|	Z						|
		|2		|	DOWN					|	DOWN					|	DOWN					|	DOWN					|	LOW						|	Y						|
		|3		|							|	LEFT					|							|	LEFT					|	LOW						|	X						|
		|4		|							|	RIGHT					|							|	RIGHT					|	LOW						|	MODE					|
		|6		|	BUTTON A				|	BUTTON B				|	BUTTON A				|	BUTTON B				|	BUTTON A				|	HIGH					|
		|7		|	(SELECT)				|	(SELECT)				|	(SELECT)				|	(SELECT)				|	(SELECT)				|	(SELECT)				|
		|9		|	START					|	BUTTON C				|	START					|	BUTTON C				|	START					|	HIGH					|
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	**************************************************************************************************************************************************************************************/		
	
	// First Cycle LOW TO HIGH	
	this->writePinState(DSUB_P7, LOW);
	this->writeButtonState(SEGA_BTN_A, this->readPinState(DSUB_P6));
	this->writeButtonState(SEGA_BTN_START, this->readPinState(DSUB_P9));	
	delayMicroseconds(clockTime);
	
	this->writePinState(DSUB_P7, HIGH);
	this->writeButtonState(SEGA_BTN_UP, this->readPinState(DSUB_P1));
	this->writeButtonState(SEGA_BTN_DOWN, this->readPinState(DSUB_P2));
	this->writeButtonState(SEGA_BTN_LEFT, this->readPinState(DSUB_P3));	
	this->writeButtonState(SEGA_BTN_RIGHT, this->readPinState(DSUB_P4));	
	this->writeButtonState(SEGA_BTN_B, this->readPinState(DSUB_P6));	
	this->writeButtonState(SEGA_BTN_C, this->readPinState(DSUB_P9));		
	delayMicroseconds(clockTime);	
	
	// Second Cycle LOW TO HIGH
	this->writePinState(DSUB_P7, LOW);
	this->writePinState(DSUB_P7, HIGH);
	delayMicroseconds(clockTime);	
	
	// Third Cycle LOW TO HIGH
	this->writePinState(DSUB_P7, LOW);
	delayMicroseconds(clockTime);
	
	// Check, if all direction buttons are low, to determine, if a six button controller is plugged in
	if(this->readPinState(DSUB_P1) == LOW && this->readPinState(DSUB_P2) == LOW && this->readPinState(DSUB_P3) == LOW && this->readPinState(DSUB_P4) == LOW)
	{
		this->writePinState(DSUB_P7, HIGH);
		this->writeButtonState(SEGA_BTN_Z, this->readPinState(DSUB_P1));
		this->writeButtonState(SEGA_BTN_Y, this->readPinState(DSUB_P2));
		this->writeButtonState(SEGA_BTN_X, this->readPinState(DSUB_P3));	
		this->writeButtonState(SEGA_BTN_MODE, this->readPinState(DSUB_P4));	
		
		// Fourth Cycle LOW TO HIGH / Reset
		this->writePinState(DSUB_P7, LOW);
		this->writePinState(DSUB_P7, HIGH);
	}
	//delayMicroseconds(1000);
}

void Gamepad::_readButtonsSEGA_SATURN()
{
	
	/********************************************************************************
		Get the state of the 13 buttons. 

		-----------------------------
		|Pin	|	Control Pad		|
		-----------------------------
		|1		|	+5V VCC			|
		|2		|	DATA 1 (D1)		|
		|3		|	DATA 0 (D0)		|
		|4		|	SELECT 0 (S0)	|
		|5		|	SELECT 0 (S0)	|
		|6		|	+5V (Detect!?)	|
		|7		|	DATA 3 (D3)		|
		|8		|	DATA 2 (D2)		|
		|9		|	GND				|
		-----------------------------	

		OFF = LOW
		ON = HIGH
				
		-------------------------------------------------
		|S0		|	S1	|	D0	|	D1	|	D2	|	D3	|
		-------------------------------------------------
		|OFF	|	OFF	|	Z	|	Y	|	X	|	R	|
		|ON		|	OFF	|	B	|	C	|	A	|	ST	|
		|OFF	|	ON	|	UP	|	DN	|	LT	|	RT	|
		|ON		|	ON	|	-	|	-	|	-	|	L	|
		-------------------------------------------------
	
	********************************************************************************/	
	
	// S0 = OFF & S1 = OFF
	this->writePinState(DSUB_P4, LOW);
	this->writePinState(DSUB_P5, LOW);
	
	this->writeButtonState(SEGA_BTN_Z, this->readPinState(DSUB_P3));
	this->writeButtonState(SEGA_BTN_Y, this->readPinState(DSUB_P2));
	this->writeButtonState(SEGA_BTN_X, this->readPinState(DSUB_P8));
	this->writeButtonState(SEGA_BTN_R, this->readPinState(DSUB_P7));
	
	// S0 = ON & S1 = OFF
	this->writePinState(DSUB_P4, HIGH);
	this->writePinState(DSUB_P5, LOW);
	
	this->writeButtonState(SEGA_BTN_B, this->readPinState(DSUB_P3));
	this->writeButtonState(SEGA_BTN_C, this->readPinState(DSUB_P2));
	this->writeButtonState(SEGA_BTN_A, this->readPinState(DSUB_P8));
	this->writeButtonState(SEGA_BTN_START, this->readPinState(DSUB_P7));	
	
	// S0 = OFF & S1 = ON
	this->writePinState(DSUB_P4, LOW);
	this->writePinState(DSUB_P5, HIGH);
	
	this->writeButtonState(SEGA_BTN_UP, this->readPinState(DSUB_P3));
	this->writeButtonState(SEGA_BTN_DOWN, this->readPinState(DSUB_P2));
	this->writeButtonState(SEGA_BTN_LEFT, this->readPinState(DSUB_P8));
	this->writeButtonState(SEGA_BTN_RIGHT, this->readPinState(DSUB_P7));
	
	// S0 = ON & S1 = ON
	this->writePinState(DSUB_P4, HIGH);
	this->writePinState(DSUB_P5, HIGH);
	
	this->writeButtonState(SEGA_BTN_L, Gamepad::readPinState(DSUB_P7));			
}

/*
void Gamepad::setMouseSensitivity()
{
	
}

byte Gamepad::getMouseSensitivity()
{
	/*
		- Bit 11 LOW, Bit 12 HIGH: High sensitivity
		- Bit 11 HIGH, Bit 12 LOW: Medium sensitivity
		- Bit 11 HIGH, Bit 12 HIGH: Low sensitivity 
	*/	
/*	if(Gamepad::getButtonState(SNES_M_SEN_B1) == HIGH && Gamepad::getButtonState(SNES_M_SEN_B2) == HIGH) { return 1; }
	if(Gamepad::getButtonState(SNES_M_SEN_B1) == HIGH && Gamepad::getButtonState(SNES_M_SEN_B2) == LOW) { return 2; }
	if(Gamepad::getButtonState(SNES_M_SEN_B1) == LOW && Gamepad::getButtonState(SNES_M_SEN_B2) == HIGH) { return 3; }
	
	// Return 0 if nothing from the above applies
	return 0;
}

bool Gamepad::mouseConnected()
{
	//Before you can use this method please run readButtons() at least once.
	
	bool mouse;
	
	mouse = (Gamepad::getButtonState(SNES_M_FOUND) == LOW) ? true : false;
	
	return mouse;
}*/
