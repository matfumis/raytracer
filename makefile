CC = gcc-14
CFLAGS = -O3 -Wall -pedantic -fopenmp 

all: main

main: main.o scene.o ppm.o 
	${CC} ${CFLAGS} -o $@ main.o scene.o ppm.o 

scene.o: scene.c scene.h 
	${CC} ${CFLAGS} -c $<

ppm.o: ppm.c ppm.h scene.h
	${CC} ${CFLAGS} -c $<

PHONY: clean

clean:
	rm -f *.o
	rm -f main