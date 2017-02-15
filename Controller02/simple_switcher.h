#include "Arduino.h"

class SimpleSwitcher {
    int buttonPin;
    int ledPin;
    char pressed; 
    int light1_on;


    //debouncing staff
    char isDebouncing;
    long debounceLastTime;  
    void checkSwitches();

    void onButtonPressed();
    void onButtonReleased();
    void onButtonKeepsPressed();

    public:
    SimpleSwitcher(int buttonPin, int ledPin);
    void loop();
    void setup();
};
