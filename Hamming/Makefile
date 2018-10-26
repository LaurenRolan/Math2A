CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lm

hamming: hamming.o
	$(CC) -o hamming hamming.o $(LDFLAGS) 

hamming.o: LaurenRolan.c
	$(CC) $(CFLAGS) -o hamming.o -c LaurenRolan.c

clean:
	rm -rf *.o hamming
