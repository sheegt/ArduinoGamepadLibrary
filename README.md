# Introduction #

**Version: 1.0**

This library is used for reading various gamepads from older videogame systems with an Arduino. See the examples folder for some samples. More gamepads will follow.  

**Tested with:**

- Arduino IDE V. 1.6.9
- Arduino Uno
- Arduino Micro  

Might work with Atmega-chips. Testing needed. 

# Installation #

Copy the folder 'Gamepad' to the Arduino libraries folder (for example: %userprofile%\Documents\Arduino\libraries ). The library should now appear in the Arduino IDE list of libraries.

### Notes ###

You can use the library in two pinout modes.

#### 1. Full Pinout ####

This means, each pin of the corresponding connector plug is connected to an Arduino pin. VCC and GND is set via DigitalWrite.  

**Advantage:** It's easier to use various GamepadLayouts, without the need for re-pluging the pins.

**Disadvantage:** More pins are needed. 

#### 2. Reduced Pinout ####

This means, only the pins, who are actually needed for reading the buttons are used. VCC and GND must come from an external source, like the VCC/GNDs pins supplied by the Arduino.   

**Advantage:** Lesser pins are used. 

**Disadvantage:** If you want to use gamepads, which might have the same plug, but use a different pinout, you need to rearrange the connections to your Arduino pins.

You can switch between these two modes by using the fullPinout parameter.

**It's not recommend to have more than one Gamepad connected to the same set of Arduino pins. This can cause a distortion in the button reading.**

# Supported Gamepads / Systems #

**ATARI**

- 7800 1 button Mode
- 7800 2 button Mode
- 2600 Keyboard Controller

**NINTENDO**

- NES Gamepad
- SNES Gamepad
- SNES Mouse

**SEGA**

- Master System Control Pad
- Genesis 3 button Controller
- Genesis 6 button Controller
- Saturn Control Pad

**MISC**

- Generic 2 button gampad

# Gamepad Library API #

#### 1. Primary Methods ####
#### 2. Background Methods ####
#### 3. Button Buffer Methods ####
#### 4. Constants Reference list ####

## 1. Primary Methods ##

**void Gamepad.begin(byte gamepadLayout, byte mcPins[], __bool fullPinout=false__)**

Setup the basic variables and call **Gamepad.setConnector()** with the given parameters.

**void Gamepad.setConnector(byte gamepadLayout, byte mcPins[], __bool fullPinout=false__)**

Configures the virtual gameport connector. By default the connector only uses the pins, which are necessary to read the gamepad. Unused pins on the connector will be ignored and for VCC / GND no digital pins will be used, therefore if the gamepad need a power supply use an external source or the designated VCC / GND pins from the Arduino.
If you use fullPinout=true all pins from the connector / gamepad must be connected to the arduino, but in this case it's easier to switch the gamepadLayout during runtime.

**byte Gamepad.getGamepadLayout()**

Returns the active GamepadLayout.

**void Gamepad.readButtons()**

Read the button states of the connected gamepad, which is specifed by the gamepadLayout. After the reading you can get either the state (HIGH/LOW) of a single button via **Gamepad.getButtonState()** or the state of all buttons via **Gamepad.getButtonStates()** as an unsigned long variable. The bit for every button, which is pressed, is set to lo.w In the variable.

**byte Gamepad.getButtonState(byte btnBit)**

Returns the state (HIGH/LOW) of a button. See the “**Constants Reference list**” for the bit names.

**unsigned long Gamepad.getButtonStates()**

Returns the state of all buttons as an unsigned long variable. The bit for every button, which is pressed, is set to low In the variable.

## 2. Background Methods ##

These methods are used by the aforementioned methods and are normally not needed if you just want to read a gamepad. 

**void Gamepad.setPin(byte arrayID, byte pin, byte modePin, __byte initState=false__)**

Assign an arduino pin to a pin from the virtual connector port.

**byte Gamepad.getPin(byte arrayID)**

Returns the digitial pin from the virtual cennector port array. 

**void Gamepad.writePinState(byte arrayID, byte state)**

A wrapper for **digitalWrite()**.

**bool Gamepad.readPinState(byte arrayID)**

A wrapper for **digitalRead()**.

**void Gamepad.writeButtonState(byte btnBit, byte state, __bool converseLogic=false__, __bool saveInBuffer=true__)**

Sets the state of a button, which is a bit inside of a unsigned long variable. Such variable has 32 bits and therefore can hold 32 button states. Normally most gamepads work with active-low buttons. If you press a button a circuit will be closed and the video game system detects that a button was pressed. The Library uses the same schematic for storing a button states, but if a gamepad uses buttons with is active-high you can set *converseLogic=true* so **Gampead.writeButtonState()** will reverse the state, before it will be stored. With saveInBuffer you can decide if the state will be added to the internal button buffer.See the “**Constants Reference list**” for the bit names.

## 3. Button Buffer Methods ##

**void Gamepad.buffer.begin()**

Initialize the button buffer. Will be called automatically. 

**bool Gamepad.buffer.available()**

Returns true if the buffer is not empty, otherwise false.

**byte Gamepad.buffer.count()**

Returns the actual buffer count.

**void Gamepad.buffer.add(byte buttonBit)**

Adds a button bit to the buffer.

**byte Gamepad.buffer.next()**

