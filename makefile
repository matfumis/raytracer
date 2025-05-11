CC = gcc-14
CFLAGS = -O3 -Wall -pedantic -fopenmp 

all: main

main: main.o scene.o ppm.o utils.o
	${CC} ${CFLAGS} -o $@ main.o scene.o ppm.o utils.o

scene.o: scene.c scene.h utils.h
	${CC} ${CFLAGS} -c $<

ppm.o: ppm.c ppm.h scene.h
	${CC} ${CFLAGS} -c $<

utils.o: utils.c utils.h scene.h
	${CC} ${CFLAGS} -c $<

PHONY: clean

clean:
	rm -f *.o
	rm -f main