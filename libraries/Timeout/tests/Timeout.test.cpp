// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include "Timeout.h"

int called = 0;
void callback() {
	called = 1;
}

class TimeoutTest : public CxxTest::TestSuite
{
public:

    void testConstructor(void)
    {
    	int buttonPin = HIGH;
    	Timeout timeout = Timeout(1000, callback);
        TS_ASSERT_EQUALS(called, 0);
    }

    void testStart(void)
    {
    	int buttonPin = HIGH;
    	Timeout timeout = Timeout(1000, callback);
        TS_ASSERT_EQUALS(called, 0);
        timeout.start();
        timeout.loop();
        TS_ASSERT_EQUALS(called, 0);
        addMillis(1500);
        timeout.loop();
        TS_ASSERT_EQUALS(called, 1);
    }

    void testCancel(void)
    {
    	called = 0;
    	int buttonPin = HIGH;
    	Timeout timeout = Timeout(1000, callback);
        TS_ASSERT_EQUALS(called, 0);
        timeout.start();
        timeout.loop();
        TS_ASSERT_EQUALS(called, 0);
        timeout.cancel();
        addMillis(1500);
        timeout.loop();
        TS_ASSERT_EQUALS(called, 0);
    }
};
