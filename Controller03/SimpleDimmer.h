#include "Arduino.h"

class SimpleDimmer {
    int ledPin;
    int powerPin;
    int powerOn;
    int lightOn;
    int lightMax;
    int lightStep; //step to go
    int lightStepSetup;
    int lightStepLight;
    int lightValue;
    int lightTarget;
    int lightSetupMode;
    long lightPressedTime;
    long lastTime;
    int buttonPin;

    //modbus stuff
    uint16_t *modbus; 
    int buttonBit; 
    int stateBit;
    int maxValueIndex;

    //debouncing stuff
    char isDebouncing;
    long debounceLastTime;

    // we will track if a button is just pressed, just released, or 'currently pressed' 
    char pressed; 
    long buttonPressedTime;
    
    void checkSwitches();
    void doAnimation();
    void onButtonPressed();
    void onButtonReleased();
    void onButtonKeepsPressed();
    
    public:
    SimpleDimmer(int buttonPin, int ledPin, int powerPin, uint16_t *modbus, int buttonBit, int stateBit, int maxValueIndex);
    
    int isOn();
    int isOff();
    
    void on();
    void off();
    
    void loop();
    void setup();
};
