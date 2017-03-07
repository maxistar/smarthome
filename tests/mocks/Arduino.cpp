int _pinModes[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void pinMode(int pin, int value) {
	_pinModes[pin] = value;
}

long millis() {

}

int digitalRead(int) {

}


int getPinMode(int pin) {
	return _pinModes[pin];
}


