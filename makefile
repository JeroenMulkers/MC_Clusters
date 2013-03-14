SHELL = /bin/bash

DBUG = -D DEBUGGING
COMP = g++-4.6 ${DBUG} -std=c++0x -Wall
OBJS = main.o

main.out: ${OBJS}
	${COMP} $^ -o $@

%.o: %.cpp *.h
	${COMP} -c $<

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.out
