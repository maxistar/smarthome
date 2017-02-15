#include "Arduino.h"

class SimpleDimmer: public BaseDimmer {
    int light1_led;
    int light1_on;
    int light1_max;
    int light1_state; //0 ready
    int light1_step; //step to go
    int light1_stepSetup;
    int light1_stepLight;
    int light1_value;
    int light1_target;
    int light1_setupMode;
    long light1_pressedTime;
    long lasttime;


    void doAnimation();
    void onButtonPressed();
    void onButtonReleased();
    void onButtonKeepsPressed();
    
    public:
    SimpleDimmer(int buttonPin, int ledPin);
    void loop();
    void setup();
};
