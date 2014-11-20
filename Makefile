CFLAGS=-g -Wall -O3
LDFLAGS=-g -O3

all: src/test
	./src/test

src/test: src/test.o src/minmaxer.o

src/test.o: src/test.c

src/minmaxer.o: src/minmaxer.c

clean:
	rm -f src/*.o test
