// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include "Switcher.h"

int changed = 0;
int pressed = 0;
int released = 0;

void changeCallback(char) {
    changed = 1;
};

void pressCallback() {
    pressed = 1;
};

void releaseCallback() {
    released = 1;
};

class SwitcherTest : public CxxTest::TestSuite
{
public:

    void testConstructor(void)
    {
    	int buttonPin = HIGH;
    	Switcher switcher = Switcher(buttonPin, changeCallback);
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

	void testDebouncing(void)
    {
    	int buttonPin = 1;
    	setPinValue(1, HIGH);
    	Switcher switcher = Switcher(buttonPin, changeCallback);
    	switcher.setup();
    	changed = 0;
    	switcher.loop();

    	setPinValue(1, LOW);

    	addMillis(2);
    	switcher.loop();
        TS_ASSERT_EQUALS(changed, 1);

        changed = 0;
        setPinValue(1, HIGH);
    	addMillis(2);
        switcher.loop();
        TS_ASSERT_EQUALS(changed, 0);

        addMillis(500);
        switcher.loop();
        TS_ASSERT_EQUALS(changed, 1);
    }

	void testCallbacks(void)
    {
		pressed = 0;
		changed = 0;
		released = 0;
    	int buttonPin = 1;
    	setPinValue(1, HIGH);
    	Switcher switcher = Switcher(buttonPin);
    	switcher.setChangeCallback(changeCallback);
    	switcher.setPressCallback(pressCallback);
    	switcher.setReleaseCallback(releaseCallback);

    	switcher.setup();
    	changed = 0;
    	switcher.loop();

    	setPinValue(1, LOW);

    	addMillis(2);
    	switcher.loop();
        TS_ASSERT_EQUALS(changed, 1);
        TS_ASSERT_EQUALS(pressed, 1);
        TS_ASSERT_EQUALS(released, 0);

		pressed = 0;
		changed = 0;
		released = 0;

		setPinValue(1, HIGH);
    	addMillis(2);
        switcher.loop();
        TS_ASSERT_EQUALS(changed, 0);
        TS_ASSERT_EQUALS(pressed, 0);
        TS_ASSERT_EQUALS(released, 0);


        addMillis(500);
        switcher.loop();
        TS_ASSERT_EQUALS(changed, 1);
        TS_ASSERT_EQUALS(pressed, 0);
        TS_ASSERT_EQUALS(released, 1);
    }
};
