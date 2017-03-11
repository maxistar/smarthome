#!/bin/bash

./clean.sh

CXXTEST="../../../third-party/cxxtest/";
INCLUDEPATH="../"
MOCKPATH="../../../tests/mocks/"

../../../third-party/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp LightSwitcher.test.cpp

g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -o runner -I$INCLUDEPATH -I$CXXTEST -I$MOCKPATH ../LightSwitcher.cpp ../../../tests/mocks/Arduino.cpp runner.cpp

./runner

gcovr -r .. --html --html-details -o report.html