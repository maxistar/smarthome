// MyTestSuite1.h
#include <cxxtest/TestSuite.h>
#include "Switcher.h"

class SwitcherTest : public CxxTest::TestSuite
{
public:
    void testAddition(void)
    {
    	int buttonPin = 1;
    	Switcher switcher = Switcher(buttonPin);
    	//switcher.setup();
        TS_ASSERT(1 + 1 > 1);
        TS_ASSERT_EQUALS(1 + 1, 2);
    }
};
