#include "Arduino.h"

class SimpleKitchenDimmer: public BaseDimmer {
    // here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
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

    
    int light2On;
    int light2Pin;
    
    void doAnimation();
    void onButtonPressed();
    void onButtonReleased();
    void onButtonKeepsPressed();
    
    public:
    SimpleKitchenDimmer(int buttonPin, int ledPin, int light2Pin);
    void loop();
    void setup();
};
