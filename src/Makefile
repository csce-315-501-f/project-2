CC=g++

all: board

board.o: board.cpp board.h
	$(CC) -c board.cpp -o board.o

dark_turn.o: dark_turn.cpp board.h
	$(CC) -c dark_turn.cpp -o dark_turn.o

board: board.o dark_turn.o
	$(CC) board.o dark_turn.o -o board

clean:
	rm -f *.o board
