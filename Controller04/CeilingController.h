#ifndef CEILING_CONTROLLER_H
#define CEILING_CONTROLLER_H

#include "SimpleLedDimmer.h"
#include "Arduino.h"

#define PRESETS_COUNT 3

#define W1_ON_MASK 0x01
#define W2_ON_MASK 0x02
#define R_ON_MASK 0x04
#define G_ON_MASK 0x08
#define B_ON_MASK 0x10

#define ANIMATION_STEP 10
#define LIGHT_STEP 4

struct LightsState {
  byte w1;
  byte w2;
  byte r;
  byte g;
  byte b;
};

class SimpleLedDimmer;

struct CeilingControllerConfig {
    byte pinW1;
    byte pinW2;
    byte pinR;
    byte pinG;
    byte pinB;
    byte pinPower;

    word *modbus;
    byte modbusReadRegisters;
    byte modbusReadWriteRegisters;
    byte modbusOnBit;
    byte modbusPresetNum;
    byte modbusW1;
    byte modbusW2;
    byte modbusR;
    byte modbusG;
    byte modbusB;
};

class CeilingController {
private:
    CeilingControllerConfig *config;
    byte currentPresetNum = 0;
    LightsState presets[PRESETS_COUNT];
    void restorePresets();
    byte onValue = 0;
    byte powerState = 0;

    SimpleLedDimmer *w1;
    SimpleLedDimmer *w2;
    SimpleLedDimmer *r;
    SimpleLedDimmer *g;
    SimpleLedDimmer *b;

    void onLedPowerOn(byte mask);
    void onLedPowerOff(byte mask);
    friend void onPowerOnW1(CeilingController *c);
    friend void onPowerOffW1(CeilingController *c);
    friend void onPowerOnW2(CeilingController *c);
    friend void onPowerOffW2(CeilingController *c);
    friend void onPowerOnR(CeilingController *c);
    friend void onPowerOffR(CeilingController *c);
    friend void onPowerOnG(CeilingController *c);
    friend void onPowerOffG(CeilingController *c);
    friend void onPowerOnB(CeilingController *c);
    friend void onPowerOffB(CeilingController *c);

    void copyPresetsToModbus();

    void (*onChangeCallback)(byte value) = NULL;
    void init(CeilingControllerConfig *config);

public:
    CeilingController(CeilingControllerConfig *config);
    CeilingController(CeilingControllerConfig *config, void (*onChangeCallback)(byte value));

    void setup();
    void loop();
    void on();
    void off();
    char isOn();
    char isOff();
};

#endif
