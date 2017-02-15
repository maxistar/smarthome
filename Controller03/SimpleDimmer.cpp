#ifndef SIMPLE_DIMMER_H
#define SIMPLE_DIMMER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "SimpleDimmer.h"

#define DEBOUNCE 50
#define ANIMATION_STEP 25
#define LONG_PRESS 5000

SimpleDimmer::SimpleDimmer(int buttonPin, int ledPin, int powerPin, uint16_t *modbus, int buttonBit, int stateBit, int maxValueIndex) {
  this->buttonPin = buttonPin;
  this->debounceLastTime = 0;
  this->isDebouncing = 0;
  
  // here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
  this->ledPin = ledPin;
  this->lightOn = 0;
  this->lightMax = 255;
  this->lightStep = 0; //step to go
  this->lightStepSetup = 1;
  this->lightStepLight = 4;
  this->lightValue = 0;
  this->lightTarget = 0;
  this->lightSetupMode = 0;
  this->lightPressedTime = 0;
  this->lastTime = 0;
  this->powerPin = powerPin;
  this->powerOn = 0;

  this->modbus = modbus;
  this->buttonBit = buttonBit;
  this->stateBit = stateBit;
  this->maxValueIndex = maxValueIndex; 

  modbus[maxValueIndex] = lightMax;
} 

void SimpleDimmer::setup(){
  pinMode(this->ledPin, OUTPUT); 
  pinMode(this->powerPin, OUTPUT); 
  pinMode(this->buttonPin, INPUT);
  digitalWrite(this->buttonPin, HIGH);

} 

void SimpleDimmer::checkSwitches()
{
    if (this->isDebouncing && (this->debounceLastTime + DEBOUNCE) > millis()) {
        return; // not enough time has passed to debounce
    }
    
    char buttonState = digitalRead(this->buttonPin);
    char currentState = !buttonState;   // read the button

    //Сохраняем состояние кнопки в регистр 0.x
    bitWrite(modbus[0], buttonBit, buttonState);
  
    if (currentState != this->pressed) {
        if (currentState == 1) {
            // just pressed
            this->onButtonPressed();
            this->pressed = 1;
        }
        else if (currentState == 0) {
            // just released
            this->onButtonReleased();
            this->pressed = 0;
        }
        this->isDebouncing = 1;
    } 
    else {
        this->isDebouncing = 0; 
    }
    // ok we have waited DEBOUNCE milliseconds, lets reset the timer
    this->debounceLastTime = millis();
}


void SimpleDimmer::onButtonPressed() {
    this->lightPressedTime = millis();
    if (this->lightOn == 1) {
       this->lightOn = 0;
       this->lightTarget = 0;
       this->lightStep = -this->lightStepLight;
    }
    else {
       this->lightOn = 1;
       this->lightTarget = this->lightMax;
       this->lightStep = this->lightStepLight;
    }
    
    //Сохраняем состояние света в регистр 1.x
    bitWrite(modbus[1], stateBit, lightOn);
}

void SimpleDimmer::onButtonReleased() {
    this->lightPressedTime = 0;
    if (this->lightSetupMode == 1) {
       this->lightSetupMode = 0;
       this->lightMax = this->lightValue;
       this->lightStep = 0;
       modbus[maxValueIndex] = lightMax;
    }
}
  
void SimpleDimmer::doAnimation() {
  if ((this->lastTime + ANIMATION_STEP) > millis()) {
    return; // not enough time has passed
  }
  this->lastTime = millis();
  
  if (this->lightStep == 0) return; //no need to animate
  
  if (this->lightStep > 0) { //going up
       if (this->lightValue + this->lightStep > this->lightTarget) {
           this->lightValue = this->lightTarget;
           if (this->lightSetupMode == 1) {
               this->lightStep = -this->lightStep;
               this->lightTarget = 0;
           }
           else {
               this->lightStep = 0; //stop animation
           }
       }
       else {
           this->lightValue += this->lightStep;
       }
       if (this->powerOn == 0) {
          this->powerOn = 1;
          digitalWrite(this->powerPin, HIGH);        
       }
       analogWrite(this->ledPin, this->lightValue);
   }
   else { //going down
       if (this->lightValue + this->lightStep < this->lightTarget) {
           this->lightValue = this->lightTarget;
           if (this->lightSetupMode == 1) {
               this->lightStep = -1*this->lightStep;
               this->lightTarget = 255;
           }
           else {
               this->lightStep = 0; //stop animation
               if (this->lightValue == 0) {
                  //power off
                  this->powerOn = 0;
                  digitalWrite(this->powerPin, LOW);                
               }
           }
       }
       else {
           this->lightValue += this->lightStep;       
       }
       analogWrite(this->ledPin, this->lightValue);
   }
}

void SimpleDimmer::onButtonKeepsPressed() {
   if (this->lightSetupMode == 1) {
      return; //no need it 
   }
   if (this->lightPressedTime + LONG_PRESS < millis()) {
       this->lightSetupMode = 1;
       if (this->lightValue == 0) {
          this->lightStep = this->lightStepSetup;
          this->lightTarget = 255;
       }
       else {
          this->lightStep = -1*this->lightStepSetup;
          this->lightTarget = 0;
       }
   }  
}

void SimpleDimmer::loop() {
    char modbusOn = bitRead(modbus[1], stateBit);
    if (modbusOn != lightOn) {
        if (this->lightOn == 1) {
           this->lightOn = 0;
           this->lightTarget = 0;
           this->lightStep = -this->lightStepLight;
        }
        else {
           this->lightOn = 1;
           this->lightTarget = this->lightMax;
           this->lightStep = this->lightStepLight;
        }
    }

    if (modbus[maxValueIndex] != lightMax) {
        lightMax = modbus[maxValueIndex];
        if (this->lightOn) {
            if (this->lightValue > this->lightMax) {
              this->lightStep = -this->lightStepLight;
            }
            else {
              this->lightStep = this->lightStepLight;
            }
            this->lightTarget = this->lightMax;
        }
    }
  
    this->checkSwitches();      // when we check the switches we'll get the current state
 
    if (this->pressed) {
        this->onButtonKeepsPressed();
    }
    this->doAnimation();
}

int SimpleDimmer::isOn() {
  return lightOn;  
}

int SimpleDimmer::isOff() {
  return lightOn == 0;  
}


void SimpleDimmer::on() {
    if (this->lightOn) return; //already on, nothing to do
    
    this->lightOn = 1;
    this->lightTarget = this->lightMax;
    this->lightStep = this->lightStepLight;
    
    //Сохраняем состояние света в регистр 1.x
    bitWrite(modbus[1], stateBit, lightOn);  
}

void SimpleDimmer::off() {
    if (this->lightOn == 0) return; //already off, nothing to do
    
    this->lightOn = 0;
    this->lightTarget = 0;
    this->lightStep = -this->lightStepLight;
    
    //Сохраняем состояние света в регистр 1.x
    bitWrite(modbus[1], stateBit, lightOn);  
}

#endif

