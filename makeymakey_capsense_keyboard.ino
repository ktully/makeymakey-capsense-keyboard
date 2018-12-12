#include "Arduino.h"
#define NUM_INPUTS 18

// keys
// edit this array to change the keys pressed 
int keys[NUM_INPUTS] = {
  // top side of the makey makey board

  KEY_UP_ARROW,      // up arrow pad
  KEY_DOWN_ARROW,    // down arrow pad
  KEY_LEFT_ARROW,    // left arrow pad
  KEY_RIGHT_ARROW,   // right arrow pad
  ' ',               // space button pad
  MOUSE_LEFT,        // click button pad
  
  // female header on the back left side
  
  'w',                // pin D5
  'a',                // pin D4
  's',                // pin D3
  'd',                // pin D2
  'f',                // pin D1
  'g',                // pin D0
  
  // female header on the back right side
  
  MOUSE_MOVE_UP,      // pin A5
  MOUSE_MOVE_DOWN,    // pin A4
  MOUSE_MOVE_LEFT,    // pin A3
  MOUSE_MOVE_RIGHT,   // pin A2
  MOUSE_LEFT,         // pin A1
  MOUSE_RIGHT         // pin A0
};

// cap sense threshold for each pin
// this number is proportional to the capacitance on the pin that will count as a press
// it is units of a very small unit of time, in iterations through an unrolled loop
// higher values make it less sensitive (i.e. require larger capacitance)

int capThresholds[NUM_INPUTS] = {
  1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1,
};

// Pin Numbers
// input pin numbers for kickstarter production board
int pinNumbers[NUM_INPUTS] = {
  12, 8, 13, 15, 7, 6,     // top of makey makey board
  5, 4, 3, 2, 1, 0,        // left side of female header, KEYBOARD
  23, 22, 21, 20, 19, 18   // right side of female header, MOUSE
};


const int outputPin = 14; // pin D14, leftmost pin on the output header
// TODO: restrict pin 14 to actual key-presses only, and restore pin 16 going high to mouse moves
// TODO: add a way to swith cap and resistive on fly maybe, or to have a mix

boolean pressed[NUM_INPUTS];

void setup(){
  Keyboard.begin();
  for (int i=0; i<NUM_INPUTS; i++) {
    pressed[i] = false;
  }

  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);

}

void loop() { 
  for (int i=0; i<NUM_INPUTS; i++) {                      // for each pin
    if (readCapacitivePin(pinNumbers[i])>capThresholds[i]){       // if we detect a touch on the pin
      if (!pressed[i]) {                                          // and if we're not already pressed
        Keyboard.press(keys[i]);                                        // send the key press
        pressed[i] = true;                                              // remember it was pressed
      }
    } 
    else {                                                  // if we don't a detect touch on the pin
      if (pressed[i]) {                                           // if this key was pressed before
        Keyboard.release(keys[i]);                                   // send the key release
        pressed[i] = false;                                          // remember we are not pressed
      }        
    }
  }

  // OUTPUT
  // output pin D14 goes high while any input is pressed

  boolean anythingIsPressed = false;
  for (int i=0; i<NUM_INPUTS; i++) {                      
    if (pressed[i]) {
      anythingIsPressed = true;
    }
  }

  if (anythingIsPressed) {
    digitalWrite(outputPin, HIGH);
  } 
  else {
    digitalWrite(outputPin, LOW);
  }

}  



// CapacitiveSensor tutorial from http://www.arduino.cc/playground/Code/CapacitiveSensor
// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher

uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
  if (*pin & bitmask) { 
    cycles =  0;
  }
  else if (*pin & bitmask) { 
    cycles =  1;
  }
  else if (*pin & bitmask) { 
    cycles =  2;
  }
  else if (*pin & bitmask) { 
    cycles =  3;
  }
  else if (*pin & bitmask) { 
    cycles =  4;
  }
  else if (*pin & bitmask) { 
    cycles =  5;
  }
  else if (*pin & bitmask) { 
    cycles =  6;
  }
  else if (*pin & bitmask) { 
    cycles =  7;
  }
  else if (*pin & bitmask) { 
    cycles =  8;
  }
  else if (*pin & bitmask) { 
    cycles =  9;
  }
  else if (*pin & bitmask) { 
    cycles = 10;
  }
  else if (*pin & bitmask) { 
    cycles = 11;
  }
  else if (*pin & bitmask) { 
    cycles = 12;
  }
  else if (*pin & bitmask) { 
    cycles = 13;
  }
  else if (*pin & bitmask) { 
    cycles = 14;
  }
  else if (*pin & bitmask) { 
    cycles = 15;
  }
  else if (*pin & bitmask) { 
    cycles = 16;
  }

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}

