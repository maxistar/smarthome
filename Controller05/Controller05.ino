#include "ModbusRtu.h"
#include "Switcher.h"
#include "CeilingController.h"
#include "Timeout.h"
#include "LightSwitcher.h"
#include "timeouts_def_prod.h"
#include "modbus_def.h"

#include <OneWire.h>
#include <EEPROM.h>


// номер пина, к которому подсоединен датчик температуры
#define LIGHT_SWITCH A2
#define W1_PIN 3
#define W2_PIN 5
#define W3_PIN 6
#define W4_PIN 9
#define W5_PIN 10
#define W6_PIN 11
#define POWER_PIN 13


// адрес ведомого
#define ID   5

// массив данных modbus
uint16_t modbus[MODBUS_SIZE];
int8_t state = 0;
uint8_t doNotHide = 0;

Modbus slave(ID, 0, txControlPin);



CeilingControllerConfig ceilingControllerConfig = {
  /*.pinW1 =*/ W1_PIN,
  /*.pinW2 =*/ W2_PIN,
  /*.pinW3 =*/ W3_PIN,
  /*.pinW4 =*/ W4_PIN,
  /*.pinW5 =*/ W5_PIN,
  /*.pinW6 =*/ W6_PIN,
  /*.pinPower =*/ POWER_PIN,
  /*.modbus =*/ modbus,
  /*.modbusReadRegisters =*/ MODBUS_R_REGISTERS,
  /*.modbusReadWriteRegisters =*/ MODBUS_RW_REGISTERS,
  /*.modbusOnBit*/MODBUS_RW_LIGHT,
  /*.modbusPresetNum =*/ MODBUS_LIGHT_PRESET_NUM,
  /*.modbusW1 =*/ MODBUS_LIGHT_W1_VALUE,
  /*.modbusW2 =*/ MODBUS_LIGHT_W2_VALUE,
  /*.modbusW3 =*/ MODBUS_LIGHT_W3_VALUE,
  /*.modbusW4 =*/ MODBUS_LIGHT_W4_VALUE,
  /*.modbusW5 =*/ MODBUS_LIGHT_W5_VALUE,
  /*.modbusW6 =*/ MODBUS_LIGHT_W6_VALUE
};

void onCeilingLightChange(uint8_t value) {
  //what we can do here?
}
CeilingController ceilingController(&ceilingControllerConfig, onCeilingLightChange);

void longPressTimeoutCallback() {
	ceilingController.nextMode();
	doNotHide = 1;
}

Timeout changeModeTimeout(LONG_PRESS_TIMEOUT, longPressTimeoutCallback);


void onLightRelease() {
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_LIGHT_BUTTON, 0);
    changeModeTimeout.cancel();
}
void onLightLongPress() {
  //code to work with modes
	if (ceilingController.isOn() && !changeModeTimeout.started()) {
		changeModeTimeout.start();
	}
}
LightSwitcher lightSwitcher(LIGHT_SWITCH, onLightPress, onLightRelease, onLightLongPress);

void onLightPress(uint8_t state) {
    if (state == 1) {
        ceilingController.on();
        doNotHide = 0;
    } else {
    	if (doNotHide == 0) {
    		ceilingController.off();
    	}
    }
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_LIGHT_BUTTON, 1);
}


void io_poll() {  
    modbus[MODBUS_ERRORS_COUNT] = slave.getErrCnt();   
}

void setup() 
{  
  //preset modbus to be all zeros
  for (int i = 0; i < MODBUS_SIZE; i++) {
      modbus[i] = 0;
  }
  lightSwitcher.setup();
  ceilingController.setup();
  slave.begin( 19200 );
}
 
void loop() 
{
  state = slave.poll(modbus, MODBUS_SIZE);
  
  uint8_t modbusOn = bitRead(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_LIGHT);
  if (modbusOn != ceilingController.isOn()) {
      ceilingController.setOn(modbusOn);
      lightSwitcher.setOn(modbusOn);
  }

  lightSwitcher.loop();
  ceilingController.loop(); 
  changeModeTimeout.loop();
  
  //обновляем данные в регистрах Modbus и в пользовательской программе
  io_poll();
}
