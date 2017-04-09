#include "SimpleSwitcher.h"
#include "LightSwitcher.h"
#include "ModbusRtu.h"
#include "Switcher.h"
#include "Dht11Sensor.h"
#include "Ds18b20Sensor.h"
#include "CeilingController.h"
#include "DummyHeaterController.h"
#include "Timeout.h"
#include "timeouts_def.h"
#include "modbus_def.h"

#include <OneWire.h>
#include <EEPROM.h>


// номер пина, к которому подсоединен датчик температуры
#define TEMP_SENSOR_PIN_SHOWER 4 
#define TEMP_SENSOR_PIN_SHOWER_FLOOR 8
#define TEMP_SENSOR_PIN_ENTRY_FLOOR 11
#define SHOWER_DOOR_SWITCH A2
#define SHOWER_LIGHT_SWITCH A3
#define SHOWER_WATER_SENSOR A1
#define KITCHEN_DOOR_SWITCH A0
#define PIR_SENSOR 12
#define RGB_R 6
#define RGB_G 9
#define RGB_B 10
#define W1_PIN 3
#define W2_PIN 5
#define LED_POWER_PIN 13
#define FAN_PIN 7
#define WARM_FLOOR_SHOWER_PIN A5
#define WARM_FLOOR_ENTRY_PIN A4




// адрес ведомого
#define ID   4

#define txControlPin  0 

// массив данных modbus
uint16_t modbus[MODBUS_SIZE];
int8_t state = 0;

Modbus slave(ID, 0, txControlPin);

OneWire ds(TEMP_SENSOR_PIN_ENTRY_FLOOR); // на пине 10 (нужен резистор 4.7 КОм)

//sample temperature sensor on breadboard
uint8_t addr[8] = {0x28,0x96,0xB0,0xAC,0x05,0x00,0x00,0xDC};

uint8_t fanIsOn = LOW;
uint8_t shouldSwitchFan = 0;
uint8_t switchLightOffWhenDoorClosed = true;

void lightOnTimeoutCallback() {
    shouldSwitchFan = true;
}
Timeout lightOnTimeout(LIGHT_TIMEOUT_TO_SWITCH_FAN, lightOnTimeoutCallback);

void turnOffFan() {
    if (fanIsOn == LOW) return;
    fanIsOn = LOW;
    digitalWrite(FAN_PIN, LOW);
    bitWrite(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_FAN, LOW);  
};

void fanWorkTimeoutCallback() {
    turnOffFan();
}

Timeout fanWorkTimeout(FAN_WORK_DURATION, fanWorkTimeoutCallback);

Timeout changeModeTimeout(LONG_PRESS_TIMEOUT, longPressTimeoutCallback);

void turnOnFan() {
    if (fanIsOn == HIGH) return;
    shouldSwitchFan = false;
    fanIsOn = HIGH;
    digitalWrite(FAN_PIN, HIGH);
    bitWrite(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_FAN, HIGH);
    fanWorkTimeout.start();    
};

CeilingControllerConfig ceilingControllerConfig = {
  /*.pinW1 =*/ W1_PIN,
  /*.pinW2 =*/ W2_PIN,
  /*.pinR =*/ RGB_R,
  /*.pinG =*/ RGB_G,
  /*.pinB =*/ RGB_B,
  /*.pinPower =*/ LED_POWER_PIN,
  /*.modbus =*/ modbus,
  /*.modbusReadRegisters =*/ MODBUS_R_REGISTERS,
  /*.modbusReadWriteRegisters =*/ MODBUS_RW_REGISTERS,
  /*.modbusOnBit*/MODBUS_RW_SHOWER_LIGHT,
  /*.modbusPresetNum =*/ MODBUS_LIGHT_PRESET_NUM,
  /*.modbusW1 =*/ MODBUS_LIGHT_W1_VALUE,
  /*.modbusW2 =*/ MODBUS_LIGHT_W2_VALUE,
  /*.modbusR =*/ MODBUS_LIGHT_R_VALUE,
  /*.modbusG =*/ MODBUS_LIGHT_G_VALUE,
  /*.modbusB =*/ MODBUS_LIGHT_B_VALUE
};

