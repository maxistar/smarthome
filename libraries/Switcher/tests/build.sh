#!/bin/bash
CXXTEST="../../../third-party/cxxtest/";
INCLUDEPATH="../"
MOCKPATH="../../../tests/mocks/"

../../../third-party/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp Switcher.test.cpp

g++ -o runner -I$INCLUDEPATH -I$CXXTEST -I$MOCKPATH ../Switcher.cpp ../../../tests/mocks/Arduino.cpp runner.cpp
