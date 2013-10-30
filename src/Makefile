CC=g++

all: board 

board.o: board.cpp board.h
	$(CC) -c board.cpp -o board.o

dark_turn.o: dark_turn.cpp board.h
	$(CC) -c dark_turn.cpp -o dark_turn.o

board: board.o dark_turn.o
	$(CC) board.o dark_turn.o -o board

Graph.o: Graph.cpp Graph.h
	$(CC) -c Graph.cpp -o Graph.o

GUI.o: GUI.cpp GUI.h
	$(CC) -c GUI.cpp -o GUI.o

Window.o: Window.cpp Window.h
	$(CC) -c Window.cpp -o Window.o

Board_bui.o: Board_gui.cpp Board_gui.h
	$(CC) Board_gui.cpp -o Board_gui.o

gui: Board_gui.o Graph.o GUI.o Window.o
	$(CC) Board_gui.o Graph.o GUI.o Window.o -lfltk -lfltk_images -o gui


clean:
	rm -f *.o board gui
