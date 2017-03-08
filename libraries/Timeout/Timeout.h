#ifndef TIMEOUT_H
#define TIMEOUT_H
#include "Arduino.h"

class Timeout {
private:
  long milliseconds;
  void (*onTimerCallback)();
  long triggerValue;


public:
  Timeout(long, void (*)());
  void start();
  void cancel();
  void loop();  
};

Timeout::Timeout(long value, void (*callback)()) {
	milliseconds = 0;
	triggerValue = 0;
    this->onTimerCallback = callback;
    this->milliseconds = value;
}

void Timeout::cancel() {
    this->triggerValue = 0;
}

void Timeout::start() {
    this->triggerValue = millis() + this->milliseconds;
}

void Timeout::loop() {
    if (this->triggerValue != 0 && this->triggerValue <= millis()) {
        this->triggerValue = 0;
        this->onTimerCallback();
    }
}

#endif
