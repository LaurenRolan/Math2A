CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lm

huffman: huffman.o
	$(CC) -o huffman huffman.o $(LDFLAGS) 

huffman.o: LaurenRolan.c
	$(CC) $(CFLAGS) -o huffman.o -c LaurenRolan.c

clean:
	rm -rf *.o huffman
