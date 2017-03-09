CC=g++ 
CXXTEST="../../../third-party/cxxtest/";
INCLUDEPATH="../"
MOCKPATH="../../../tests/mocks/"
INCDIRS   = -I${INCLUDEPATH} -I${MOCKPATH} -I${CXXTEST}
CFLAGS=-Wall ${INCDIRS}

main: runner.o Switcher.test.o


gen:
	../../../third-party/cxxtest/bin/cxxtestgen --error-printer -o runner.cpp Switcher.test.cpp
	
build:
	g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -o runner -I${INCLUDEPATH} -I${CXXTEST} -I${MOCKPATH} ../Switcher.cpp ../../../tests/mocks/Arduino.cpp runner.cpp	
	
clean:
	rm -f runner runner.cpp *.gcda *.gcno reports/*.html
