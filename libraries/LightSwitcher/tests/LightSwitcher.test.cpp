// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include "LightSwitcher.h"

int pressed = 0;
int released = 0;
int keepPressed = 0;

void pressCallback(char) {
    pressed = 1;
};

void releaseCallback() {
    released = 1;
};

void keepPressedCallback() {
    keepPressed = 1;
};

class LightSwitcherTest : public CxxTest::TestSuite
{
public:

    void testConstructor(void)
    {
    	int buttonPin = 1;
    	LightSwitcher switcher = LightSwitcher(buttonPin);
    	switcher.setup();
        TS_ASSERT_EQUALS(getPinMode(buttonPin), INPUT_PULLUP);
    }

    void testConstructor2(void)
    {
    	int buttonPin = 1;
    	LightSwitcher switcher = LightSwitcher(buttonPin, pressCallback);
    	switcher.setup();
        TS_ASSERT_EQUALS(getPinMode(buttonPin), INPUT_PULLUP);
    }

    void testConstructor3(void)
    {
    	int buttonPin = 1;
    	LightSwitcher switcher = LightSwitcher(buttonPin, pressCallback, releaseCallback);
    	switcher.setup();
        TS_ASSERT_EQUALS(getPinMode(buttonPin), INPUT_PULLUP);
    }

    void testConstructor4(void)
    {
    	int buttonPin = 1;
    	LightSwitcher switcher = LightSwitcher(buttonPin, pressCallback, releaseCallback, keepPressedCallback);
    	switcher.setup();
        TS_ASSERT_EQUALS(getPinMode(buttonPin), INPUT_PULLUP);
    }


};
