#!/bin/bash

./clean.sh

CXXTEST="../../../third-party/cxxtest/";
INCLUDEPATH="../"
MOCKPATH="../../../tests/mocks/"

../../../third-party/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp Switcher.test.cpp

g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -o runner -I$INCLUDEPATH -I$CXXTEST -I$MOCKPATH ../Switcher.cpp ../../../tests/mocks/Arduino.cpp runner.cpp

./runner

gcovr -r .. --html --html-details -o reports/report.html