SHELL = /bin/bash

DBUG = -D DEBUGGING
COMP = g++-4.7 ${DBUG} -std=c++11 -Wall -fopenmp
OBJS = main.o Ran1.o testRandGen.o


main.out: main.o Ran1.o
	${COMP} $^ -o $@

%.o: %.cpp *.h
	${COMP} -c $<

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.out
