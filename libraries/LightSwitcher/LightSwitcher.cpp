
#include "Arduino.h"
#include "LightSwitcher.h"

void LightSwitcher::_init() {
    onChangeCallback = NULL;
    onReleaseCallback = NULL;
    onKeepPressedCallback = NULL;
    press_before_unpress = 0;
    pressed = 0;
    light_on = 0;
}

LightSwitcher::LightSwitcher(int buttonPin){
    _init();
	this->buttonPin = buttonPin;
} 

LightSwitcher::LightSwitcher(int buttonPin, void (*onChangeCallback)(uint8_t)){
    _init();
	this->buttonPin = buttonPin;
    this->onChangeCallback = onChangeCallback;
}

LightSwitcher::LightSwitcher(int buttonPin, void (*onChangeCallback)(uint8_t), void (*onReleaseCallback)()){
    _init();
	this->buttonPin = buttonPin;
    this->onChangeCallback = onChangeCallback;
    this->onReleaseCallback = onReleaseCallback;
}

LightSwitcher::LightSwitcher(int buttonPin, void (*onChangeCallback)(uint8_t), void (*onReleaseCallback)(), void (*onKeepPressedCallback)()){
    _init();
	this->buttonPin = buttonPin;
    this->onChangeCallback = onChangeCallback;
    this->onReleaseCallback = onReleaseCallback;
    this->onKeepPressedCallback = onKeepPressedCallback;
}


void LightSwitcher::setup(){
    pinMode(this->buttonPin, INPUT_PULLUP);
} 


void LightSwitcher::checkSwitches()
{

    if (this->isDebouncing && (this->debounceLastTime + DEBOUNCE) > millis()) {
        return; // not enough time has passed to debounce
    }
    uint8_t currentstate = !digitalRead(this->buttonPin);   // read the button
    
        if (currentstate != this->pressed) {
            if (currentstate == 1) {
                // just pressed
                if (this->light_on != 1) {
                    this->light_on = 1;
                    if (onChangeCallback != NULL) {
                        onChangeCallback(this->light_on);
                    }
                } else {
                	this->press_before_unpress = 1;
                }

                this->pressed = 1;
            }
            else if (currentstate == 0) {
            	if (this->light_on == 1 && press_before_unpress == 1) {
            	    this->light_on = 0;
            	    press_before_unpress = 0;
            	    if (onChangeCallback != NULL) {
            	        onChangeCallback(this->light_on);
            	    }
            	} else {
            		this->press_before_unpress = 0;
            	}
            	// just released
                if (onReleaseCallback != NULL) {
                    onReleaseCallback();
                }
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

void LightSwitcher::loop() {
    this->checkSwitches(); // when we check the switches we'll get the current state
 
    if (this->pressed && onKeepPressedCallback != NULL) {
        onKeepPressedCallback();
    }
}

void LightSwitcher::setOn(uint8_t state) {
	this->light_on = state;
}
