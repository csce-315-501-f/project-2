#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdlib.h>

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

using namespace std;


class Game_board {

public:

	Game_board() {
		init_dark();
		init_light();
		update_board();
	}

	string board[8][8];

	class Invalid_input	{};
	class Invalid_move	{};
	class No_moves		{};
	class User_quit		{};
	class User_undo		{};
	class Game_over		{};

	void light_turn(int column, int row);
	void dark_turn();

	bool light_can_move(int column, int row);
	bool dark_can_move();

	string check_input(string s);

	void show_help();

	void quit();
	void restart();
	bool game_over();
	
private:
	
	void populate_board();
	void update_board();
	void print_row(int row);
	void print_column(int column);
	
	void init_board();
    void init_light();
    void init_dark();

	vector<int> get_light_moves(int index);
	vector<int> get_dark_moves();
	vector<int> check_moves(vector<int> pos);

	int spot_on_board(int row, int column);
	int spot_on_board(vector<int> pos);
	vector<int> spot_on_board(int space);

	bool is_valid(vector<int> pos);
	bool is_occupied(vector<int> pos);
};

ostream& operator<<(ostream& os, Game_board& gb);
string int_to_string(int n);