void onCeilingLightChange(uint8_t value) {
    if (value == HIGH) {
        if (fanIsOn == LOW) {
            lightOnTimeout.start();
        }
    } else {
        lightOnTimeout.cancel();
        if (shouldSwitchFan && fanIsOn == LOW) {
            turnOnFan(); 
        }
    }
}
CeilingController ceilingController(&ceilingControllerConfig, onCeilingLightChange);

void longPressTimeoutCallback() {
	ceilingController.nextMode();
	modbus[MODBUS_ERRORS_COUNT];
}

void entryFloorChanged(uint8_t value) {
    bitWrite(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_ENTRY_FLOOR_HEATER, value);
}
DummyHeaterController entryFloor(WARM_FLOOR_ENTRY_PIN, entryFloorChanged);

void showerFloorChanged(uint8_t value) {
    bitWrite(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_FLOOR_HEATER, value);  
}
DummyHeaterController showerFloor(WARM_FLOOR_SHOWER_PIN, showerFloorChanged);



//геркон на дверь в кухню 
void sw2Change(uint8_t value) {
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_KITCHEN_DOOR_SENSOR, value);
}
Switcher sw2(KITCHEN_DOOR_SWITCH, sw2Change);

void onLightRelease() {
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_LIGHT_BUTTON, 0);
    changeModeTimeout.cancel();
    modbus[MODBUS_ERRORS_COUNT]++;
}
void onLightLongPress() {
  //code to work with modes
	if (ceilingController.isOn() && !changeModeTimeout.started()) {
		modbus[MODBUS_ERRORS_COUNT]++;
		changeModeTimeout.start();
	}
}
LightSwitcher lightSwitcher(SHOWER_LIGHT_SWITCH, onLightPress, onLightRelease, onLightLongPress);

Timeout lightOffTimeout(LIGHT_TIMEOUT_TO_SWITCH_OFF_LIGHT, lightOffTimeoutCallback);
void onLightPress(uint8_t state) {
    if (state == 1) {
        ceilingController.on();
        lightOffTimeout.cancel();
        //digitalWrite(LED_POWER_PIN, HIGH); 
    } else {
        ceilingController.off();
        //digitalWrite(LED_POWER_PIN, LOW);
    }
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_LIGHT_BUTTON, 1);
}

//геркон на дверь в душ
void sw1Change(uint8_t value) {
    //write to modbus changed state
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_SHOWER_DOOR_SENSOR, value);

    //show lights if door is open
    if (value == 0 && ceilingController.isOff()) {
        ceilingController.on();
        lightSwitcher.setOn(1);
    } else {
        switchLightOffWhenDoorClosed = true;
        lightOffTimeout.start();
    }
}
Switcher sw1(SHOWER_DOOR_SWITCH, sw1Change);


//PIR sensor
void swPirChange(uint8_t value) {
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_SHOWER_PIR_SENSOR, !value);

    //pir worked
    if (!value && ceilingController.isOn()) {
        switchLightOffWhenDoorClosed = false;
    }
}
Switcher swPir(PIR_SENSOR, swPirChange);


void lightOffTimeoutCallback() {
    if (!switchLightOffWhenDoorClosed || swPir.isOn()) return;
    ceilingController.off();
    lightSwitcher.setOn(0);
}


//SHOWER WATER SENSOR
void sw4Change(uint8_t value) {
    bitWrite(modbus[MODBUS_R_REGISTERS], MODBUS_R_LEAK_SENSOR, value);
}
Switcher sw4(SHOWER_WATER_SENSOR, INPUT, sw4Change);

void dht1TemperatureChange(float value) {
    modbus[MODBUS_SHOWER_ROOM_TEMPERATURE] = (uint16_t) value;
}

void dht1HumidityChange(float value) {
    modbus[MODBUS_SHOWER_ROOM_HUMIDITY] = (uint16_t) value;
}
Dht11Sensor dht1(TEMP_SENSOR_PIN_SHOWER, TEMP_READ_INTERVAL, dht1TemperatureChange, dht1HumidityChange);


