CC=g++

all: board

board.o: board.cpp board.h
	$(CC) -c board.cpp -o board.o

board: board.o
	$(CC) board.o -o board

clean:
	rm -f *.o board
