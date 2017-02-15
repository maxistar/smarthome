#ifndef SIMPLE_SWITCHER_H
#define SIMPLE_SWITCHER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "dimmer_types.h"
#include "simple_switcher.h"


SimpleSwitcher::SimpleSwitcher(int buttonPin, int ledPin){
    this->buttonPin = buttonPin;
    this->ledPin = ledPin;
} 

void SimpleSwitcher::setup(){
    pinMode(this->buttonPin, INPUT_PULLUP);
    //digitalWrite(this->buttonPin, HIGH);
    pinMode(this->ledPin, OUTPUT); 
    digitalWrite(this->ledPin, LOW);
} 


void SimpleSwitcher::checkSwitches()
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

void SimpleSwitcher::loop() {
    this->checkSwitches();      // when we check the switches we'll get the current state
 
    //if (this->pressed) {
    //    this->onButtonKeepsPressed();
    //}
}

void SimpleSwitcher::onButtonPressed() {
      if (this->light1_on == 1) {
       this->light1_on = 0;
       digitalWrite(this->ledPin, LOW);
    }
    else {
       this->light1_on = 1;
       digitalWrite(this->ledPin, HIGH);
    }
}

void SimpleSwitcher::onButtonReleased() {
}

#endif

