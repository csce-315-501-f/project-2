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
	populate_board();

}

/*
 Takes a coordinate as an input and checks if it's a valid move 
 If it is it flips the corresponding pieces
 */
void Game_board::light_turn(int column, int row) {
    board[column][row-1] = "O";
    int flip = 1;
    if (do_flip_wrapper(column,row, flip))
    {
    	update_board();
    }
    else {
    	board[column][row-1] = "_";
    	update_board();
        cout << "Invalid Move!" << endl;
    }
}

/*
 Looks for all the possible moves and chooses one randomly
 */
void Game_board::dark_turn() {
    int flip = 0;
    vector< pair<int, int> > get_dark_moves;
    for (int row = 0; row <= ROWS; row++) {
        for (int column = 0; column <= COLUMNS; column++) {
            if (board[column][row] != "O" && board[column][row] != "@" ) {
                board[column][row] = "@";
                if (do_flip_wrapper(column,row+1, flip))
                {
                    get_dark_moves.push_back(pair<int,int>(column,row+1));
                    board[column][row] = "_";
                }
                else {
                    board[column][row] = "_";
                }
            }
        }
    }
    //cout << get_dark_moves.size() << endl;
    int move = rand() % get_dark_moves.size();
    //cout << "Column: " <<get_dark_moves[move].first << " Row: "<<get_dark_moves[move].second << endl;
    board[get_dark_moves[move].first][get_dark_moves[move].second-1] = "@";
    if (do_flip_wrapper(get_dark_moves[move].first,get_dark_moves[move].second, 1))
    {
    	update_board();
    }
    else {
    	board[get_dark_moves[move].first][get_dark_moves[move].second-1] = "_";
    	update_board();
        cout << "Invalid Move!" << endl;
    }
}

/*
 Flipping mechanism
 */
bool Game_board::do_flip(int x, int y, int xdir, int ydir, int flip) {
	if (board[x][y-1] == "O")
	{
	    string piece = board[x][y-1];
	    x += xdir;
	    y = y + ydir - 1;
	    vector< pair<int,int> > to_flip;
	    bool is_flip = false;
	    while (0 <= x && x <= ROWS && 0 <= y && y <= COLUMNS) {
	        if (board[x][y] == "@") {
	            to_flip.push_back(pair<int,int>(x,y));
	        }
	        else if (!to_flip.empty() && board[x][y] == "O") {
	            is_flip = true;
	            break;
	        }
	        else if (board[x][y] == "_") {
	            break;
	        }
            else if (board[x][y] == "O" && to_flip.empty()) {
	            break;
            }
	        x += xdir;
	        y += ydir;
	    }
	    if (is_flip && flip == 1) {
	        for (int i = 0; i < to_flip.size(); ++i) {
	            board[to_flip[i].first][to_flip[i].second] = "O";
	        }
	    }
	    return is_flip;
	}
	else if (board[x][y-1] == "@")
	{
	    string piece = board[x][y-1];
	    x += xdir;
	    y = y + ydir - 1;
	    vector< pair<int,int> > to_flip;
	    bool is_flip = false;
	    while (0 <= x && x <= ROWS && 0 <= y && y <= COLUMNS) {
	        if (board[x][y] == "O") {
	            to_flip.push_back(pair<int,int>(x,y));
	        }
	        else if (!to_flip.empty() && board[x][y] == "@") {
	            is_flip = true;
	            break;
	        }
            else if (board[x][y] == "_") {
	            break;
            }
            else if (board[x][y] == "@" && to_flip.empty()) {
	            break;
            }
	        x += xdir;
	        y += ydir;
	    }
	    if (is_flip && flip == 1) {
	        for (int i = 0; i < to_flip.size(); ++i) {
	            board[to_flip[i].first][to_flip[i].second] = "@" ;
	        }
	    }
	    return is_flip;
	}

	else 
		return false;
}

/*
 Looks for all the possible moves and prints them out
 */
void Game_board::available_moves(){
    vector< pair<int, int> > get_light_moves;
    int flip = 0;
    for (int row = 0; row <= ROWS; row++) {
        for (int column = 0; column <= COLUMNS; column++) {
            if (board[column][row] == "_" && board[column][row] != "@") {
                board[column][row] = "O";
                if (do_flip_wrapper(column,row+1, flip))
                {
                    get_light_moves.push_back(pair<int,int>(column,row+1));
                    board[column][row] = "_";
                }
                else {
                    board[column][row] = "_";
                }
            }
        }
    }
    for (int i = 0; i < get_light_moves.size(); ++i) {
        if (get_light_moves[i].first == 0) {
            cout << "Column: a" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 1) {
            cout << "Column: b" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 2) {
            cout << "Column: c" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 3) {
            cout << "Column: d" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 4) {
            cout << "Column: e" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 5) {
            cout << "Column: f" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 6) {
            cout << "Column: g" << " Row: " << get_light_moves[i].second << endl;
        }
        else if (get_light_moves[i].first == 7) {
            cout << "Column: h" << " Row: " << get_light_moves[i].second << endl;
        }
    }
}

/*
 Takes a coordinate and checks all the directions for possible flips
 */
bool Game_board::do_flip_wrapper(int x, int y, int flip) {
   return  do_flip(x,y,-1,0,flip) ||
    do_flip(x,y,-1,1,flip) ||
    do_flip(x,y,0,1,flip) ||
    do_flip(x,y,1,1,flip) ||
    do_flip(x,y,1,0,flip) ||
    do_flip(x,y,1,-1,flip) ||
    do_flip(x,y,-1,0,flip) ||
    do_flip(x,y,-1,-1,flip);
}


int main () {
    Game_board gb;
    gb.available_moves();
    gb.light_turn(E, 3);
    gb.dark_turn();
    gb.available_moves();
    gb.light_turn(C, 5);
    gb.dark_turn();
    //gb.available_moves();
    //gb.light_turn(D, 2);
    //gb.available_moves();
	return 0;
}


