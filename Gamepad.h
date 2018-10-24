/*
	
	A library for using various console gamepads with an arduino.
	
	Copyright (C) 2016  SHeegt

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
	
	History:
	
	Version 1.0: (2016-07-XY) - Initial Release	(S.Heegt)		
*/

#ifndef Gamepad_H
#define Gamepad_H

#include "Arduino.h"
#include "GamepadPinoutMapping.h"
#include "GamepadButtonMapping.h"
#include "_ButtonBuffer.h"

/**********************
	Declare constants
**********************/

const int NOTHING = -255;

typedef struct
{
	byte gamepadLayout = GL_NOT_SET;
} gpConfig;

class Gamepad
{
	public:
		Gamepad();
		/*
			Variables
		*/
		_ButtonBuffer buffer;	

		/* 
			Methods
		*/
		void begin(byte gamepadLayout, byte mcPins[], bool fullPinout=false);
		
		// Methods for pin manipulaton
		void setPin(byte arrayID, byte pin, byte modePin, byte initState=LOW);
		byte getPin(byte arrayID);
		bool readPinState(byte arrayID);
		void writePinState(byte arrayID, byte state);
		void setConnector(byte gamepadLayout, byte mcPins[], bool fullPinout=false);
		byte getGamepadLayout();
		
		// Methods for button state interaction
		void readButtons();
		void writeButtonState(byte buttonBit, byte state, bool converseLogic=false, bool saveInBuffer=true);
		byte getButtonState(byte buttonBit);
		unsigned long getButtonStates();
		
	private:
		/*
			Variables
		*/
		gpConfig _cfg;
		unsigned long _buttonStates; // Up to 32 button states (32 bits) can be stored
		byte _controllerPortPins[15]; 	// A virtually gamepad connector with 15 pins

		/*
			Methods
		*/
		void _setConnectorFullPinout(byte gamepadLayout, byte mcPins[]);	
		void _setConnectorReducedPinout(byte gamepadLayout, byte mcPins[]);	

		/*
			ReadButtons()
		*/
		void _readButtonsGENERIC();
		void _readButtonsNES();
		void _readButtonsSNES();
		void _readButtonsATARI();
		void _readButtonsSEGA_SMS();
		void _readButtonsSEGA_3BTN();
		void _readButtonsSEGA_6BTN();
		void _readButtonsSEGA_SATURN();
	
};


#endif
