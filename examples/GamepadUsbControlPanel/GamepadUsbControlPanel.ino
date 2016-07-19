/*
 * A quick and dirty example for using the supported Gamepads as an USB Gamepad. The Joystick library from MHeironimus is needed. You can get it here:
 * https://github.com/MHeironimus/ArduinoJoystickLibrary
 * 
 * Use the program GamepadControlPanel.exe for setting the GamepadLayout. It's included in the subfolder 'controlPanel'.
 * 
 * Tested with an Arduino Unoa and ArduinoIDE 1.6.9.
 * 
 * My site: https://github.com/sheegt/ArduinoGamepadLibrary
 * 
 * by S.Heegt
 * 2016-07-11
 */

#include <SoftwareSerial.h>
#include <Joystick.h>
#include <Gamepad.h>

Gamepad gp1;

const String VERSION = "1.0";
byte gamepadLayout = GL_NOT_SET;
byte pin1 = 2;
byte pin2 = 3;
byte pin3 = 4;
byte pin4 = 5;
byte pin5 = 6;
byte pin6 = 7;
byte pin7 = 8;
byte pin8 = 9;
byte pin9 = 10;
byte mcPins[] = {pin1,pin2,pin3,pin4,pin5,pin6,pin7,pin8,pin9};
  
void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(20);

  Joystick.begin(false);

  gp1.begin(gamepadLayout, mcPins, true);

}

void loop() 
{
  //  Check if a command was send to change the GamepadLayout
  checkSerial();

  // Read the buttons 
  gp1.readButtons();

  // Submit the button states to the JoystickLibrary
  for(byte x=0;x<32;x++)
  {
    byte state = gp1.getButtonState(x);
    state = (state == LOW) ? HIGH : LOW;
    Joystick.setButton(x, state);
  }

  // Send the states 
  Joystick.sendState();
}

void checkSerial()
{
   if(Serial.available() > 0)
    {
      int startCommandPos = -1;
      int startValPos = -1;
      int endValPos = -1;
      String inStr = Serial.readString();
        
      if(inStr == "%") 
      {
        Serial.print("@GP"); // Give a feedback to the calling program.
        return;
      }
      // Check if a command is found in the stream
      for(int i=0;i<inStr.length();i++)
      {
        char inChar = inStr.charAt(i);
        switch(inChar)
        {
          case '$':
            startCommandPos = i + 1;
          break;
          
          case '=':
            startValPos = i + 1;
          break;  
          
          case ';':
            if(startValPos == -1) { startValPos = i + 1; }
            endValPos = i;
          break;              
        }
      }
       String command = inStr.substring(startCommandPos, startValPos - 1);  
      String value = inStr.substring(startValPos, endValPos);  
      executeCommand(command, value);
    }
}

void executeCommand(String command, String value)
{
  if(command == "set_gl")
  {
   int gamepadLayout = value.toInt();
    gp1.setConnector(gamepadLayout, mcPins, true); 
    Serial.print("@SET");
  }
  if(command == "get_gl")
  {
    String response = "@GL=";
    response.concat(gp1.getGamepadLayout());
    Serial.print(response);
  }  
}
