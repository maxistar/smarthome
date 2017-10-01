#ifndef CEILING_CONTROLLER_H
#define CEILING_CONTROLLER_H

#include "SimpleLedDimmer.h"
#include "Arduino.h"

#define PRESETS_COUNT 5
#define COUNT_LIGHTS 6

#define W1_ON_MASK 0x01
#define W2_ON_MASK 0x02
#define W3_ON_MASK 0x04
#define W4_ON_MASK 0x08
#define W5_ON_MASK 0x10
#define W6_ON_MASK 0x20


#define ANIMATION_STEP 10
#define LIGHT_STEP 4

struct LightsState {
  byte w1;
  byte w2;
  byte w3;
  byte w4;
  byte w5;
  byte w6;
};

class SimpleLedDimmer;

struct CeilingControllerConfig {
    byte pinW1;
    byte pinW2;
    byte pinW3;
    byte pinW4;
    byte pinW5;
    byte pinW6;
    byte pinPower;

    word *modbus;
    byte modbusReadRegisters;
    byte modbusReadWriteRegisters;
    byte modbusOnBit;
    byte modbusPresetNum;
    byte modbusW1;
    byte modbusW2;
    byte modbusW3;
    byte modbusW4;
    byte modbusW5;
    byte modbusW6;
};

class CeilingController {
private:
    CeilingControllerConfig *config;
    byte currentPresetNum = 0;
    LightsState presets[PRESETS_COUNT];
    void restorePresets();
    void savePresets();
    byte onValue = 0;
    byte powerState = 0;

    SimpleLedDimmer *w1;
    SimpleLedDimmer *w2;
    SimpleLedDimmer *w3;
    SimpleLedDimmer *w4;
    SimpleLedDimmer *w5;
    SimpleLedDimmer *w6;

    void onLedPowerOn(byte mask);
    void onLedPowerOff(byte mask);
    friend void onPowerOnW1(CeilingController *c);
    friend void onPowerOffW1(CeilingController *c);
    friend void onPowerOnW2(CeilingController *c);
    friend void onPowerOffW2(CeilingController *c);
    friend void onPowerOnW3(CeilingController *c);
    friend void onPowerOffW3(CeilingController *c);
    friend void onPowerOnW4(CeilingController *c);
    friend void onPowerOffW4(CeilingController *c);
    friend void onPowerOnW5(CeilingController *c);
    friend void onPowerOffW5(CeilingController *c);
    friend void onPowerOnW6(CeilingController *c);
    friend void onPowerOffW6(CeilingController *c);

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
    void setOn(uint8_t);
    uint8_t isOn();
    uint8_t isOff();
    void nextMode();
};

#endif
