#include "Arduino.h"

class Switcher {
    int buttonPin;
    uint8_t pressed;
    int mode;
    void (*changeCallbackPtr)(uint8_t);
    void (*pressCallbackPtr)();
    void (*releaseCallbackPtr)();
    
    //debouncing staff
    uint8_t isDebouncing;
    long debounceLastTime;  
    void checkSwitches();
    void _onPressed();
    void _onReleased();
    void _onChanged(uint8_t state);

    void _setup(int buttonPin);

    public:
    Switcher(int buttonPin);
    Switcher(int buttonPin, int mode, void (*changeCallbackPtr)(uint8_t));
    Switcher(int buttonPin, void (*changeCallbackPtr)(uint8_t));
    void setChangeCallback(void (*changeCallbackPtr)(uint8_t));
    void setPressCallback(void (*pressCallbackPtr)());
    void setReleaseCallback(void (*releaseCallbackPtr)());
    void loop();
    void setup();
    uint8_t isOn();
};
