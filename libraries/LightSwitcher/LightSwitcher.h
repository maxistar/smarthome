#ifndef LIGHT_SWITCHER_H
#define LIGHT_SWITCHER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#define DEBOUNCE 50  

class LightSwitcher {
    int buttonPin;
    char pressed; 
    char light1_on;


    //debouncing staff
    char isDebouncing;
    long debounceLastTime;  
    void checkSwitches();

    void (*onPressCallback)(char state) = NULL;
    void (*onReleaseCallback)() = NULL;
    void (*onKeepPressedCallback)() = NULL;

    public:
    LightSwitcher(int buttonPin);
    LightSwitcher(int buttonPin, void (*onPressCallback)(char state));
    LightSwitcher(int buttonPin, void (*onPressCallback)(char state), void (*onReleaseCallback)());
    LightSwitcher(int buttonPin, void (*onPressCallback)(char state), void (*onReleaseCallback)(), void (*onKeepPressedCallback)());
    void loop();
    void setup();
};


LightSwitcher::LightSwitcher(int buttonPin){
    this->buttonPin = buttonPin;
} 

LightSwitcher::LightSwitcher(int buttonPin, void (*onPressCallback)(char)){
    this->buttonPin = buttonPin;
    this->onPressCallback = onPressCallback;
}

LightSwitcher::LightSwitcher(int buttonPin, void (*onPressCallback)(char), void (*onReleaseCallback)()){
    this->buttonPin = buttonPin;
    this->onPressCallback = onPressCallback;
    this->onReleaseCallback = onReleaseCallback;
}

LightSwitcher::LightSwitcher(int buttonPin, void (*onPressCallback)(char), void (*onReleaseCallback)(), void (*onKeepPressedCallback)()){
    this->buttonPin = buttonPin;
    this->onPressCallback = onPressCallback;
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
    char currentstate = !digitalRead(this->buttonPin);   // read the button
    
    if (currentstate != this->pressed) {
        if (currentstate == 1) {
            // just pressed
            if (this->light1_on == 1) {
               this->light1_on = 0;
            }
            else {
                this->light1_on = 1;
            }
            if (onPressCallback != NULL) {
                onPressCallback(this->light1_on);              
            }
            this->pressed = 1;
        }
        else if (currentstate == 0) {
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

#endif

