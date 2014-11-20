CFLAGS=-g -Wall -O3
LDFLAGS=-g -O3

all: test
	./test

test: test.o minmaxer.o

test.o: test.c

minmaxer.o: minmaxer.c

clean:
	rm -f *.o test
