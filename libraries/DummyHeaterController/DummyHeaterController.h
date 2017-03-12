#ifndef DUMMY_HEATER_CONTROLLER_H
#define DUMMY_HEATER_CONTROLLER_H

#include "Arduino.h"

class DummyHeaterController {
private:
    float scale;
    long interval; //1 minute
    long intervalTime;
    long triggerTime;
    int pinNumber;
    uint8_t value;
    void (*onChangeCallback)(uint8_t value);
    void _setup();

public:
    DummyHeaterController(int pinNumber);
    DummyHeaterController(int pinNumber, void (*onChangeCallback)(uint8_t value));
    void setInterval(long value);
    void setScale(float value);
    long getInterval();
    float getScale();

    void setup();
    void loop();
    uint8_t isOn();
    void setOnValue(uint8_t value);
};

#endif
