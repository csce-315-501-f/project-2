#include "board.h"

using namespace std;

string int_to_string(int n)
{
	stringstream s;
	s << n;
	return s.str();
}

ostream& operator<<(ostream& os, Game_board& gb)
{
	for (int i=0; i < 8; ++i) {
		for (int j=0; j < 8; ++j) os << gb.board[i][j];
		os << endl;
	}
	return os;
}


void Game_board::print_row(int row) {
	for (int i = 0; i <= COLUMNS; i++) {
		if (board[i][row] == "") board[i][row] = "_";
		cout << board[i][row] << "|";
	}

}

void Game_board::populate_board() {
	cout << ";  _ _ _ _ _ _ _ _ " << endl;
	for (int i = 0; i <= ROWS; i++) {
		cout << ";" << i+1 << "|";
        print_row(i);
        cout << endl;
	}
	cout << ";  a b c d e f g h \n" <<endl;;
}

void Game_board::update_board()
{
	//cout << "Updating board...\n";
	populate_board();

}

void Game_board::light_turn(int column, int row) {
    // Check if valid move
    board[column][row-1] = "O";
    update_board();
}

void Game_board::dark_turn() {
    // Check possible moves
    //
    
    update_board();
}

void Game_board::init_dark()
{
    board [D][3] = "O";
    board [E][4] = "O";
}

void Game_board::init_light()
{
    board [D][4] = "@";
    board [E][3] = "@";
}

int main () {
    Game_board gb;
    gb.light_turn(5, 4);
	return 0;
}