Returns the next button bit from the buffer and delete it afterwards. See the “**Constants Reference list**” for the bit names.

**void Gamepad.buffer.flush()**

Clears the buffer.

## 4. Reference List Constants ##

This constants can be used in your sketches. 

**GamepadPinoutMapping.h**

**PIN Names for Array ID**
    
    byte DSUB_P1 				= 0
    byte DSUB_P2  				= 1
    byte DSUB_P3  				= 2
    byte DSUB_P4  				= 3
    byte DSUB_P5  				= 4
    byte DSUB_P6  				= 5
    byte DSUB_P7  				= 6
    byte DSUB_P8  				= 7
    byte DSUB_P9  				= 8
    byte DSUB_P10				= 9
    byte DSUB_P11 				= 10
    byte DSUB_P12 				= 11
    byte DSUB_P13 				= 12
    byte DSUB_P14 				= 13
    byte DSUB_P15 				= 14
    
    byte NES_P1_5V   			= 0
    byte NES_P2_CLOCK			= 1
    byte NES_P3_LATCH			= 2
    byte NES_P4_DATA 			= 3
    byte NES_P5_NC				= 4
    byte NES_P6_NC				= 5
    byte NES_P7_GND  	 		= 6
    
**GamepadButtonMapping.h**
    
**GAMEPAD LAYOUTS**
    
    byte GL_NOT_SET 			= 0
    byte GL_GENERIC				= 1
    byte GL_SEGA_SMS_2BTN 		= 2
    byte GL_SEGA_GENESIS_3BTN	= 3
    byte GL_SEGA_GENESIS_6BTN 	= 4
    byte GL_SEGA_SATURN_CP		= 5
    byte GL_ATARI_2600_1BTN 	= 6	
    byte GL_ATARI_7800_1BTN 	= 7
    byte GL_ATARI_7800_2BTN 	= 8
    byte GL_ATARI_2600_KEY 		= 9
    byte GL_NINTENDO_NES 		= 10
    byte GL_NINTENDO_SNES		= 11
    
**Button mapping to the corresponding Bit. Begining with 0**	
    
**Mapping for the NINTENDO NES**
    
    byte NES_BTN_A 				= 0
    byte NES_BTN_B 				= 1
    byte NES_BTN_SELECT 		= 2
    byte NES_BTN_START 			= 3
    byte NES_BTN_UP 			= 4
    byte NES_BTN_DOWN 			= 5
    byte NES_BTN_LEFT 			= 6
    byte NES_BTN_RIGHT 			= 7
    
**Mapping for the NINTENDO SNES Including the SNES MOUSE**
    
    byte SNES_BTN_B				= 0
    byte SNES_BTN_Y				= 1
    byte SNES_BTN_SELECT		= 2
    byte SNES_BTN_START			= 3
    byte SNES_BTN_UP			= 4
    byte SNES_BTN_DOWN			= 5
    byte SNES_BTN_LEFT			= 6
    byte SNES_BTN_RIGHT			= 7
    byte SNES_BTN_A				= 8
    byte SNES_BTN_X				= 9
    byte SNES_BTN_L				= 10
    byte SNES_BTN_R				= 11
    byte SNES_M_BTN_L			= 8
    byte SNES_M_BTN_R			= 9
    byte SNES_M_SEN_B1			= 10
    byte SNES_M_SEN_B2			= 11
    byte SNES_M_FOUND			= 15	If this Bit is low, a Mouse is plugged in
    byte SNES_M_DIRY			= 16
    byte SNES_M_DIRX			= 24
    
**Mapping for SEGA MASTER SYSTEM & GENESIS (MEGA DRIVE) & SEGA SATURN**

    byte SEGA_BTN_UP 			= 0
    byte SEGA_BTN_DOWN 			= 1
    byte SEGA_BTN_LEFT 			= 2
    byte SEGA_BTN_RIGHT 		= 3
    byte SEGA_BTN_A 			= 4
    byte SEGA_BTN_B 			= 5
    byte SEGA_BTN_C 			= 6
    byte SEGA_BTN_START 		= 7
    byte SEGA_BTN_X		 		= 8
    byte SEGA_BTN_Y		 		= 9
    byte SEGA_BTN_Z		 		= 10
    byte SEGA_BTN_MODE	 		= 11
    byte SEGA_BTN_R				= 12
    byte SEGA_BTN_L				= 13
    
**Mapping for ATARI 2600 & 7800 1BTN Mode & 2BTN Mode & KEYBOARD**

    byte ATARI_BTN_UP 			= 0
    byte ATARI_BTN_DOWN 		= 1
    byte ATARI_BTN_LEFT 		= 2
    byte ATARI_BTN_RIGHT 		= 3    
    byte ATARI_BTN_1 			= 4    
    byte ATARI_BTN_2 			= 5    
    byte ATARI_KEY_1 			= 6    
    byte ATARI_KEY_2 			= 7    
    byte ATARI_KEY_3 			= 8    
    byte ATARI_KEY_4 			= 9    
    byte ATARI_KEY_5 			= 10    
    byte ATARI_KEY_6 			= 11    
    byte ATARI_KEY_7 			= 12    
    byte ATARI_KEY_8 			= 13    
    byte ATARI_KEY_9 			= 14    
    byte ATARI_KEY_0 			= 15    
    byte ATARI_KEY_A 			= 16    
    byte ATARI_KEY_H 			= 17
