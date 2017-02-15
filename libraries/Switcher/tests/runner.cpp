/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_SwitcherTest_init = false;
#include "Switcher.test.cpp"

static SwitcherTest suite_SwitcherTest;

static CxxTest::List Tests_SwitcherTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SwitcherTest( "Switcher.test.cpp", 5, "SwitcherTest", suite_SwitcherTest, Tests_SwitcherTest );

static class TestDescription_suite_SwitcherTest_testAddition : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SwitcherTest_testAddition() : CxxTest::RealTestDescription( Tests_SwitcherTest, suiteDescription_SwitcherTest, 8, "testAddition" ) {}
 void runTest() { suite_SwitcherTest.testAddition(); }
} testDescription_suite_SwitcherTest_testAddition;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
