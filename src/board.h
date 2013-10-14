#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <stack>

#define ROWS 7
#define COLUMNS 7
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7


#define BLACK "@"
#define WHITE "O"
#define EMPTY "_"

using namespace std;


class Game_board {

public:

	Game_board() {
		vector<string> tempRow(8,EMPTY);
		vector< vector<string> > tempBoard(8,tempRow);
		board = tempBoard;
		init_board();
	}

	// string board[8][8];
	vector< vector<string> > board;
	stack<vector<vector<string> > > board_states;

	bool light_turn(int column, int row);
	pair<int,int> dark_turn();
	bool undo(); // return true if successfull, or false if there is nothing to undo
    vector<pair<int, int> > get_moves(string turn);
    char has_won(string turn); // returns l for loss, t for tie, w for win, and n for game not over

    bool can_move(string turn);
	bool light_can_move(int column, int row);

	string check_input(string s);
    void available_moves();

	void show_help();

	void quit();
	void restart();
	bool game_over();
	
    friend ostream& operator<<(ostream& os, Game_board& gb);

private:
	
	void populate_board();
	void update_board();
	void print_row(int row);
	
	void init_board();

    void save_board_state(); // saves the current board state to board_states stack

    int pieces(string turn);

	vector<pair<int, int> > get_light_moves;

	bool do_flip(int x, int y, int xdir, int ydir, int flip);
	bool do_flip_wrapper(int x, int y, int flip); // flip = 0 means looking for possible moves; 1 flip will happen
    
};
