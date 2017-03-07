#!/bin/bash

rm ./runner ./runner.cpp

../third-party/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp MyTestSuite1.h

g++ -o runner -I ../third-party/cxxtest/ runner.cpp
