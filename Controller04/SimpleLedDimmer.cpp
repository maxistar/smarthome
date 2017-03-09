#include "SimpleLedDimmer.h"
#include "Arduino.h"


SimpleLedDimmer::SimpleLedDimmer(CeilingController *ceilingController, int ledPin, uint16_t *maxValue) {
  this->ledPin = ledPin;
  this->maxValue = maxValue;
  this->lastTime = millis();
  this->ceilingController = ceilingController;
}

SimpleLedDimmer::SimpleLedDimmer(CeilingController *ceilingController, int ledPin, uint16_t *maxValue, void (*powerOnCallback)(CeilingController *ceilingController), void (*powerOffCallback)(CeilingController *ceilingController)) {
  this->ledPin = ledPin;
  this->maxValue = maxValue;
  this->lastTime = millis();
  this->ceilingController = ceilingController;
  this->powerOnCallback = powerOnCallback;
  this->powerOffCallback = powerOffCallback;
}

void SimpleLedDimmer::setup(){
    pinMode(this->ledPin, OUTPUT);
    analogWrite(this->ledPin, 255);
}

void SimpleLedDimmer::setPowerOnCallback(void (*callback)(CeilingController *ceilingController)) {
    powerOnCallback = callback;
}

void SimpleLedDimmer::setPowerOffCallback(void (*callback)(CeilingController *ceilingController)) {
    powerOffCallback = callback;
}


void SimpleLedDimmer::doAnimation() {
  if ((this->lastTime + ANIMATION_STEP) > millis()) {
    return; // not enough time has passed
  }
  this->lastTime = millis();

  byte target = this->lightOn ? *maxValue : 0;
  if (this->lightValue == target) return; //no need to animate

  if (this->lightValue < target) { //going up
       if (this->lightValue + LIGHT_STEP >= target) {
           this->lightValue = target;
       }
       else {
           this->lightValue += LIGHT_STEP;
       }
       if (this->powerOn == 0) {
          this->powerOn = 1;
          if (this->powerOnCallback != NULL) {
              this->powerOnCallback(ceilingController);
          }
       }
       analogWrite(this->ledPin, 255 - this->lightValue);
   }
   else { //going down
       if (this->lightValue - LIGHT_STEP <= target) {
           this->lightValue = target;
           if (this->lightValue == 0) {
               //power off
               this->powerOn = 0;
               if (this->powerOffCallback != NULL) {
                   this->powerOffCallback(ceilingController);
               }
           }
       }
       else {
           this->lightValue -= LIGHT_STEP;
       }
       analogWrite(this->ledPin, 255 - this->lightValue);
   }
}

void SimpleLedDimmer::loop() {
    this->doAnimation();
}

int SimpleLedDimmer::isOn() {
  return lightOn;
}

int SimpleLedDimmer::isOff() {
  return lightOn == 0;
}


void SimpleLedDimmer::on() {
    if (this->lightOn) return; //already on, nothing to do
    this->lightOn = 1;
}

void SimpleLedDimmer::off() {
    if (this->lightOn == 0) return; //already off, nothing to do
    this->lightOn = 0;
}
