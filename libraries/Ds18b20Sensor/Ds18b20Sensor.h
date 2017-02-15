#ifndef DS18B20_SENSOR_H
#define DS18B20_SENSOR_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <OneWire.h>
#define TEMPERATURE_CHECK_TIME 1000


class Ds18b20Sensor {
    private:
    void (*temperatureCallback)(float) = NULL;
    unsigned long lastTempRead = 1; //should be non zero to start process
    unsigned long temperatureReadStartTime = 0;
    unsigned long checkInterval = 0;
    OneWire *ds;
    byte data[12];
    byte *addr;
    float temperature = -1000.0;
    byte type_s;

    
    public:
    Ds18b20Sensor(OneWire *ds, byte* addr, int checkInterval);
    Ds18b20Sensor(OneWire *ds, byte* addr, int checkInterval, void (*changeCallbackPtr)(float));
    void loop();
    void onTemperatureChanged(void (*changeCallbackPtr)(float));
};

Ds18b20Sensor::Ds18b20Sensor(OneWire *_ds, byte* _addr, int _checkInterval) {
    this->ds = _ds;
    this->addr = _addr;
    this->checkInterval = _checkInterval;
    this->lastTempRead = 1;
    this->temperatureReadStartTime = 0;

    switch (addr[0]) {
        case 0x10:
            type_s = 1;
        break;
        case 0x28:
            type_s = 0;
        break;
        case 0x22:
            type_s = 0;
        break;
        default:
            return;
    }    
}

Ds18b20Sensor::Ds18b20Sensor(OneWire *_ds, byte* _addr, int _checkInterval, void (*changeCallbackPtr)(float)) {
    this->ds = _ds;
    this->addr = _addr;
    this->checkInterval = _checkInterval;
    this->lastTempRead = 1;
    this->temperatureReadStartTime = 0;

    switch (addr[0]) {
        case 0x10:
            type_s = 1;
        break;
        case 0x28:
            type_s = 0;
        break;
        case 0x22:
            type_s = 0;
        break;
        default:
            return;
    }    

    temperatureCallback = changeCallbackPtr;
}

void Ds18b20Sensor::onTemperatureChanged(void (*changeCallbackPtr)(float)) {
    temperatureCallback = changeCallbackPtr;
}

void Ds18b20Sensor::loop() {

    int i;
  
    if (temperatureCallback == NULL) {
        return;
    }
    unsigned long currentTime = millis();
    if (!currentTime) return; //pass first iteration

    if (lastTempRead != 0 && currentTime - lastTempRead > checkInterval) {

        temperatureReadStartTime = currentTime; //add 1ms to meke condition tru for first run 
        lastTempRead = 0;

        ds->reset();
        ds->select(addr);
        ds->write(0x44);   
    }
    
    if (temperatureReadStartTime && (currentTime - temperatureReadStartTime) > TEMPERATURE_CHECK_TIME) {

        lastTempRead = currentTime;
        temperatureReadStartTime = 0;
        
        ds->reset();
        ds->select(addr);
        ds->write(0xBE);

        for ( i = 0; i < 9; i++) { // нам необходимо 9 байт
            data[i] = ds->read();
        }
        int16_t raw = (data[1] << 8) | data[0];
        if (type_s) {
            raw = raw << 3; // разрешение 9 бит по умолчанию
           if (data[7] == 0x10) {
               raw = (raw & 0xFFF0) + 12 - data[6];
           }
        } else {
            byte cfg = (data[4] & 0x60);
            // при маленьких значениях, малые биты не определены, давайте их обнулим
            if (cfg == 0x00) raw = raw & ~7; // разрешение 9 бит, 93.75 мс
            else if (cfg == 0x20) raw = raw & ~3; // разрешение 10 бит, 187.5 мс
            else if (cfg ==0x40) raw = raw & ~1; // разрешение 11 бит, 375 мс
            //// разрешение по умолчанию равно 12 бит, время преобразования - 750 мс

        }
        float value = (float)raw / 16.0;
        
        if (temperature != value) {
            temperature = value;
            this->temperatureCallback(value);
        }     
    }
}


#endif

