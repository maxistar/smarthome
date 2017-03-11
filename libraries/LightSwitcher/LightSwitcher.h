#ifndef LIGHT_SWITCHER_H
#define LIGHT_SWITCHER_H

#include "Arduino.h"


#define DEBOUNCE 50  

class LightSwitcher {
    int buttonPin;
    char pressed; 
    char light1_on;


    //debouncing staff
    char isDebouncing;
    long debounceLastTime;  
    void checkSwitches();

    void (*onPressCallback)(char state);
    void (*onReleaseCallback)();
    void (*onKeepPressedCallback)();

    void _init();

    public:
    LightSwitcher(int buttonPin);
    LightSwitcher(int buttonPin, void (*onPressCallback)(char state));
    LightSwitcher(int buttonPin, void (*onPressCallback)(char state), void (*onReleaseCallback)());
    LightSwitcher(int buttonPin, void (*onPressCallback)(char state), void (*onReleaseCallback)(), void (*onKeepPressedCallback)());
    void loop();
    void setup();
};


#endif

