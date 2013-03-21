SHELL = /bin/bash

DBUG = -D DEBUGGING
COMP = g++-4.6 ${DBUG} -std=c++0x -Wall
OBJS = main.o Ran1.o testRandGen.o

main.out: main.o Ran1.o
	${COMP} $^ -o $@

testRandGen.out: testRandGen.o Ran1.o
	${COMP} $^ -o $@

%.o: %.cpp
	${COMP} -c $<

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.out
