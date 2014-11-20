CFLAGS=-g -Wall -O3
LDFLAGS=-g -O3

all: src/minmaxer-test
	./src/minmaxer-test

src/minmaxer-test: src/minmaxer-test.o src/minmaxer.o

src/minmaxer-test.o: src/minmaxer-test.c

src/minmaxer.o: src/minmaxer.c

clean:
	rm -f src/*.o src/minmaxer-test
