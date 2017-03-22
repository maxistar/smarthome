#ifndef LIGHT_SWITCHER_H
#define LIGHT_SWITCHER_H

#include "Arduino.h"


#define DEBOUNCE 50  

class LightSwitcher {
    int buttonPin;
    uint8_t pressed;
    uint8_t light_on;
    uint8_t press_before_unpress;


    //debouncing staff
    uint8_t isDebouncing;
    long debounceLastTime;  
    void checkSwitches();

    void (*onChangeCallback)(uint8_t state);
    void (*onReleaseCallback)();
    void (*onKeepPressedCallback)();

    void _init();

    public:
    LightSwitcher(int buttonPin);
    LightSwitcher(int buttonPin, void (*onChangeCallback)(uint8_t state));
    LightSwitcher(int buttonPin, void (*onChangeCallback)(uint8_t state), void (*onReleaseCallback)());
    LightSwitcher(int buttonPin, void (*onChangeCallback)(uint8_t state), void (*onReleaseCallback)(), void (*onKeepPressedCallback)());
    void loop();
    void setup();
    void setOn(uint8_t state);
};


#endif

