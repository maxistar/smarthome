#ifndef SIMPLE_SWITCHER_H
#define SIMPLE_SWITCHER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "SimpleSwitcher.h"

#define DEBOUNCE 50


SimpleSwitcher::SimpleSwitcher(int buttonPin, int ledPin, uint16_t *modbus, int buttonBit, int stateBit){
    this->buttonPin = buttonPin;
    this->ledPin = ledPin;
    this->modbus = modbus;
    this->buttonBit = buttonBit;
    this->stateBit = stateBit;
} 

void SimpleSwitcher::setup(){
    pinMode(this->buttonPin, INPUT_PULLUP);
    pinMode(this->ledPin, OUTPUT); 
    digitalWrite(this->ledPin, LOW);
} 


void SimpleSwitcher::checkSwitches()
{
    if (this->isDebouncing && (this->debounceLastTime + DEBOUNCE) > millis()) {
        return; // not enough time has passed to debounce
    }
    char buttonState = digitalRead(this->buttonPin);
    char currentState = !buttonState;   // read the button

    //Сохраняем состояние кнопки в регистр 0.x
    bitWrite(modbus[0], buttonBit, buttonState);
    
    if (currentState != this->pressed) {
        if (currentState == 1) {
            // just pressed
            this->onButtonPressed();
            this->pressed = 1;
        }
        else {
            // just released
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
    char modbusOn = bitRead(modbus[1], stateBit);
    if (modbusOn != lightOn) {
        lightOn = modbusOn;
        setLight();
    }
 
    this->checkSwitches();      // when we check the switches we'll get the current state 
}

void SimpleSwitcher::setLight() {
    if (this->lightOn == 0) {
       digitalWrite(this->ledPin, LOW);
    }
    else {
       digitalWrite(this->ledPin, HIGH);
    }
}

void SimpleSwitcher::onButtonPressed() {
    if (this->lightOn == 1) {
       this->lightOn = 0;
    }
    else {
       this->lightOn = 1;
    }

    setLight();
       
    //Сохраняем состояние кнопки в регистр 1.x
    bitWrite(modbus[1], stateBit, lightOn);
}

#endif

