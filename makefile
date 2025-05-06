CC = gcc

all: main

main: main.o scene.o
	${CC} -o $@ main.o scene.o

%.o: %.c scene.h
	${CC} -c $<

PHONY: clean

clean:
	rm -f *.o
	rm -f main