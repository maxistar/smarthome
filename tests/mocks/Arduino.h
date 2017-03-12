#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H


#ifndef NULL
#define NULL __null
#endif

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define HIGH 0x1
#define LOW 0x0

#define TRUE 0x1
#define FALSE 0x0

#define uint8_t unsigned char
#define int8_t char

void pinMode(int, int);

long millis();

int digitalRead(int);
void digitalWrite(int, int);

//testing parts
int getPinMode(int);
int setMillis(long);
int addMillis(long);
int setPinValue(int pin, int value);
int setPinValue(int pin, double value);


#endif

