#include "CeilingController.h"

void onPowerOnW1(CeilingController *c) {
    c->onLedPowerOn(W1_ON_MASK);
};

void onPowerOffW1(CeilingController *c) {
    c->onLedPowerOff(W1_ON_MASK);
};

void onPowerOnW2(CeilingController *c) {
    c->onLedPowerOn(W2_ON_MASK);
};

void onPowerOffW2(CeilingController *c) {
    c->onLedPowerOff(W2_ON_MASK);
};

void onPowerOnR(CeilingController *c) {
    c->onLedPowerOn(R_ON_MASK);
};

void onPowerOffR(CeilingController *c) {
    c->onLedPowerOff(R_ON_MASK);
};

void onPowerOnG(CeilingController *c) {
    c->onLedPowerOn(G_ON_MASK);
};

void onPowerOffG(CeilingController *c) {
    c->onLedPowerOff(G_ON_MASK);
};

void onPowerOnB(CeilingController *c) {
    c->onLedPowerOn(B_ON_MASK);
};

void onPowerOffB(CeilingController *c) {
    c->onLedPowerOff(B_ON_MASK);
};

CeilingController::CeilingController(CeilingControllerConfig *config) {
    init(config);
};

CeilingController::CeilingController(CeilingControllerConfig *config, void (*onChangeCallback)(byte value)) {
    init(config);
    this->onChangeCallback = onChangeCallback;
};

void CeilingController::init(CeilingControllerConfig *config) {
    this->config = config;
    this->currentPresetNum = 0;
    this->restorePresets();

    w1 = new SimpleLedDimmer(this, config->pinW1, &config->modbus[config->modbusW1], onPowerOnW1, onPowerOffW1);
    w2 = new SimpleLedDimmer(this, config->pinW2, &config->modbus[config->modbusW2], onPowerOnW2, onPowerOffW2);
    r = new SimpleLedDimmer(this, config->pinR, &config->modbus[config->modbusR], onPowerOnR, onPowerOffR);
    g = new SimpleLedDimmer(this, config->pinG, &config->modbus[config->modbusG], onPowerOnG, onPowerOffG);
    b = new SimpleLedDimmer(this, config->pinB, &config->modbus[config->modbusB], onPowerOnB, onPowerOffB);
};

void CeilingController::onLedPowerOn(byte mask) {
    this->powerState = this->powerState | mask;
    if (this->powerState != 0) {
        digitalWrite(this->config->pinPower, HIGH);
    }
};

void CeilingController::onLedPowerOff(byte mask) {
    this->powerState = this->powerState & ~mask;
    //all leds are off
    if (this->powerState == 0) {
        digitalWrite(this->config->pinPower, LOW);
    }
};

/**
 * Restore default presents
 */
void CeilingController::restorePresets() {
  int i;
  for(i=0;i<PRESETS_COUNT;i++) {
    this->presets[i].w1 = 255;
    this->presets[i].w2 = 0;
    this->presets[i].r = 0;
    this->presets[i].g = 0;
    this->presets[i].b = 0;
  };
  this->copyPresetsToModbus();
};

void CeilingController::setup() {
  this->config->modbus[config->modbusPresetNum] = this->currentPresetNum;
  pinMode(this->config->pinPower, OUTPUT);

  w1->setup();
  w2->setup();
  r->setup();
  g->setup();
  b->setup();
};

void CeilingController::on() {
    if (this->onValue == HIGH) return;
    this->onValue = HIGH;
    w1->on();
    w2->on();
    r->on();
    g->on();
    b->on();
    bitWrite(this->config->modbus[this->config->modbusReadWriteRegisters], this->config->modbusOnBit, 1);
    if (onChangeCallback) {
        onChangeCallback(HIGH);
    }
};

void CeilingController::off() {
    if (this->onValue == LOW) return;
    this->onValue = LOW;
    w1->off();
    w2->off();
    r->off();
    g->off();
    b->off();
    bitWrite(this->config->modbus[this->config->modbusReadWriteRegisters], this->config->modbusOnBit, 0);
    if (onChangeCallback) {
        onChangeCallback(LOW);
    }
};

char CeilingController::isOn() {
    return this->onValue;
};

char CeilingController::isOff() {
    return this->onValue == 0;
};

void CeilingController::copyPresetsToModbus() {
  //copy presets to modbus
     this->config->modbus[config->modbusW1] = this->presets[this->currentPresetNum].w1;
     this->config->modbus[config->modbusW2] = this->presets[this->currentPresetNum].w2;
     this->config->modbus[config->modbusR] = this->presets[this->currentPresetNum].r;
     this->config->modbus[config->modbusG] = this->presets[this->currentPresetNum].g;
     this->config->modbus[config->modbusB] = this->presets[this->currentPresetNum].b;
}

void CeilingController::loop() {
  if (this->config->modbus[this->config->modbusPresetNum] != this->currentPresetNum) {
     this->currentPresetNum = this->config->modbus[config->modbusPresetNum];
     this->copyPresetsToModbus();
  }

  if (this->config->modbus[this->config->modbusW1] != this->presets[this->currentPresetNum].w1) {
      this->presets[this->currentPresetNum].w1 = this->config->modbus[this->config->modbusW1];
  }

  if (this->config->modbus[this->config->modbusW2] != this->presets[this->currentPresetNum].w2) {
      this->presets[this->currentPresetNum].w2 = this->config->modbus[this->config->modbusW2];
  }

  if (this->config->modbus[this->config->modbusR] != this->presets[this->currentPresetNum].r) {
      this->presets[this->currentPresetNum].r = this->config->modbus[this->config->modbusR];
  }

  if (this->config->modbus[this->config->modbusG] != this->presets[this->currentPresetNum].g) {
      this->presets[this->currentPresetNum].g = this->config->modbus[this->config->modbusG];
  }

  if (this->config->modbus[this->config->modbusB] != this->presets[this->currentPresetNum].b) {
      this->presets[this->currentPresetNum].b = this->config->modbus[this->config->modbusB];
  }

  char modbusOn = bitRead(this->config->modbus[this->config->modbusReadWriteRegisters], this->config->modbusOnBit);
  if (modbusOn != onValue) {
      if (this->onValue == 1) {
          this->off();
      } else {
          this->on();
      }
  }



  w1->loop();
  w2->loop();
  r->loop();
  g->loop();
  b->loop();


};
