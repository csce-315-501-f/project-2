#include "board.h"
#include "Graph.h"
#include "GUI.h"
#include "Window.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> 

using namespace Graph_lib;

/*  */
struct Piece : Rectangle {
	vector<int> pos;
	Piece(int c, int r) : Rectangle(Point(c*50,r*50), 50, 50)
	{
		pos.push_back(c);
		pos.push_back(r);
	}
};

/*  */
struct Light : Piece {
	Image img;
	// c = column, r = row
	Light(int c, int r) : Piece(c, r), img(Point(this->point(0).x, this->point(0).y),"reversi_white.jpg") {
	}
};

/*  */
struct Dark : Piece {
	int index;
	Image img;
	Dark(int c, int r) : Piece(c, r), img(Point(this->point(0).x, this->point(0).y),"reversi_black.jpg") {
	}
};

struct Space : Piece {
	Space(int c, int r) : Piece(c, r) {
	set_fill_color(Color::white;
	}
};

struct Possible_moves : Button {
	Rectangle* color_square;
	void* owner;
	int col;
	int row;
	Image img;
	Possible_moves(Point xy, int w, int ht, const string& label, Callback cb, int c, int r), img(Point(this->point(0).x, this->point(0).y),"dot.jpg")
	: Button(xy, w, ht, label, cb)
	{
		color_square = new Rectangle(xy, w, ht);
		col = c;
		row = r;	
	}

	void attach(Graph_lib::Window& win)
	{
		pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
		owner = &win;
	}

	~Possible_moves()
	{
		delete color_square;
	}
};

class Board : Graph_lib::Window {
public:
	Board(Point xy, int r, int c, int b, const string& title, Vector_ref<Light>& lp, Vector_ref<Dark>& dp)
	: Window(xy, (c*b)+150, (r*b)+30, title)
	{
		num_rows = r;
		num_cols = c;
		button_size = b;
		dark_pieces = &dp;
		light_pieces = &lp;

		init();
	}

	class Game_over {};

	Vector_ref<Dark>* dark_pieces;
	Vector_ref<Light>* light_pieces;
	Vector_ref<Possible_moves> moves;

	void init();
	void update();

 	int num_rows;
	int num_cols;
	int button_size;

	Text* caption;

	Vector_ref<Space>* spaces;
	
	void create_buttons();
	void reset();
	void set_caption(const string&);

	void start_turn(Vector_ref<Dark>*);
	void start_turn(Vector_ref<Light>*);
	void wait(double);
	void wait_for_move();
	void end_of_turn();
	void current_move();


	bool is_occupied(vector<int>&);
	bool is_valid(vector<int>&);
	bool game_over();
	bool posssible_moves();

	~Board()
	{
		delete caption;
	}

private:
	//Graph_lib::Button undo_button;
	//Graph_lib::Button redo_button;

	//void undo();
	
	// callback functions
	//static void cb_undo(Address, Address);
	//static void cb_redo(Address, Address);

	static void button_cb(Address, Address addr)
	{
		Space* s = static_cast<Space*>(addr);
		Board* c = static_cast<Board*>(s->owner);
		c->caption->set_label(s->label);
		//c->light_pieces->mv(s->loc.x - c->light_pieces->point(0).x,s->loc.y - c->light_pieces->point(0).y);
		c->update();
		Fl::redraw();
	}
	
	/* This function sends back the coordinates that the button contained !!!! NOT WORKING !!! */	
	static void move_cb(Address, Address addr)
	{
		Possible_moves* pm = static_cast<Possible_moves*>(addr);
		Board* gb = static_cast<Board*>(pm->owner);
		gb->caption->set_label(pm->label);
		//c->light_pieces->mv(s->loc.x - c->light_pieces->point(0).x,s->loc.y - c->light_pieces->point(0).y);
		gb->update();
		Fl::redraw();
	}

		/* This function sends back the undo function !!!! NOT WORKING !!! */	
	static void cb_undo(Address, Address addr)
	{
		//Possible_moves* pm = static_cast<Possible_moves*>(addr);
		Board* gb = static_cast<Board*>(pm->owner);
		//gb->caption->set_label(pm->label);
		//c->light_pieces->mv(s->loc.x - c->light_pieces->point(0).x,s->loc.y - c->light_pieces->point(0).y);
		gb->update();
		Fl::redraw();
	}

		/* This function sends back the redo !!!! NOT WORKING !!! */	
	static void cb_redo(Address, Address addr)
	{
		//Possible_moves* pm = static_cast<Possible_moves*>(addr);
		Board* gb = static_cast<Board*>(pm->owner);
		//gb->caption->set_label(pm->label);
		//c->light_pieces->mv(s->loc.x - c->light_pieces->point(0).x,s->loc.y - c->light_pieces->point(0).y);
		gb->update();
		Fl::redraw();
	}
};

