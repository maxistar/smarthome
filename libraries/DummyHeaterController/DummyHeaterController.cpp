#include "Arduino.h"
#include "DummyHeaterController.h"

void DummyHeaterController::_setup() {
	scale = 0.01;
	interval = 60000; //10 minutes
	intervalTime = interval;
	triggerTime = interval * scale;
	pinNumber = 0;
	value = 1;
	onChangeCallback = NULL;
}

DummyHeaterController::DummyHeaterController(int pinNumber) {
    this->_setup();
	this->pinNumber = pinNumber;
};



DummyHeaterController::DummyHeaterController(int pinNumber, void (*onChangeCallback)(uint8_t value)) {
    this->_setup();
	this->pinNumber = pinNumber;
    this->onChangeCallback = onChangeCallback;
};

void DummyHeaterController::setup() {
    //this->intervalTime = millis() + interval;
    pinMode(this->pinNumber, OUTPUT);
    digitalWrite(this->pinNumber, HIGH);
};

void DummyHeaterController::loop() {
	//digitalWrite(this->pinNumber, HIGH);
	//return;
	long currentTime = millis();

    if (this->value == 1 && currentTime > this->triggerTime) {
    	digitalWrite(this->pinNumber, LOW);
    	this->value = 0;
    	if (onChangeCallback != NULL) {
            onChangeCallback(LOW);
        }
    }

    if (currentTime >= this->intervalTime) {
        this->triggerTime = this->intervalTime + this->interval * this->scale;
        this->intervalTime = this->intervalTime + this->interval;

        digitalWrite(this->pinNumber, HIGH);
        this->value = 1;
        if (onChangeCallback != NULL) {
            onChangeCallback(HIGH);
        }
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

long DummyHeaterController::getInterval() {
	return this->interval;
}
float DummyHeaterController::getScale() {
	return this->scale;
}


uint8_t DummyHeaterController::isOn() {
    return this->value;
};

void DummyHeaterController::setOnValue(uint8_t value) {
    if (this->value != value) {
        this->value = value;
        digitalWrite(this->pinNumber, value);
        if (onChangeCallback != NULL) {
            onChangeCallback(value);
        }
    }
};

