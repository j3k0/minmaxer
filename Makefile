CFLAGS=-Wall

all: test

test: test.o minmaxer.o

test.o: test.c

minmaxer.o: minmaxer.c
