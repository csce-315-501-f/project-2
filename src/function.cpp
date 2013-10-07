


bool do_flip(int x, int y, int xdir, int ydir) {
    int piece = board[x][y];
    x += xdir;
    y += ydir;
    vector< pair<int,int> > to_flip;
    bool is_flip = false;
    while (0 <= x && x < ROWS && 0 <= y && y < COLUMNS) {
        if (board[x][y] == !piece) {
            to_flip.push_back(pair<int,int>(x,y));
        }
        else if (!to_flip.empty() && board[x][y] == piece) {
            is_flip = true;
            break;
        }
        x += xdir;
        y += ydir;
    }
    if (is_flip) {
        for (int i = 0; i < to_flip.size(); ++i) {
            board[to_flip[i].first][to_flip[i].second] = !piece;
        }
    }
    return is_flip;

}


bool do_flip_wrapper(int x, int y) {
   return  do_flip(x,y,-1,0) ||
    do_flip(x,y,-1,1) ||
    do_flip(x,y,0,1) ||
    do_flip(x,y,1,1) ||
    do_flip(x,y,1,0) ||
    do_flip(x,y,1,-1) ||
    do_flip(x,y,-1,0) ||
    do_flip(x,y,-1,-1);
}


int main() {
    if (do_flip_wrapper(x,y)) 
        //do something;
        NULL;
}