void dht2TemperatureChange(float value) {
    modbus[MODBUS_SHOWER_ROOM_FLOOR_TEMPERATURE] = (uint16_t) value;
}
Dht11Sensor dht2(TEMP_SENSOR_PIN_SHOWER_FLOOR, TEMP_READ_INTERVAL, dht2TemperatureChange);


void floorTemperatureChange(float value) {
    modbus[MODBUS_ENTRY_FLOOR_TEMPERATURE] = (uint16_t) (value * 100);
}
Ds18b20Sensor ds19b20(&ds, addr, TEMP_READ_INTERVAL, floorTemperatureChange);

void io_poll() {  
    //modbus[MODBUS_ERRORS_COUNT] = slave.getErrCnt();

    showerFloor.setInterval(modbus[MODBUS_SHOWER_FLOOR_INTERVAL]);
    showerFloor.setScale(modbus[MODBUS_SHOWER_FLOOR_SCALE] / 100.0);

    entryFloor.setInterval(modbus[MODBUS_ENTRY_FLOOR_INTERVAL]);
    entryFloor.setScale(modbus[MODBUS_ENTRY_FLOOR_SCALE] / 100.0);

    
    uint8_t isShowerFloorBitOn = bitRead(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_FLOOR_HEATER);
    if (isShowerFloorBitOn != showerFloor.isOn()) {
        showerFloor.setOnValue(isShowerFloorBitOn);
    }

    uint8_t isEntryFloorBitOn = bitRead(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_ENTRY_FLOOR_HEATER);
    if (isEntryFloorBitOn != entryFloor.isOn()) {
        entryFloor.setOnValue(isEntryFloorBitOn);
    }

    uint8_t isFanBitOn = bitRead(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_FAN);
    if (isFanBitOn != fanIsOn) {
       if (isFanBitOn) {
           turnOnFan(); 
       } else {
           turnOffFan();
       }
    }
   
}

void setup() 
{  
  for (int i = 0; i < MODBUS_SIZE; i++) {
      modbus[i] = 0;
  }
  modbus[MODBUS_SHOWER_FLOOR_INTERVAL] = showerFloor.getInterval();
  modbus[MODBUS_SHOWER_FLOOR_SCALE] = 100 * showerFloor.getScale();
  bitWrite(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_FLOOR_HEATER, 1);


  modbus[MODBUS_ENTRY_FLOOR_INTERVAL] = entryFloor.getInterval();
  modbus[MODBUS_ENTRY_FLOOR_SCALE] = 100 * entryFloor.getScale();
  bitWrite(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_ENTRY_FLOOR_HEATER, 1);
  
  dht1.setup();
  lightSwitcher.setup();

  dht2.setup();
  
  sw1.setup();
  sw2.setup();
  swPir.setup();
  sw4.setup();
  //sw5.setup();
  
  ceilingController.setup();
  showerFloor.setup();
  entryFloor.setup();
  
  slave.begin( 19200 );
}
 
void loop() 
{
  
  state = slave.poll(modbus, MODBUS_SIZE);
  
  uint8_t modbusOn = bitRead(modbus[MODBUS_RW_REGISTERS], MODBUS_RW_SHOWER_LIGHT);
  if (modbusOn != ceilingController.isOn()) {
      ceilingController.setOn(modbusOn);
      lightSwitcher.setOn(modbusOn);
  }



  sw1.loop();
  sw2.loop();
  swPir.loop();
  sw4.loop();
  //sw5.loop();
  
  dht1.loop();
  dht2.loop();
  
  ds19b20.loop();
  lightSwitcher.loop();

  ceilingController.loop();
  showerFloor.loop();
  entryFloor.loop();

  lightOnTimeout.loop();
  fanWorkTimeout.loop();
  lightOffTimeout.loop();
  changeModeTimeout.loop();
  
  //обновляем данные в регистрах Modbus и в пользовательской программе
  io_poll();
}
