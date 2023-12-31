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
    savePresets();
};

/**
 * Restore default presents
 */
void CeilingController::restorePresets() {
  int i;
  for(i=0;i<PRESETS_COUNT;i++) {
    this->presets[i].w1 = EEPROM.read(i*5);
    this->presets[i].w2 = EEPROM.read(i*5+1);
    this->presets[i].r = EEPROM.read(i*5+2);
    this->presets[i].g = EEPROM.read(i*5+3);
    this->presets[i].b = EEPROM.read(i*5+4);
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
    saveValue(this->presets[i].w1, i*5);
    saveValue(this->presets[i].w2, i*5+1);
    saveValue(this->presets[i].r, i*5+2);
    saveValue(this->presets[i].g, i*5+3);
    saveValue(this->presets[i].b, i*5+4);
  };
  this->copyPresetsToModbus();
};

void CeilingController::setup() {
  this->restorePresets();
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

  w1->loop();
  w2->loop();
  r->loop();
  g->loop();
  b->loop();


};

void CeilingController::setOn(uint8_t value) {
	if (value) {
		this->on();
	} else {
		this->off();
	}
};
