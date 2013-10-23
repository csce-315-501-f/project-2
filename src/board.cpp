#include "board.h"

using namespace std;

ostream& operator<<(ostream& os, Game_board& gb)
{
    cout << ";  _ _ _ _ _ _ _ _ " << endl;
	for (int i = 0; i <= ROWS; i++) {
		cout << ";" << i+1 << "|";
        gb.print_row(i);
        cout << endl;
	}
	cout << ";  a b c d e f g h " <<endl;;
	return os;
}

void Game_board::init_board()
{
    board [D][3] = WHITE;
    board [D][4] = BLACK;
    board [E][3] = BLACK;
    board [E][4] = WHITE;
}

void Game_board::print_row(int row) {
	for (int i = 0; i <= COLUMNS; i++) {
		if (board[i][row] == "") board[i][row] = EMPTY;
		cout << board[i][row] << "|";
	}
}


void Game_board::save_board_state() {
    // board state is saved after every DARK turn, so that
    // the board can be reset to the last time the player was able to play
    board_states.push(board);
}

bool Game_board::undo() {
    if (board_states.size() <= 0) return false;
    board = board_states.top();
    board_states.pop(); // remove the top because it is the current move
    return true;
}

/*
 Takes a coordinate as an input and checks if it's a valid move 
 If it is, it flips the corresponding pieces
 */
bool Game_board::light_turn(int column, int row) {
    save_board_state();
    if (board[column][row-1] == BLACK || board[column][row-1] == WHITE) 
        return false;
    board[column][row-1] = WHITE;
    int flip = 1;
    if (do_flip_wrapper(column,row, flip)) {
    }
    else {
    	board[column][row-1] = EMPTY;
        return false;
    }
    return true;
}

int Game_board::pieces(string turn) {
    int cnt = 0;
    for (int row = 0; row <= ROWS; row++) {
        for (int column = 0; column <= COLUMNS; column++) {
            if (board[column][row] == turn) ++cnt;
        }
    }
    return cnt;
}

bool Game_board::can_move(string turn) {
    vector<pair<int, int> > moves;
    moves = get_moves(turn);
    return moves.size() > 0;
}

char Game_board::has_won(string turn) {

    // check if no moves are left on the board
    vector< pair<int, int> > my_moves;
    vector< pair<int, int> > their_moves;
    my_moves = get_moves(turn);
    their_moves = get_moves(turn==BLACK?WHITE:BLACK);
        
    // if there are no more moves, return the piece count
    if (my_moves.size() == 0 && their_moves.size() == 0) {
        int black = pieces(BLACK);
        int white = pieces(WHITE);
        if (turn == BLACK)
            if (black > white)
                return 'w';
            else if (black == white)
                return 't';
            else
                return 'l';
        else
            if (white > black)
                return 'w';
            else if (black == white)
                return 't';
            else
                return 'l';
    }
    return 'n';
}

vector<pair<int, int> > Game_board::get_moves(string turn) {
    int flip = 0;
    vector< pair<int, int> > moves;
    for (int row = 0; row <= ROWS; row++) {
        for (int column = 0; column <= COLUMNS; column++) {
            if (board[column][row] != WHITE && board[column][row] != BLACK ) {
                board[column][row] = turn;
                if (do_flip_wrapper(column,row+1, flip))
                    moves.push_back(pair<int,int>(column,row+1));
                board[column][row] = EMPTY;
            }
        }
    }
    return moves;
}

/*
 Flipping mechanism
 */
