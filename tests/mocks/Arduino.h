#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H


#ifndef NULL
#define NULL __null
#endif

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2


void pinMode(int, int);

int getPinMode(int);

long millis();

int digitalRead(int);

#endif

