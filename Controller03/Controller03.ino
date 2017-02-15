#include "SimpleSwitcher.h"
#include "SimpleDimmer.h"
#include "ModbusRtu.h"
#include "DHT.h"
#include "Switcher.h"



// адрес ведомого
#define ID   3      
//define stlPin  13  // номер выхода индикатора работы
                    // расположен на плате Arduino
//define ledPin  9  // номер выхода светодиода
#define txControlPin  2 

// номер пина, к которому подсоединен датчик температуры
#define DHTPIN 3 

// temperature read interval
#define TEMP_READ_INTERVAL 20000

/**
 * 1 - 1 word - registers to read
 * 1.0 light switch boys room 
 * 1.1 light switch 2 boys room
 * 1.2 ligth switch girls room
 * 1.3 light switch closet room
 * 1.4 door switch closet room
 * 1.5 door switch boys room
 * 1.6 door switch girls room
 * 
 * 2 - 1 word - registers to write/read
 * 2.0 light 1 boys room
 * 2.1 ligth 2 boys room
 * 2.2 ligth girls room
 * 2.3 ligth closet room
 * 
 * 3 - 3 words
 * 3.1 temperature
 * 3.2 humidity
 * 3.3 count of modbus errors
 * 
 * 4 - 1 words hold registers
 * 4.1 - maximum pwd value for closet light
 * 
 * 
 */

Modbus slave(ID, 0, txControlPin);

// массив данных modbus
uint16_t modbus[6];
int8_t state = 0;

DHT dht(DHTPIN, DHT22);

//детская Мальчиков
SimpleSwitcher sd2(A2, A5, modbus, 0, 0);

//детская Мальчиков 2
SimpleSwitcher sd3(8, 13, modbus, 1, 1);

//детская Ани
SimpleSwitcher sd1(A0, A4, modbus, 2, 2);

//Гардеробная
SimpleDimmer sd4(11, 5, 12, modbus, 3, 3, 5);

void sw1_change(char value) {
  //write to modbus changed state
  bitWrite(modbus[0], 4, !value);

  if (value == 0 && sd4.isOff()) {
      sd4.on();
  }
}

Switcher sw1(10); //геркон на дверь в гардеробную

unsigned long lastTempRead;
 
void setup() 
{
  sd1.setup();
  sd2.setup();
  sd3.setup();
  sd4.setup();

  sw1.setup();
  sw1.setChangeCallback(sw1_change);

  pinMode(10, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);


  dht.begin();
  
  slave.begin( 19200 ); 

  lastTempRead = millis();

}

void io_poll() {
  modbus[4] = slave.getErrCnt();

  bitWrite(modbus[0], 5, digitalRead( A1 ));
  bitWrite(modbus[0], 6, digitalRead( A3 ));
}
 
void loop() 
{
  state = slave.poll(modbus, 6);

  if (millis() - lastTempRead > TEMP_READ_INTERVAL) {
    lastTempRead = millis();

    // Считываем температуру
    float t = dht.readTemperature();
    if (!isnan(t)) {
       modbus[2] = (uint16_t) t*100;
    }
    
    //read humidity
    float h = dht.readHumidity();
    if (!isnan(h)) {
       modbus[3] = (uint16_t) h*100;
    }
  }
    
  sd1.loop();
  sd2.loop();
  sd3.loop();
  sd4.loop();
  sw1.loop();

  //обновляем данные в регистрах Modbus и в пользовательской программе
  io_poll();
}
