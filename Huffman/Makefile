CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lm

huffman: huffman.o
	$(CC) -o huffman huffman.o $(LDFLAGS) 

huffman.o: Huffman_initial.c
	$(CC) $(CFLAGS) -o huffman.o -c Huffman_initial.c

clean:
	rm -rf *.o huffman
