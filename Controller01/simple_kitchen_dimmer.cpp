#ifndef SIMPLE_KITCHEN_DIMMER_H
#define SIMPLE_KITCHEN_DIMMER_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "base_dimmer.h"
#include "simple_kitchen_dimmer.h"


SimpleKitchenDimmer::SimpleKitchenDimmer(int buttonPin, int ledPin, int light2Pin){
  this->setButton(buttonPin);
  this->light1_led = ledPin;
  this->light1_on = 0;
  this->light1_max = 255;
  this->light1_state = 0; //0 ready
  this->light1_step = 0; //step to go
  this->light1_stepSetup = 1;
  this->light1_stepLight = 4;
  this->light1_value = 0;
  this->light1_target = 0;
  this->light1_setupMode = 0;
  this->light1_pressedTime = 0;
  this->lasttime = 0;
  
  this->light2Pin = light2Pin;
  this->light2On  = 1;
} 

void SimpleKitchenDimmer::setup(){
  BaseDimmer::setup();
  pinMode(this->light1_led, OUTPUT); 
  
  pinMode(this->light2Pin, OUTPUT); 
  digitalWrite(this->light2Pin, LOW);
} 

void SimpleKitchenDimmer::onButtonPressed() {
    this->light1_pressedTime = millis();
    if (this->light1_on == 1) {
       this->light1_on = 0;
       this->light1_target = 0;
       this->light1_step = -this->light1_stepLight;
       digitalWrite(this->light2Pin, LOW);
       
    }
    else {
       this->light1_on = 1;
       this->light1_target = this->light1_max;
       this->light1_step = this->light1_stepLight;
       if (this->light2On) {
         digitalWrite(this->light2Pin, HIGH);
       }
    }
}

void SimpleKitchenDimmer::onButtonReleased() {
    this->light1_pressedTime = 0;
    if (this->light1_setupMode == 1) {
       this->light1_setupMode = 0;
       this->light1_max = this->light1_value;
       this->light1_step = 0;
    }
}
  
void SimpleKitchenDimmer::doAnimation() {
  if ((this->lasttime + ANIMATION_STEP) > millis()) {
    return; // not enough time has passed
  }
  this->lasttime = millis();
  
  if (this->light1_step == 0) return; //no need to animate
  
   if (this->light1_step > 0) {
       if (this->light1_value + this->light1_step > this->light1_target) {
           this->light1_value = this->light1_target;
           if (this->light1_setupMode == 1) {
               this->light1_step = -this->light1_step;
               this->light1_target = 0;
                              
               if (this->light2On) {
                   this->light2On = 0;
                   digitalWrite(this->light2Pin, LOW);
               }
               else {
                   this->light2On = 1;
                   digitalWrite(this->light2Pin, HIGH);
               }
               
           }
           else {
               this->light1_step = 0; //stop animation
           }
       }
       else {
           this->light1_value += this->light1_step;
       }
       analogWrite(this->light1_led, this->light1_value);
   }
   else {
       if (this->light1_value + this->light1_step < this->light1_target) {
           this->light1_value = this->light1_target;
           if (this->light1_setupMode == 1) {
               this->light1_step = -1*this->light1_step;
               this->light1_target = 255;
               if (this->light2On) {
                   this->light2On = 0;
                   digitalWrite(this->light2Pin, LOW);
               }
               else {
                   this->light2On = 1;
                   digitalWrite(this->light2Pin, HIGH);
               }

           }
           else {
               this->light1_step = 0; //stop animation
           }
       }
       else {
           this->light1_value += this->light1_step;       
       }
       analogWrite(this->light1_led, this->light1_value);
   }
}

void SimpleKitchenDimmer::onButtonKeepsPressed() {
   if (this->light1_setupMode == 1) {
      return; //no need it 
   }
   if (this->light1_pressedTime + LONG_PRESS < millis()) {
       this->light1_on = 1;
       if (this->light2On) {
         digitalWrite(this->light2Pin, HIGH);
       }
       
       this->light1_setupMode = 1;
       if (this->light1_value == 0) {
          this->light1_step = this->light1_stepSetup;
          this->light1_target = 255;
       }
       else {
          this->light1_step = -1*this->light1_stepSetup;
          this->light1_target = 0;
       }
   }  
}

void SimpleKitchenDimmer::loop() {
    BaseDimmer::loop();
    this->doAnimation();  
}

#endif
