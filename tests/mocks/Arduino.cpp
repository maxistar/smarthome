
int _pinModes[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int _pinDigitalValues[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double _pinAnalogValues[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

long _time = 0;

void pinMode(int pin, int value) {
	_pinModes[pin] = value;
}

long millis() {
    return _time;
}

int digitalRead(int pin) {
    return _pinDigitalValues[pin];
}

void digitalWrite(int pin, int value) {
    _pinDigitalValues[pin] = value;
}

int analogRead(int pin) {
    return _pinAnalogValues[pin];
}

int getPinMode(int pin) {
	return _pinModes[pin];
}

int setPinValue(int pin, int value) {
	_pinDigitalValues[pin] = value;
}

int setPinValue(int pin, double value) {
	_pinAnalogValues[pin] = value;
}

void setMillis(long time) {
	_time = time;
}

void addMillis(long time) {
	_time = _time + time;
}