bool Game_board::do_flip(int x, int y, int xdir, int ydir, int flip) {
	if (board[x][y-1] == WHITE)
	{
	    string piece = board[x][y-1];
	    x += xdir;
	    y = y + ydir - 1;
	    vector< pair<int,int> > to_flip;
	    bool is_flip = false;
	    while (0 <= x && x <= ROWS && 0 <= y && y <= COLUMNS) {
	        if (board[x][y] == BLACK) {
	            to_flip.push_back(pair<int,int>(x,y));
	        }
	        else if (!to_flip.empty() && board[x][y] == WHITE) {
	            is_flip = true;
	            break;
	        }
	        else if (board[x][y] == EMPTY) {
	            break;
	        }
            else if (board[x][y] == WHITE && to_flip.empty()) {
	            break;
            }
	        x += xdir;
	        y += ydir;
	    }
	    if (is_flip && flip == 1) {
	        for (int i = 0; i < to_flip.size(); ++i) {
	            board[to_flip[i].first][to_flip[i].second] = WHITE;
	        }
	    }
	    return is_flip;
	}
	else if (board[x][y-1] == BLACK)
	{
	    string piece = board[x][y-1];
	    x += xdir;
	    y = y + ydir - 1;
	    vector< pair<int,int> > to_flip;
	    bool is_flip = false;
	    while (0 <= x && x <= ROWS && 0 <= y && y <= COLUMNS) {
	        if (board[x][y] == WHITE) {
	            to_flip.push_back(pair<int,int>(x,y));
	        }
	        else if (!to_flip.empty() && board[x][y] == BLACK) {
	            is_flip = true;
	            break;
	        }
            else if (board[x][y] == EMPTY) {
	            break;
            }
            else if (board[x][y] == BLACK && to_flip.empty()) {
	            break;
            }
	        x += xdir;
	        y += ydir;
	    }
	    if (is_flip && flip == 1) {
	        for (int i = 0; i < to_flip.size(); ++i) {
	            board[to_flip[i].first][to_flip[i].second] = BLACK ;
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
            if (board[column][row] == EMPTY && board[column][row] != BLACK) {
                board[column][row] = WHITE;
                if (do_flip_wrapper(column,row+1, flip))
                {
                    get_light_moves.push_back(pair<int,int>(column,row+1));
                    board[column][row] = EMPTY;
                }
                else {
                    board[column][row] = EMPTY;
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
    bool f[8];
    f[0] = do_flip(x,y,-1,0,flip);
    f[1] = do_flip(x,y,-1,1,flip);
    f[2] = do_flip(x,y,-1,-1,flip);
    f[3] = do_flip(x,y,0,1,flip);
    f[4] = do_flip(x,y,0,-1,flip);
    f[5] = do_flip(x,y,1,1,flip);
    f[6] = do_flip(x,y,1,0,flip);
    f[7] = do_flip(x,y,1,-1,flip);
    return f[0] || f[1] || f[2] || f[3] || f[4] || f[5] || f[6] || f[7];
}

void Game_board::change_sides() {
    board [D][3] = BLACK;
    board [D][4] = WHITE;
    board [E][3] = WHITE;
    board [E][4] = BLACK;
}

void Game_board::setdiff(char d) {
    diff = d;
}

int convert(char let) {
    switch (let) {
        case 'A':
            return A;
        case 'B':
            return B;
        case 'C':
            return C;
        case 'D':
            return D;
        case 'E':
            return E;
        case 'F':
            return F;
        case 'G':
            return G;
        case 'H':
            return H;
    }
    return 0;
}


int main () {
    Game_board gb;
    srand(time(0));
    char comm;
    pair<int,int> n;
    while (comm != '@') {
        cin >> comm;
        switch (comm) {
        case 'e':
        case 'm':
        case 'h':
            gb.setdiff(comm);
            break;
        case 'd':
            cout << gb;
            break;
        case 'u':
            cout << (gb.undo()?"G":"B") << endl;
            break;
        case 'w':
            gb.change_sides();
            n = gb.dark_turn();
            cout << n.first << n.second << endl;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
            int i;
            cin >> i;
            if (gb.light_turn(convert(comm),i)) {
                switch (gb.has_won(WHITE)) {
                case 'w':
                    cout << "W" << endl; 
                    return 0;
                case 't':
                    cout << "T" << endl;
                    return 0;
                case 'l':
                    cout << "L" << endl;
                    return 0;
                }
                pair<int,int> m = gb.dark_turn();
                
                //skip dark turn if no moves
                if (m == pair<int,int>()) {
                    cout << "G" << endl;
                    continue;
                }
                cout << m.first << m.second << endl;
                while (!gb.can_move(WHITE)) {
                    m = gb.dark_turn();
                    cout << m.first << m.second << endl;
                    switch (gb.has_won(BLACK)) {
                    case 'w':
                        cout << "L" << endl; 
                        return 0;
                    case 't':
                        cout << "T" << endl;
                        return 0;
                    case 'l':
                        cout << "W" << endl;
                        return 0;
                    }
                }
                cout << "G" << endl;
            }
            else cout << "I" << endl;
        }
    }
	return 0;
}
