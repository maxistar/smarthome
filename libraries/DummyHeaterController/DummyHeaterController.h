#ifndef DUMMY_HEATER_CONTROLLER_H
#define DUMMY_HEATER_CONTROLLER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class DummyHeaterController {
private:
    float scale = 0.5;
    long interval = 60000; //1 minute
    long intervalTime = NULL;
    long triggerTime = NULL; 
    int pinNumber = 0; 
    byte value = 0;
    void (*onChangeCallback)(byte value) = NULL;
  
public:
    DummyHeaterController(int pinNumber);
    DummyHeaterController(int pinNumber, void (*onChangeCallback)(byte value));
    void setInterval(long value);
    void setScale(float value);
    void setup();
    void loop();
    byte isOn();
    void setOnValue(byte value);
};

DummyHeaterController::DummyHeaterController(int pinNumber) {
    this->pinNumber = pinNumber;
};

DummyHeaterController::DummyHeaterController(int pinNumber, void (*onChangeCallback)(byte value)) {
    this->pinNumber = pinNumber;
    this->onChangeCallback = onChangeCallback;
};

void DummyHeaterController::setup() {
    this->intervalTime = millis() + interval;
    pinMode(this->pinNumber, OUTPUT);
};

void DummyHeaterController::loop() {
    long currentTime = millis();
    if (currentTime > this->intervalTime) {
        this->intervalTime = currentTime + this->interval;
        this->triggerTime = currentTime + this->interval * this->scale;
        if (this->triggerTime > currentTime) {
            digitalWrite(this->pinNumber, HIGH);
            if (onChangeCallback != NULL) {
                onChangeCallback(HIGH);
            }
            this->value = 1;
        }
    }
    if (this->value == 1 && currentTime > this->triggerTime) {
        digitalWrite(this->pinNumber, LOW);
        if (onChangeCallback != NULL) {
            onChangeCallback(LOW);
        }
        this->value = 0;
    }
};

void DummyHeaterController::setInterval(long value) {
    if (this->interval == value) return;
    this->interval = value;
};

void DummyHeaterController::setScale(float value) {
    if (this->scale == value) return;
    if (this->scale > 1) return;
    this->scale = value;
};

byte DummyHeaterController::isOn() {
    return this->value;
};

void DummyHeaterController::setOnValue(byte value) {
    if (this->value != value) {
        this->value = value;
        digitalWrite(this->pinNumber, value);
    }
};


#endif
