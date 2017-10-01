#ifndef SIMPLE_LED_DIMMER_H
#define SIMPLE_LED_DIMMER_H

#define ANIMATION_STEP 10
#define LIGHT_STEP 4

#include "CeilingController.h"
#include "Arduino.h"

class CeilingController;

class SimpleLedDimmer {
    int ledPin;
    int powerOn = 0;
    int lightOn = 0;
    int lightValue = 0;
    uint16_t *maxValue;
    long lastTime = 0;
    void (*powerOnCallback)(CeilingController *ceilingController) = NULL;
    void (*powerOffCallback)(CeilingController *ceilingController) = NULL;
    CeilingController *ceilingController;
    
    //modbus stuff
    uint16_t *modbus; 
    int maxValueIndex;

    void doAnimation();
    
    public:
    SimpleLedDimmer(CeilingController *ceilingController, int ledPin, uint16_t *maxValue);
    SimpleLedDimmer(CeilingController *ceilingController, int ledPin, uint16_t *maxValue, void (*powerOnCallback)(CeilingController *ceilingController), void (*powerOffCallback)(CeilingController *ceilingController));

    void setPowerOnCallback(void (*)(CeilingController *ceilingController));
    void setPowerOffCallback(void (*)(CeilingController *ceilingController));
    
    int isOn();
    int isOff();
    
    void on();
    void off();
    
    void loop();
    void setup();
};

#endif
