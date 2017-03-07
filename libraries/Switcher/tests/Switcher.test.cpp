// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include "Switcher.h"

void someCallback(char) {

}

class SwitcherTest : public CxxTest::TestSuite
{
public:
    void testConstructor(void)
    {
    	//void (*changeCallbackPtr)(char) = someCallback;
    	int buttonPin = 1;
    	Switcher switcher = Switcher(buttonPin, someCallback);
    	switcher.setup();
        TS_ASSERT_EQUALS(getPinMode(buttonPin), INPUT_PULLUP);
    }

	void testSetup(void)
    {
    	int buttonPin = 1;
    	Switcher switcher = Switcher(buttonPin);
    	switcher.setup();
        TS_ASSERT_EQUALS(getPinMode(buttonPin), INPUT_PULLUP);
    }
};
