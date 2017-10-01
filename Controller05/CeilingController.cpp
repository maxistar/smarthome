#include "CeilingController.h"
#include <EEPROM.h>

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

void onPowerOnW3(CeilingController *c) {
    c->onLedPowerOn(W3_ON_MASK);
};

void onPowerOffW3(CeilingController *c) {
    c->onLedPowerOff(W3_ON_MASK);
};

void onPowerOnW4(CeilingController *c) {
    c->onLedPowerOn(W4_ON_MASK);
};

void onPowerOffW4(CeilingController *c) {
    c->onLedPowerOff(W4_ON_MASK);
};

void onPowerOnW5(CeilingController *c) {
    c->onLedPowerOn(W5_ON_MASK);
};

void onPowerOffW5(CeilingController *c) {
    c->onLedPowerOff(W5_ON_MASK);
};

void onPowerOnW6(CeilingController *c) {
    c->onLedPowerOn(W6_ON_MASK);
};

void onPowerOffW6(CeilingController *c) {
    c->onLedPowerOff(W6_ON_MASK);
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
    

    w1 = new SimpleLedDimmer(this, config->pinW1, &config->modbus[config->modbusW1], onPowerOnW1, onPowerOffW1);
    w2 = new SimpleLedDimmer(this, config->pinW2, &config->modbus[config->modbusW2], onPowerOnW2, onPowerOffW2);
    w3 = new SimpleLedDimmer(this, config->pinW3, &config->modbus[config->modbusW3], onPowerOnW3, onPowerOffW3);
    w4 = new SimpleLedDimmer(this, config->pinW4, &config->modbus[config->modbusW4], onPowerOnW4, onPowerOffW4);
    w5 = new SimpleLedDimmer(this, config->pinW5, &config->modbus[config->modbusW5], onPowerOnW5, onPowerOffW5);
    w6 = new SimpleLedDimmer(this, config->pinW6, &config->modbus[config->modbusW6], onPowerOnW6, onPowerOffW6);

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
    savePresets();
};

/**
 * Restore default presents
 */
void CeilingController::restorePresets() {
  int i;
  for(i=0;i<PRESETS_COUNT;i++) {
    this->presets[i].w1 = EEPROM.read(i*COUNT_LIGHTS);
    this->presets[i].w2 = EEPROM.read(i*COUNT_LIGHTS+1);
    this->presets[i].w3 = EEPROM.read(i*COUNT_LIGHTS+2);
    this->presets[i].w4 = EEPROM.read(i*COUNT_LIGHTS+3);
    this->presets[i].w5 = EEPROM.read(i*COUNT_LIGHTS+4);
    this->presets[i].w6 = EEPROM.read(i*COUNT_LIGHTS+5);
  };
  this->copyPresetsToModbus();
};

void saveValue(byte value, int address) {
  byte stored = EEPROM.read(address);
  if (stored != value) {
    EEPROM.write(address, value);
  }
}

/**
 * Restore default presents
 */
void CeilingController::savePresets() {
  int i;
  byte data;
  for(i=0;i<PRESETS_COUNT;i++) {
    saveValue(this->presets[i].w1, i*COUNT_LIGHTS);
    saveValue(this->presets[i].w2, i*COUNT_LIGHTS+1);
    saveValue(this->presets[i].w3, i*COUNT_LIGHTS+2);
    saveValue(this->presets[i].w4, i*COUNT_LIGHTS+3);
    saveValue(this->presets[i].w5, i*COUNT_LIGHTS+4);
    saveValue(this->presets[i].w6, i*COUNT_LIGHTS+5);
  };
  this->copyPresetsToModbus();
};

void CeilingController::setup() {
  this->restorePresets();
  this->config->modbus[config->modbusPresetNum] = this->currentPresetNum;
  pinMode(this->config->pinPower, OUTPUT);

  w1->setup();
  w2->setup();
  w3->setup();
  w4->setup();
  w5->setup();
  w6->setup();
};

void CeilingController::on() {
    if (this->onValue == HIGH) return;
    this->onValue = HIGH;
    w1->on();
    w2->on();
    w3->on();
    w4->on();
    w5->on();
    w6->on();
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
    w3->off();
    w4->off();
    w5->off();
    w6->off();
    bitWrite(this->config->modbus[this->config->modbusReadWriteRegisters], this->config->modbusOnBit, 0);
    if (onChangeCallback) {
        onChangeCallback(LOW);
    }
};

void CeilingController::nextMode() {
  if (this->currentPresetNum < (PRESETS_COUNT-1)) {
    this->currentPresetNum++;
  } else {
    this->currentPresetNum = 0;
  }
  this->config->modbus[config->modbusPresetNum] = this->currentPresetNum;
  this->copyPresetsToModbus();
}

uint8_t CeilingController::isOn() {
    return this->onValue;
};

uint8_t CeilingController::isOff() {
    return this->onValue == 0;
};

void CeilingController::copyPresetsToModbus() {
  //copy presets to modbus
     this->config->modbus[config->modbusW1] = this->presets[this->currentPresetNum].w1;
     this->config->modbus[config->modbusW2] = this->presets[this->currentPresetNum].w2;
     this->config->modbus[config->modbusW3] = this->presets[this->currentPresetNum].w3;
     this->config->modbus[config->modbusW4] = this->presets[this->currentPresetNum].w4;
     this->config->modbus[config->modbusW5] = this->presets[this->currentPresetNum].w5;
     this->config->modbus[config->modbusW6] = this->presets[this->currentPresetNum].w6;

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

  if (this->config->modbus[this->config->modbusW3] != this->presets[this->currentPresetNum].w3) {
      this->presets[this->currentPresetNum].w3 = this->config->modbus[this->config->modbusW3];
  }

  if (this->config->modbus[this->config->modbusW4] != this->presets[this->currentPresetNum].w4) {
      this->presets[this->currentPresetNum].w4 = this->config->modbus[this->config->modbusW4];
  }

  if (this->config->modbus[this->config->modbusW5] != this->presets[this->currentPresetNum].w5) {
      this->presets[this->currentPresetNum].w5 = this->config->modbus[this->config->modbusW5];
  }

  if (this->config->modbus[this->config->modbusW6] != this->presets[this->currentPresetNum].w6) {
      this->presets[this->currentPresetNum].w6 = this->config->modbus[this->config->modbusW6];
  }

  w1->loop();
  w2->loop();
  w3->loop();
  w4->loop();
  w5->loop();
  w6->loop();

};

void CeilingController::setOn(uint8_t value) {
  if (value) {
    this->on();
  } else {
    this->off();
  }
};
