#ifndef TIMEOUT_H
#define TIMEOUT_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class Timeout {
private:
  long milliseconds = 0;
  void (*onTimerCallback)() = NULL;
  long triggerValue = NULL;


public:
  Timeout(long, void (*)());
  void start();
  void cancel();
  void loop();  
};

Timeout::Timeout(long value, void (*callback)()) {
    this->onTimerCallback = callback;
    this->milliseconds = value;
}

void Timeout::cancel() {
    this->triggerValue = NULL;
}

void Timeout::start() {
    this->triggerValue = millis() + this->milliseconds;
}

void Timeout::loop() {
    if (this->triggerValue != NULL && this->triggerValue >= millis()) {
        this->triggerValue == NULL;
        this->onTimerCallback();
    }
}

#endif
