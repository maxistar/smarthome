#include "Arduino.h"


class LivingRoomDimmer: public BaseDimmer {
    int light1Pin;
    int light2Pin;
    
    //state of our light
    int lightIsOn; //1 if light is on
    int lightMode; //0 - light1 is on, light2 is on,1 - light1 is off, light2 is on, 2, light1 is on light2 is off

    int setupMode;

    long modeChangedLastTime;

    void onButtonPressed();
    void onButtonReleased();
    void onButtonKeepsPressed();
    void doSetupModeLoop();
    void applyLightMode();

    public: 
    LivingRoomDimmer(int buttonPin, int light1Pin, int light2Pin);
    void setup();
    
};


