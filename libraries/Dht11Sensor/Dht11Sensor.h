#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "DHT.h"


class Dht11Sensor {
  private:
  DHT *dht;
  float temperature = -1000.0;
  float humidity = -1.0;
  void (*temperatureCallback)(float) = NULL;
  void (*humidityCallback)(float) = NULL;
  unsigned long lastTempRead = 0;
  int checkInterval = 0;
  
  public:
  Dht11Sensor(int gpioPin, int checkInterval);
  Dht11Sensor(int gpioPin, int checkInterval, void (*)(float));
  Dht11Sensor(int gpioPin, int checkInterval, void (*)(float), void (*)(float));
  void loop();
  void setup();
  void onTemperatureChanged(void (*value)(float));
  void onHumidityChanged(void (*value)(float));
};

Dht11Sensor::Dht11Sensor(int gpioPin, int checkInterval) {
  dht = new DHT(gpioPin, DHT11);
  this->checkInterval = checkInterval;
  this->humidityCallback = NULL;
  this->temperatureCallback = NULL;
}

Dht11Sensor::Dht11Sensor(int gpioPin, int checkInterval, void (*temperatureCallback)(float)) {
  dht = new DHT(gpioPin, DHT11);
  this->checkInterval = checkInterval;
  this->humidityCallback = NULL;
  this->temperatureCallback = temperatureCallback;
}

Dht11Sensor::Dht11Sensor(int gpioPin, int checkInterval, void (*temperatureCallback)(float), void (*humidityCallback)(float)) {
  dht = new DHT(gpioPin, DHT11);
  this->checkInterval = checkInterval;
  this->humidityCallback = humidityCallback;
  this->temperatureCallback = temperatureCallback;
}

void Dht11Sensor::onTemperatureChanged(void (*value)(float)) {
   this->temperatureCallback = value;
}

void Dht11Sensor::onHumidityChanged(void (*value)(float)) {
   this->humidityCallback = value;
}

void Dht11Sensor::setup() {
  dht->begin();
}

void Dht11Sensor::loop() {
  
  float value;
  if (millis() - lastTempRead > checkInterval) {
    lastTempRead = millis();

    if (temperatureCallback != NULL) {
        value = dht->readTemperature();
        if (!isnan(value) && temperature != value) {
            temperature = value;  
            this->temperatureCallback(value); 
        }      
    }

    if (humidityCallback != NULL) {
        value = dht->readHumidity();
        if (!isnan(value) && humidity != value) {
            humidity = value;
            this->humidityCallback(value);
        }
    }
  }
  
}

#endif



