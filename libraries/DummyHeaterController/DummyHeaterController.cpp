#include "Arduino.h"
#include "DummyHeaterController.h"


void DummyHeaterController::_setup() {
	scale = 0.5;
	interval = 60000; //1 minute
	intervalTime = 0;
	triggerTime = 0;
	pinNumber = 0;
	value = 0;
	onChangeCallback = NULL;
}

DummyHeaterController::DummyHeaterController(int pinNumber) {
    this->_setup();
	this->pinNumber = pinNumber;
};



DummyHeaterController::DummyHeaterController(int pinNumber, void (*onChangeCallback)(char value)) {
    this->_setup();
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

char DummyHeaterController::isOn() {
    return this->value;
};

void DummyHeaterController::setOnValue(char value) {
    if (this->value != value) {
        this->value = value;
        digitalWrite(this->pinNumber, value);
    }
};

