#include "Arduino.h"

class SimpleSwitcher {
    int buttonPin;
    int ledPin;
    char pressed; 
    int lightOn;
    
    uint16_t *modbus; 
    int buttonBit; 
    int stateBit;


    //debouncing staff
    char isDebouncing;
    long debounceLastTime;  
    void checkSwitches();
    void setLight();
    void onButtonPressed();
    void onButtonKeepsPressed();

    public:
    SimpleSwitcher(int buttonPin, int ledPin, uint16_t *modbus, int buttonBit, int stateBit);
    void loop();
    void setup();
};
