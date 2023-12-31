#ifndef SWITCHER_H
#define SWITCHER_H

#include "Arduino.h"
#include "Switcher.h"

#define DEBOUNCE 50

Switcher::Switcher(int buttonPin){
	this->_setup(buttonPin);
} 

Switcher::Switcher(int buttonPin, int mode, void (*changeCallbackPtr)(uint8_t)) {
	this->_setup(buttonPin);
	this->mode = mode;
	this->changeCallbackPtr = changeCallbackPtr;
}

Switcher::Switcher(int buttonPin, void (*changeCallbackPtr)(uint8_t)) {
	this->_setup(buttonPin);
	this->changeCallbackPtr = changeCallbackPtr;
}

void Switcher::_setup(int buttonPin) {
    this->buttonPin = buttonPin;
    this->mode = INPUT_PULLUP;
    changeCallbackPtr = NULL;
    pressCallbackPtr = NULL;
    releaseCallbackPtr = NULL;
    this->debounceLastTime = 0;
    this->isDebouncing = 0;
}

void Switcher::setup(){
    pinMode(this->buttonPin, this->mode);
    pressed = !digitalRead(this->buttonPin);
} 


void Switcher::checkSwitches()
{
    if (this->isDebouncing && (this->debounceLastTime + DEBOUNCE) > millis()) {
        return; // not enough time has passed to debounce
    }
    uint8_t buttonState = digitalRead(this->buttonPin);
    uint8_t currentState = !buttonState;   // read the button

    if (currentState != this->pressed) {
        this->pressed = currentState;
        if (currentState == 1) {
            this->_onPressed();
        }
        else {
            this->_onReleased();
        }
        this->_onChanged(this->pressed);
        this->isDebouncing = 1;
    } 
    else {
        this->isDebouncing = 0; 
    }
    // ok we have waited DEBOUNCE milliseconds, lets reset the timer
    this->debounceLastTime = millis();
}

void Switcher::loop() {
    this->checkSwitches();      // when we check the switches we'll get the current state 
}

void Switcher::_onPressed() {
    if (pressCallbackPtr != NULL) {
        pressCallbackPtr();       
    }
}

void Switcher::_onReleased() {
    if (releaseCallbackPtr != NULL) {
        releaseCallbackPtr();      
    }
}

void Switcher::_onChanged(uint8_t state) {
    if (changeCallbackPtr != NULL) {
        changeCallbackPtr(state);
    }
}

void Switcher::setChangeCallback(void (*changeCallbackPtr)(uint8_t)) {
    this->changeCallbackPtr = changeCallbackPtr;
}
void Switcher::setPressCallback(void (*pressCallbackPtr)()) {
    this->pressCallbackPtr = pressCallbackPtr;
}
void Switcher::setReleaseCallback(void (*releaseCallbackPtr)()) {
    this->releaseCallbackPtr = releaseCallbackPtr;
}

uint8_t Switcher::isOn() {
	return !pressed;
}



#endif
