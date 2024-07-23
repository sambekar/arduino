// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "myfonts.h"
#include <Keypad.h>

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 8
#define CS_PIN 10

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {0, 7, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);
	// Intialize the object
	myDisplay.begin();

	// Set the intensity (brightness) of the display (0-15)
	myDisplay.setIntensity(3);

	// Clear the display
	myDisplay.displayClear();
  
  
  //myDisplay.displayText("SHAURYA", PA_CENTER, 5, 500, PA_SLICE, PA_SLICE);
}

int acquisitionMode = 0;
void loop() {
  //int r = Serial.read(); // -1 when nothing
  char customKey = customKeypad.getKey();
  Serial.println(customKey);
  switch ( customKey ) {
    case '0':  acquisitionMode = 0;  break;  // G for Go
    case '1':  acquisitionMode = 1;  break;  // G for Go
    case '2':  acquisitionMode = 2; break;  // S for Stop
    case '3':  acquisitionMode = 3; break;  // S for Stop
    case '4':  acquisitionMode = 4; break;  // S for Stop
    case '5':  acquisitionMode = 5; break;  // S for Stop
    default: break;  // ignore everything else
  }
  
  if (acquisitionMode == 1) {
    if (myDisplay.displayAnimate()){
      myDisplay.displayShutdown(false);
    myDisplay.setFont(fontSudeep);
    myDisplay.setCharSpacing(0);
    myDisplay.displayText("AB AB AB ", PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);}
  } else if (acquisitionMode == 2){
    if (myDisplay.displayAnimate()){
    myDisplay.setFont(fontSudeep);
    myDisplay.setCharSpacing(0);
    myDisplay.displayText("BC BC BC ", PA_CENTER, 30, 0, PA_SCROLL_RIGHT, PA_NO_EFFECT);}
  } else if (acquisitionMode == 3){
    if (myDisplay.displayAnimate()){
    myDisplay.setFont(nullptr);
    myDisplay.setCharSpacing(1);
    myDisplay.displayText("STOP", PA_CENTER, 5, 10, PA_PRINT, PA_NO_EFFECT);
    }
  }
  else if (acquisitionMode == 4){
    if (myDisplay.displayAnimate()){
    myDisplay.setFont(nullptr);
    myDisplay.setCharSpacing(1);
    myDisplay.displayText("FREE FIRE ", PA_CENTER, 50, 10, PA_SCROLL_LEFT, PA_NO_EFFECT);
    
    }
  }
  else if (acquisitionMode == 5){
    if (myDisplay.displayAnimate()){
    myDisplay.setFont(fontSudeep);
    myDisplay.setCharSpacing(1);
    myDisplay.displayText("DEFF", PA_CENTER, 40, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);}
  }
  else if (acquisitionMode == 0){
    if (myDisplay.displayAnimate()){
      myDisplay.displayShutdown(true);}
  }

}