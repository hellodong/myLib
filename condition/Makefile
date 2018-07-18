

CC=gcc 
CFLAGS=
LDFLAGS=-lpthread


all:test

test:condition.o test.o
	$(CC) $^ -o $@ $(LDFLAGS)
condition.o:condition.c condition.h
	$(CC) $< $(CFLAGS) -c
test.o:test.c condition.h
	$(CC) $< $(CFLAGS) -c


.PHONY:clean

clean:
	rm test *.o -rf
