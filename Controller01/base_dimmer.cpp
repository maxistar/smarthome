#ifndef BASE_DIMMER_H
#define BASE_DIMMER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "base_dimmer.h"

void BaseDimmer::setButton(int buttonPin) {
    this->buttonPin = buttonPin;
    this->debounceLastTime = 0;
    this->isDebouncing = 0;
}

void BaseDimmer::setup() {
    pinMode(this->buttonPin, INPUT);
    digitalWrite(this->buttonPin, HIGH);
}


void BaseDimmer::checkSwitches()
{
    if (this->isDebouncing && (this->debounceLastTime + DEBOUNCE) > millis()) {
        return; // not enough time has passed to debounce
    }
    char currentstate = !digitalRead(this->buttonPin);   // read the button
  
    if (currentstate != this->pressed) {
        if (currentstate == 1) {
            // just pressed
            this->onButtonPressed();
            this->pressed = 1;
        }
        else if (currentstate == 0) {
            // just released
            this->onButtonReleased();
            this->pressed = 0;
        }
        this->isDebouncing = 1;
    } 
    else {
        this->isDebouncing = 0; 
    }
    // ok we have waited DEBOUNCE milliseconds, lets reset the timer
    this->debounceLastTime = millis();
}

void BaseDimmer::loop() {
    this->checkSwitches();      // when we check the switches we'll get the current state
 
    if (this->pressed) {
        this->onButtonKeepsPressed();
    }
}


#endif

