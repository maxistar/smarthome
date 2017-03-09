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

};
