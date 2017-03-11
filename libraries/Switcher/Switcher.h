#include "Arduino.h"

class Switcher {
    int buttonPin;
    char pressed; 
    int mode;
    void (*changeCallbackPtr)(char);
    void (*pressCallbackPtr)();
    void (*releaseCallbackPtr)();
    
    //debouncing staff
    char isDebouncing;
    long debounceLastTime;  
    void checkSwitches();
    void _onPressed();
    void _onReleased();
    void _onChanged(char state);

    void _setup(int buttonPin);

    public:
    Switcher(int buttonPin);
    Switcher(int buttonPin, int mode, void (*changeCallbackPtr)(char));
    Switcher(int buttonPin, void (*changeCallbackPtr)(char));
    void setChangeCallback(void (*changeCallbackPtr)(char));
    void setPressCallback(void (*pressCallbackPtr)());
    void setReleaseCallback(void (*releaseCallbackPtr)());
    void loop();
    void setup();
};
