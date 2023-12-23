CC=gcc
OPT=-O3 -std=c99 
INCDIR=-I.
CFLAGS=-Wall -Wextra $(INCDIR) $(OPT)

zeropass: main.o sha512.o utils.o
	$(CC) sha512.o utils.o main.o -o bin/zeropass $(CFLAGS)

main.o: sha512.o utils.o main.c
	$(CC) -c main.c $(CFLAGS)

sha512.o: sha512.c sha512.h 
	$(CC) -c sha512.c $(CFLAGS)

utils.o: utils.c utils.h constant.h
	$(CC) -c utils.c $(CFLAGS)

clean:
	rm -f *.o bin/zeropass

run: zeropass
	bin/zeropass

