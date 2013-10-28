#include "Board_gui.h"
#include "time.h"

string int_to_string(int n)
{
	stringstream s;
	s << n;
	return s.str();
}

void Board::init()
{	
	//create_buttons();
	caption = new Text(Point(0,y_max()), "New game");	
	caption->set_font_size(20);
	caption->set_color(Color::black);
	caption->set_font(Graph_lib::Font::times);
	/*
	Light *lp1 = new Light(3,3);
	Dark *dp1 = new Dark(3,4);
	dark_pieces->push_back(dp1);
	light_pieces->push_back(lp1);
	for (int i=0; i < dark_pieces->size(); ++i) {
		attach((*dark_pieces)[i]); 
		attach((*dark_pieces)[i].img);
	}
	for (int i=0; i < light_pieces->size(); ++i) {
		attach((*light_pieces)[i]); //
		attach((*light_pieces)[i].img);
	}
	*/
	
	//update();
	//start_turn(light_pieces);
}


void Board::create_buttons()
{
	string button_name;
	for (int r = 0; r < num_rows; ++r) {
		for (int c = 0; c < num_cols; ++c) {
			button_name = "(" + int_to_string(c);
			button_name += ", ";
			button_name += int_to_string(r) + ")";
			Space* s = new Space(Point(c*button_size,r*button_size),button_size,button_size,button_name,button_cb, c, r);
			s->color_square->set_fill_color(Color::white);
			spaces.push_back(s);
			attach(*s);
			attach(*(s->color_square));
		}
	}
	//undo_button(Point(x_max()-80,10), 70, 20, "Undo", cb_undo);

	//attach(undo_button);
}


void Board::reset()
{
	for (int i=0; i < spaces.size(); ++i) {
		spaces[i].hide();
		spaces[i].color_square->set_fill_color(Color::white);
	}
}

void Board::set_caption(const string& s)
{
	caption->set_label(s);
}

void Board::update()
{ // this function is called at the start of every 'round'
	//reset();
	int r, c;
	int dx, dy;

	/*for (int i=0; i < moves.size(); ++i) {
		int k = (moves[i][1]*8)+moves[i][0];
		spaces[k].show();
		spaces[k].color_square->set_fill_color(Color::dark_green);
	}*/
}

/*  This two function can be called to change the label of the window which shouldn't mess with the server  */
/*void Board::start_turn(Vector_ref<Light> *lp)
{
	caption->set_label("Your Turn turn");
}*/

/*void Board::start_turn(Vector_ref<Dark> *dp)
{
	caption->set_label("AI's turn");
}*/



/*
*********** I believe this is how things should be called when receiveing information from the server *************

char board[10];
char print_board[8][8];

int i = 0;

char* command = "RDISPLAY\n";
write(sockfd,command,strlen(command));
for (int i = 0; i <= 7; i++) {
    read(sockfd,board,sizeof(board)-1);
	for ( int j = 0; j <= 7; j++) {
		print_board[i][j] = board[j];
	}
}

light_pieces->clear();
dark_pieces->clear();
spaces->clear();
possible_moves.clear();

for (int r = 0; r <= 7; r++) {
	for (int c = 0; c <= 7; c++) {
		if (print_board[r][c] == ".") { // add the space pieces to the vector_ref for printing
			Space *s = new Space(r,c);
			spaces->push_back(s);
		}
		else if (print_board[r][c] == "M"){ // Make the buttons for the possible moves and attach the image
			button_name = "(" + int_to_string(c);
			button_name += ", ";
			button_name += int_to_string(r) + ")";
			Possible_moves* pm = new Space(Point(c*button_size,r*button_size),button_size,button_size,button_name,move_cb, c, r);
			pm->color_square->set_fill_color(Color::white);
			possible_moves.push_back(pm);
			attach(*pm);
			attach(*(pm->color_square));
		}
		else if (print_board[r][c] == "B"){ // add the dark pieces to the vector_ref for printing
			Dark *dp = new Dark(r,c);
			dark_pieces->push_back(dp);
		}
		else if (print_board[r][c] == "W"){ // add the light pieces to the vector_ref for printing
			Light *lp = new Light(r,c);
			light_pieces->push_back(lp);
		}
	}
}

// Printout all the pieces
for (int i=0; i < dark_pieces->size(); ++i) {
	attach((*dark_pieces)[i]); 
	attach((*dark_pieces)[i].img);
}

for (int i=0; i < light_pieces->size(); ++i) {
	attach((*light_pieces)[i]); 
	attach((*light_pieces)[i].img);
}

for (int i=0; i < spaces->size(); ++i) {
	attach((*spaces)[i]); 
	attach((*spaces)[i].img);
}

*/

void Board::wait(double d)
{
	time_t now;
	time(&now);
	while (difftime(time(NULL),now) < d);
}


bool Board::is_occupied(vector<int>& p)
{
	for (int i=0; i < light_pieces->size(); ++i) {
		if ((p[0] == (*light_pieces)[i].pos[0]) && (p[1] == (*light_pieces)[i].pos[1])) return true;
	}
	for (int i=0; i < dark_pieces->size(); ++i) {
		if ((p[0] == (*dark_pieces)[i].pos[0]) && (p[1] == (*dark_pieces)[i].pos[1])) return true;
	}
	return false;
}

bool Board::is_valid(vector<int>& p)
{
	if (is_occupied(p)) return false;
	if ((p[0] < 0) || (p[0] > 7) || (p[1] < 0) || (p[1] > 7)) return false;
	return true;
}

int main()
{


	try {
		/*
		*************** I couldn't figure out how to implement the socket but I found this and I think it might be useful ******************/

		//http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
		int sockfd, portno;
	    char buffer[256];
	    struct sockaddr_in serv_addr;
	    int  n;

	    // First call to socket() function 
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) 
	    {
	        perror("ERROR opening socket");
	        exit(1);
	    }
	    // Initialize socket structure 
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    portno = 8888;
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(portno);

        inet_pton(AF_INET,"128.194.138.51",&serv_addr.sin_addr);
	 
	    // Now bind the host address using bind() call.
	    if (connect(sockfd, (struct sockaddr *) &serv_addr,
	                          sizeof(serv_addr)) < 0)
	    {
	         perror("ERROR on connect");
	         exit(1);
	    }
        
	    // Now start listening for the clients, here 
	    // process will go in sleep mode and will wait 
	    // for the incoming connection
	     
        while ( (n = read(sockfd, buffer, sizeof(buffer)-1)) > 0 ) {
            Vector_ref<Light> LP;
            Vector_ref<Dark> DP;
            Board board(Point(0,0), 8, 8, 50, "Gameboard", LP, DP);
            board.setSock(sockfd);
            //command = "D6\n";
            //write(sockfd,command,strlen(command));

        }
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		return 1;
	}
	catch (...) {
		cerr << "unknown exception" << endl;
		return 2;
	}

	return Fl::run();
}

