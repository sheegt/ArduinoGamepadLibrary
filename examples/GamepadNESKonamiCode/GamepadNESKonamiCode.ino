/*
 * A quick and dirty example for using a NES Gamepad and the button buffer to enter the famous Konami Code (UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, B, A)
 * If the inputed code is correct, the build-in LED will go out.
 * 
 * Tested with an Arduino Unoa and ArduinoIDE 1.6.9.
 *  
 *  My site: https://github.com/sheegt/ArduinoGamepadLibrary
 *  
 * by S.Heegt
 * 2016-07-11
 */

#include <SoftwareSerial.h>
#include <Gamepad.h>

// Init the Gamepad
Gamepad gp1;

// Define the needed variables
byte codeInput[] = {255,255,255,255,255,255,255,255,255,255};
byte codeNeeded[]  = {NES_BTN_UP, NES_BTN_UP, NES_BTN_DOWN, NES_BTN_DOWN, NES_BTN_LEFT, NES_BTN_RIGHT, NES_BTN_LEFT, NES_BTN_RIGHT, NES_BTN_B, NES_BTN_A};
byte codeInputIndex = 0;
byte codeSize = sizeof(codeNeeded);
byte ledPin = 13;
byte gamepadLayout = GL_NINTENDO_NES;

void setup() 
{
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
  Serial.begin(9600);

  while(!Serial) { }
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  gp1.begin(gamepadLayout, mcPins, true);
  Serial.println("Code has " + String(codeSize) + " chars.");
  Serial.print("Enter Code: ");
}

void loop() 
{
  gp1.readButtons();
  
  while(gp1.buffer.available() == true)
  {
    byte button = gp1.buffer.next();
   addToCodeInput(button);
  }
}

void addToCodeInput(byte button)
{
  digitalWrite(ledPin, HIGH);
  if(codeInputIndex == codeSize -1) // doesn't work with code sizes < 2)
  {
    codeInput[codeInputIndex] = button;
    Serial.print(bitToString(button) + " ");
    if(checkCode() == true) 
    { 
      Serial.print("\n\n");
      Serial.println("Code correct"); 
      digitalWrite(ledPin, LOW);
      delay(1500);
      digitalWrite(ledPin, HIGH);
      Serial.print("\n\n");
      Serial.print("Enter Code: ");
      flushCodeInput();      
    }
    else
    {
      Serial.print("\n\n");
      Serial.println("Code not correct");
      digitalWrite(ledPin, HIGH);
      delay(500);
      Serial.print("\n\n");
      Serial.print("Enter Code: ");
      flushCodeInput();
    }
    return;
  }
  else
  {
    codeInput[codeInputIndex] = button;
    Serial.print(bitToString(button) + " ");
    codeInputIndex = codeInputIndex + 1;
    return;
  }
}
 
bool checkCode()
{
  for(byte i=0;i<codeSize;i++)
  {
          if(codeInput[i] != codeNeeded[i]) { return false; }
  }
  return true;
}
 
void flushCodeInput()
{
  codeInputIndex = 0;
  return;
}

// Just a helper function
String bitToString(byte button)
{
  String alphaNumeric = "NULL";
  
  switch(button)
  {
    case NES_BTN_B:
      alphaNumeric = "B";
    break;
  
    case NES_BTN_A:
      alphaNumeric = "A";
    break;
  
    case NES_BTN_SELECT:
      alphaNumeric = "SELECT";
    break;
  
    case NES_BTN_START:
      alphaNumeric = "START";
    break;
  
    case NES_BTN_UP:
      alphaNumeric = "UP";
    break;
  
    case NES_BTN_DOWN:
      alphaNumeric = "DOWN";
    break;
  
    case NES_BTN_LEFT:
      alphaNumeric = "LEFT";
    break;
  
    case NES_BTN_RIGHT:
      alphaNumeric = "RIGHT";
    break;
  }

  return alphaNumeric;
}

