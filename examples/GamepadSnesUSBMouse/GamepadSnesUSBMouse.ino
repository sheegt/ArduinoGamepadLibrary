/*
 * A example for using a SNES Gampead as an mouse.  
 *  
 * Tested with an Arduino Unoa and ArduinoIDE 1.6.9.
 * 
 * |----------------------------------|
 * |  Button    |  Description        |
 * |----------------------------------|
 * |  START     | Enable mouse        | 
 * |  D-Pad     | Moving the cursor   |
 * |  B         | Left mouse mutton   | 
 * |  A         | Right mouse button  | 
   |  SELECT    | Reset sensivitiy    |
 * |  Y         | Sensitivtiy + 1     |
 * |  X         | Sensitivtiy - 1     |
 * |  R         | Acceleration        |
 * |  L         | Middle mouse button |
 * |----------------------------------|
 * 
 * My site: https://github.com/sheegt/ArduinoGamepadLibrary
 * 
 * by S.Heegt
 * 2016-07-23
 */

#include <Gamepad.h>
#include <Mouse.h>

// feel free to edit the values to your gusto
typedef struct
{
  int xPos = 0;
  int yPos = 0;
  byte xRight = 1;
  byte xLeft = -1;  
  byte yUp = -1;
  byte yDown = 1;
  byte sensitivity = 1;
  byte acceleration = 1;
  int updateRate = 10;
  bool enabled = false;
} mouseObj;

mouseObj m;
Gamepad gp1;
byte gamepadLayout = GL_NINTENDO_SNES;
byte pin1 = 2;
byte pin2 = 3;
byte pin3 = 4;
byte pin4 = 5;
byte pin5 = 6;
byte pin6 = 7;
byte pin7 = 8;
byte mcPins[] = {pin1,pin2,pin3,pin4,pin5,pin6,pin7};
unsigned long curMillis = 0;
int xPlus = 0;
int yPlus = 0;

void setup() 
{
  gp1.begin(gamepadLayout, mcPins, true);
}

void loop() 
{
  xPlus = 0;
  yPlus = 0;
  m.xPos = 0;
  m.yPos = 0;
  m.acceleration = 1;

  // Read the button States
  gp1.readButtons();
  
  //Check if a button is pressed
  while(gp1.buffer.available() == true)
  {
    byte btn = gp1.buffer.next();
    checkButton(btn);
  }

  // Update the mouse
  updateMouse();
}

void checkButton(byte btn)
{
   switch(btn)
    {
      case SNES_BTN_START:
          if(m.enabled == true)
          {
            Mouse.end();
            m.enabled = false;
          }
          else
          {
            Mouse.begin();
            m.enabled = true;
          }
      break;
      
      case SNES_BTN_B:
        if(m.enabled == true) { Mouse.click(MOUSE_LEFT); }
      break;

      case SNES_BTN_L:
        if(m.enabled == true) { Mouse.click(MOUSE_MIDDLE); }
      break;
      
      case SNES_BTN_A:
        if(m.enabled == true) { Mouse.click(MOUSE_RIGHT); }
      break;

      case SNES_BTN_SELECT:
        m.sensitivity = 1;
      break;

      case SNES_BTN_Y:
        if(m.sensitivity > 1) { m.sensitivity--; }
      break;
      
      case SNES_BTN_X:
        m.sensitivity++;
      break;      
    }
  return;
}

void updateMouse()
{
  if(m.enabled == true)
  {
    if(curMillis + m.updateRate < millis())
    {
      curMillis = millis();   

      // Check if the direction keys are pressed
      if(gp1.getButtonState(SNES_BTN_R) == LOW) { m.acceleration  = 5; }
      if(gp1.getButtonState(SNES_BTN_RIGHT) == LOW) {xPlus = m.xRight; }
      if(gp1.getButtonState(SNES_BTN_LEFT) == LOW) { xPlus = m.xLeft; }
      if(gp1.getButtonState(SNES_BTN_UP) == LOW) { yPlus = m.yUp; }
      if(gp1.getButtonState(SNES_BTN_DOWN) == LOW) { yPlus = m.yDown; }
      
      // Calculate mouse position
      m.xPos = xPlus * m.sensitivity * m.acceleration; 
      m.yPos = yPlus * m.sensitivity * m.acceleration;

      // Move the mouse
      Mouse.move(m.xPos,m.yPos, 0);
    }
    // Enable Drag'n Drop
    if(gp1.getButtonState(SNES_BTN_B) == LOW) { Mouse.press(); }
    if(gp1.getButtonState(SNES_BTN_B) == HIGH) { Mouse.release(); }
   }  
}

