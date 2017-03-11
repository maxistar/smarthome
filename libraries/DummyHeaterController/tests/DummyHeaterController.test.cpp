// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include "DummyHeaterController.h"

int changed = 0;
int changeValue = 0;

void changeCallback(char value) {
    changed = 1;
    changeValue = value;
};

class DummyHeaterControllerTest : public CxxTest::TestSuite
{
public:

    void testConstructor(void)
    {
    	int pin = 1;
    	DummyHeaterController heater = DummyHeaterController(pin, changeCallback);
    	heater.setup();
        TS_ASSERT_EQUALS(getPinMode(pin), OUTPUT);
    }

    void testInterval(void)
    {
        	int pin = 1;
        	DummyHeaterController heater = DummyHeaterController(pin, changeCallback);
        	heater.setInterval(6000);
        	heater.setScale(0.5);
        	heater.setup();
        	addMillis(1);
        	TS_ASSERT_EQUALS(getPinMode(pin), OUTPUT);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);
            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);
            addMillis(3000);
            heater.loop();
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);
    }

    void testIntervalOff(void)
    {
    		return; //skip this
        	int pin = 1;
        	DummyHeaterController heater = DummyHeaterController(pin, changeCallback);
        	heater.setInterval(6000);
        	heater.setScale(0.0);
        	heater.setup();
            TS_ASSERT_EQUALS(getPinMode(pin), OUTPUT);

            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);

            addMillis(3000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);

            addMillis(3000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);

            addMillis(3000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 0);
    }

    void testIntervalOn(void)
    {
        	int pin = 1;
        	DummyHeaterController heater = DummyHeaterController(pin, changeCallback);
        	heater.setInterval(6000);
        	heater.setScale(1.0);
        	heater.setup();
            TS_ASSERT_EQUALS(getPinMode(pin), OUTPUT);

            addMillis(1000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);

            addMillis(3000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);

            addMillis(3000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);

            addMillis(3000);
            heater.loop();
            TS_ASSERT_EQUALS(heater.isOn(), 1);
    }

};
