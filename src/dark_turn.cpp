#include "board.h"
using namespace std;
/*
 Looks for all the possible moves and chooses one randomly
 */
pair<int,int> Game_board::dark_turn() {
    int flip = 0;
    vector< pair<int, int> > get_dark_moves;
    get_dark_moves = get_moves(BLACK);

    // skip turn if no moves
    if (get_dark_moves.size() <= 0) return pair<int,int>();

    int move = rand() % get_dark_moves.size();
    
    board[get_dark_moves[move].first][get_dark_moves[move].second-1] = BLACK;
    if (do_flip_wrapper(get_dark_moves[move].first,get_dark_moves[move].second, 1)) {
        return get_dark_moves[move];
    }
    else {
        board[get_dark_moves[move].first][get_dark_moves[move].second-1] = EMPTY;
    }
    return pair<int,int>();
}

/*
 Get board state weighting
 */
i3 Game_board::find_best_move(string turn, int depth) {
    // move needs to be odd as to end on the calling player's turn
    if (depth == 0) return i3(get_board_state_weight(turn),pair<int, int>());

    vector< vector<string> > backupBoard = board;
    vector<pair<int, int> > moves;
    pair<int, int> bestMove;
    int bestMoveWeight = -INT_MIN;

    string nextTurn = (turn == WHITE)?BLACK:WHITE;


    moves = get_moves(turn);
    if (moves.empty()) return find_best_move(nextTurn,depth-1);
    // gets list of available moves and finds the move with the max weight
    for (int j = 0; j < moves.size(); j++) {
        if (do_flip_wrapper(moves[j].first,moves[j].second, 1)) {
            // int weight = get_board_state_weight();
            i3 futureMove = find_best_move(nextTurn, depth-1);
            if (futureMove.first > bestMoveWeight) {
                bestMove = moves[j];
                bestMoveWeight = futureMove.first;
                // bestBoardState = board;
            }
            board = backupBoard;
        }
    }
    return i3(bestMoveWeight, bestMove);
}

/*
 Gets the weighting of the current board
 */
int Game_board::get_board_state_weight(string turn) {
    int weight = 0;
    for (int i = 1; i <= 7; ++i){
        for (int j = 1; i <= 7; ++j) {
            if (board[i][j] == turn) {
                weight += board_weights[i][j];
            }
        }
    }
    return weight;
}

void Game_board::init_board_weights() {
    vector< vector<int> > weights(8,vector<int>(8,0));
    board_weights = weights;

    /* Region 1
     *  C3-6
     *  D3-6
     *  E3-6
     *  F3-6
     */
    board_weights[C][3] = R1;
    board_weights[C][4] = R1;
    board_weights[C][5] = R1;
    board_weights[C][6] = R1;
    board_weights[D][3] = R1;
    board_weights[D][4] = R1;
    board_weights[D][5] = R1;
    board_weights[D][6] = R1;
    board_weights[E][3] = R1;
    board_weights[E][4] = R1;
    board_weights[E][5] = R1;
    board_weights[E][6] = R1;
    board_weights[F][3] = R1;
    board_weights[F][4] = R1;
    board_weights[F][5] = R1;
    board_weights[F][6] = R1;

    /* Region 2
     *  B3-6
     *  C2,7
     *  D2,7
     *  E2,7
     *  F2,7
     *  G3-6
     */
    board_weights[B][3] = R2;
    board_weights[B][4] = R2;
    board_weights[B][5] = R2;
    board_weights[B][6] = R2;
    board_weights[C][2] = R2;
    board_weights[C][7] = R2;
    board_weights[D][2] = R2;
    board_weights[D][7] = R2;
    board_weights[E][2] = R2;
    board_weights[E][7] = R2;
    board_weights[F][2] = R2;
    board_weights[F][7] = R2;
    board_weights[G][3] = R2;
    board_weights[G][4] = R2;
    board_weights[G][5] = R2;
    board_weights[G][6] = R2;

    /* Region 3
     *  A3-6
     *  C1,8
     *  D1,8
     *  E1,8
     *  F1,8
     *  H3-6
     */
    board_weights[A][3] = R3;
    board_weights[A][4] = R3;
    board_weights[A][5] = R3;
    board_weights[A][6] = R3;
    board_weights[C][1] = R3;
    board_weights[C][8] = R3;
    board_weights[D][1] = R3;
    board_weights[D][8] = R3;
    board_weights[E][1] = R3;
    board_weights[E][8] = R3;
    board_weights[F][1] = R3;
    board_weights[F][8] = R3;
    board_weights[H][3] = R3;
    board_weights[H][4] = R3;
    board_weights[H][5] = R3;
    board_weights[H][6] = R3;

    /* Region 4
     *  A2,7
     *  B1,2,7,8
     *  G1,2,7,8
     *  H2,7
     */
    board_weights[A][2] = R4;
    board_weights[A][7] = R4;
    board_weights[B][1] = R4;
    board_weights[B][2] = R4;
    board_weights[B][7] = R4;
    board_weights[B][8] = R4;
    board_weights[G][1] = R4;
    board_weights[G][2] = R4;
    board_weights[G][7] = R4;
    board_weights[G][8] = R4;
    board_weights[H][2] = R4;
    board_weights[H][7] = R4;

    /* Region 5
     *  A1,8
     *  H1,8
     */
     board_weights[A][1] = R5;
     board_weights[A][8] = R5;
     board_weights[H][1] = R5;
     board_weights[H][8] = R5;
}
