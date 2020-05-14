// 1000845009	Mathew Zinke

/***********************************************************************************************************
*
*	created by: MPZinke
*	on ..
*
*	DESCRIPTION:
*	BUGS:
*	FUTURE:
*
***********************************************************************************************************/

#include"DISPLAY.h"
#include <queue>

DISPLAY::DISPLAY(int player_number, asio::io_context& io_context, const tcp::resolver::results_type& endpoints, std::stringstream& inbuffer) : 
	_player_number{player_number},
	inbuffer{inbuffer},
	io_context_(io_context),
	socket_(io_context) {
	
	do_connect(endpoints);
	main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	add(*main_box);

	set_default_size(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	set_title("Poker++");
	set_position(Gtk::WIN_POS_CENTER);

	main_box->override_background_color(*COLOR_GREEN);


	// ———————————— OTHER PLAYERS & POT ————————————
	// ———— TOP ROW ————
	top_row_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	main_box->pack_start(*top_row_box);
	// —— CELLS ——
	top_left_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	top_row_box->pack_start(*top_left_box );
	top_center_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	top_row_box->pack_start(*top_center_box);
	top_right_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	top_row_box->pack_start(*top_right_box);

	// ———— MIDDLE ROW ————
	middle_row = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	main_box->pack_start(*middle_row);
	// —— CELLS ——
	middle_left_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	middle_row->pack_start(*middle_left_box);
	pot_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	middle_row->pack_start(*pot_box);
	pot_label = Gtk::manage(new Gtk::Label("POT"));
	pot_label->override_color(*COLOR_WHITE);
	pot_box->pack_start(*pot_label);
	pot = new CHIP_BOX(pot_box);
	middle_right_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 50));
	middle_row->pack_start(*middle_right_box);

	// ———— SETUP PRE-EXISTING PLAYERS AND DEALER ————
	assign_starting_players_to_all_players_array();
	// dealer
	all_players[0]->override_background_color(*COLOR_RED);
	all_players[0]->name_label()->override_color(*COLOR_WHITE);

	// ———————————————— PLAYER ————————————————
	// ———— BOTTOM ROW ————
	bottom_row_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	main_box->pack_start(*bottom_row_box);

	_player_actions_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
	bottom_row_box->pack_start(*_player_actions_box);

	// ———— BUTTONS ————
	//Check shouldn't be available after someone has bet
	_check_button = Gtk::manage(new Gtk::Button("Check"));
	_check_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::check));
	_player_actions_box->pack_start(*_check_button);

	//Bet shouldn't be available after someone has bet
	_bet_button = Gtk::manage(new Gtk::Button("Bet"));
	_bet_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::bet));
	_player_actions_box->pack_start(*_bet_button);
	
	//Raise button
	_raise_button = Gtk::manage(new Gtk::Button("Raise"));
	_raise_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::raise));
	_player_actions_box->pack_start(*_raise_button);
	
	//Call button
	_call_button = Gtk::manage(new Gtk::Button("Call"));
	_call_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::call));
	_player_actions_box->pack_start(*_call_button);

	_fold_button = Gtk::manage(new Gtk::Button("Fold"));
	_fold_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::fold));
	_player_actions_box->pack_start(*_fold_button);

	//Trade shouldn't be available in first round of betting
	_trade_button = Gtk::manage(new Gtk::Button("Trade"));
	_trade_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::trade));
	_player_actions_box->pack_start(*_trade_button);

	_resign_button = Gtk::manage(new Gtk::Button("Resign"));
	_resign_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::resign));
	_player_actions_box->pack_start(*_resign_button);

	// ———— CARD CHIP STUFF ————
	user = new USER(_player_number, "YOU", bottom_row_box);
	user->override_color(*COLOR_WHITE);
	std::vector<std::string> init = {"Back","Back","Back","Back","Back"};
	user->assign_cards(init);	
	for(int x = 1; x < _player_number; x++) all_players[x]->display_card_backs();

	send = std::thread([&io_context]() {io_context.run();});  

	main_box->show_all();
	// hide_user_actions();
}


DISPLAY::~DISPLAY() {
	for(int x = 0; x < 6; x++) if(all_players[x]) delete all_players[x];
}
   
// new player joined game after display created; create dependencies & add to players array
void DISPLAY::assign_new_player_to_all_players_array(int player_number, std::string player_name) {
	_total_players++;

	// holds box pointers in order
	Gtk::Box* box_array[5] =	{
		middle_left_box, top_left_box, top_center_box,
		top_right_box, middle_right_box
	};
	Gtk::Box* desire_box = box_array[player_number - _player_number];
	all_players[player_number] = new OTHER_PLAYER(player_number, player_name, desire_box);
}


// for easy iteration store boxes in order {dealer, player 1, player 2, player...}
void DISPLAY::assign_starting_players_to_all_players_array() {
	_total_players = _player_number;

	std::string test_names[5] = {"Player 1", "Player 2", "Player 3", "Player 4", "Player 5"};
	// holds box pointers in order
	Gtk::Box* box_array[5] =	{
		middle_left_box, top_left_box, top_center_box,
		top_right_box, middle_right_box
	};

	// assign dealer as a PLAYER
	all_players[0] = new PLAYER_DISPLAY(0, "DEALER", box_array[5 - _player_number]);
	all_players[0]->set_spacing(50);
	// copy in order the pointers of the boxes
	for(int x = 6 - _player_number, y = 1; x < 5; x++, y++) {
		all_players[y] = new OTHER_PLAYER(y, test_names[y-1], box_array[x]);
		all_players[y]->name_label()->override_color(*COLOR_WHITE);
	}
	all_players[_player_number] = NULL;  // constant so NULL put in for good measure

	// create Labels for empty spaces
}

// CSE3310 (client) message is sent to the chat server.
void DISPLAY::write(const chat_message& msg) {
	asio::post(io_context_,
	[this, msg]() {
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress) {
			do_write();
		}
	});
}

void DISPLAY::close() {
	asio::post(io_context_, [this]() {
		socket_.close();
	});
}

void DISPLAY::do_connect(const tcp::resolver::results_type& endpoints) {
	// CSE3310 (client) connection is established with the server
	asio::async_connect(socket_, endpoints,
	[this](std::error_code ec, tcp::endpoint) {
		if (!ec) {
			do_read_header();
		}
	});
}

void DISPLAY::do_read_header() {
	asio::async_read(socket_,
					 asio::buffer(read_msg_.data(), chat_message::header_length),
	[this](std::error_code ec, std::size_t /*length*/) {
		if (!ec && read_msg_.decode_header()) {
			do_read_body();
		} else {
			socket_.close();
		}
	});
}

// CSE3310 (client) message body is received from the server
void DISPLAY::do_read_body() {
	asio::async_read(socket_,
					 asio::buffer(read_msg_.body(), read_msg_.body_length()),
	[this](std::error_code ec, std::size_t /*length*/) {
		if (!ec) {
			if(read_msg_.length() > 0){
				try{
					nlohmann::json j = read_msg_.getJson();
					std::cout << "DEBUG " << j << " END DEBUG" << std::endl;
					PLAY play = j.get<PLAY>();
					if(play.type == BET)
					{
						// add_money(play);
						_bet_button->show();
						_check_button->show();
						_fold_button->show();
						_resign_button->show();
						//REMOVE
						_trade_button->show();
						std::cout << "Played bet\n";
					}
					else if(play.type == TRADE)
					{
						_check_button->show();
						_fold_button->show();
						_resign_button->show();
					}
					else if(play.type == RESIGN)
					{
						// quit game
					}
					else if(play.type == MATCHSTART)
					{
						set_initial(play);
					}
					// main_box->show_all();
				}
				catch(std::exception& e){
					std::cerr << "Exception: " << e.what() << "\n";
				}
			}
			inbuffer.str("");
			do_read_header();
		} else {
			socket_.close();
		}
	});
}

std::vector<std::string> DISPLAY::get_cards(PLAY play){
	std::vector<Card> cards = play.currenthand.getCards();
	std::vector<std::string> cardNames;
	Card c;
	
	for(int i = 0; i < 5; i++){
		c = cards[i];
		cardNames.push_back(c.toEnglish());
	}
	
    hide_user_actions();
	user->assign_cards(cardNames);
    user->show_all();
    return cardNames;
}

void DISPLAY::add_money(PLAY play){
	user->change_chip_amount(abs(play.bet));
}

void DISPLAY::set_initial(PLAY play){
	_player_number = stoi(play.ID);
	_user_cards = get_cards(play);
    user->change_chip_amount(play.bet);

}

void DISPLAY::do_write() {
	asio::async_write(socket_,
					  asio::buffer(write_msgs_.front().data(),
								   write_msgs_.front().length()),
	[this](std::error_code ec, std::size_t /*length*/) {
		if (!ec) {
			write_msgs_.pop_front();
			if (!write_msgs_.empty()) {
				do_write();
			}
		} else {
			socket_.close();
		}
	});
}

void DISPLAY::send_to_server(PLAY play){
	nlohmann::json message = play;
	std::cout << "Writing message...";
	write(chat_message{message});
	std::cout << "DONE\n";
	hide_user_actions();
}


void DISPLAY::bet()
{
    int amount = 0;
    amount = bet_window();
	PLAY play(BET, amount);
	play.ID = std::to_string(_player_number);
	auto message = nlohmann::json{play};
	send_to_server(play);
}

int DISPLAY::bet_window(){
    Gtk::Dialog dialog("Please input your bet", *this);
    
    Gtk::Grid grid;
    
    Gtk::Label l_amount("Bet amount: ");
    Gtk::Entry e_amount;
    grid.attach(l_amount, 0, 0, 1, 1);
    grid.attach(e_amount, 1, 0, 2, 1);
    
    dialog.get_content_area()->add(grid);
    
    dialog.add_button("Place Bet",1);
    dialog.show_all();
    
    while(dialog.run()){
        if(e_amount.get_text().size() == 0){e_amount.set_text("0"); continue;}
        break;
    }
    return std::stoi(e_amount.get_text());
}


void DISPLAY::check()
{
	PLAY play(CHECK, 0);
    play.ID = std::to_string(_player_number);
	auto message = nlohmann::json{play};
    send_to_server(play);
}

void DISPLAY::raise()
{

}

void DISPLAY::call()
{

}

void DISPLAY::fold()
{

}

//Quit game
void DISPLAY::resign()
{
	PLAY play(RESIGN, 0);
    play.ID = std::to_string(_player_number);
    send_to_server(play);
}


void DISPLAY::trade()
{
    std::vector<Card> c = trade_window();
	PLAY play(TRADE);
	play.ID = std::to_string(_player_number);
	play.tradedCards = c;
	play.bet = 0;
	auto message = nlohmann::json{play};
	send_to_server(play);
}

std::vector<Card> DISPLAY::trade_window(){
    Gtk::Dialog dialog("Trade", *this);
    
    Gtk::Grid grid;
    
    Gtk::Label l_amount("Select Cards: ");
    grid.attach(l_amount, 0, 0, 1, 1);
    
    Gtk::CheckButton button1(_user_cards[0]);
    Gtk::CheckButton button2(_user_cards[1]);
    Gtk::CheckButton button3(_user_cards[2]);
    Gtk::CheckButton button4(_user_cards[3]);
    Gtk::CheckButton button5(_user_cards[4]);
    
    grid.attach(button1, 0, 1, 1, 1);
    grid.attach(button2, 0, 2, 1, 1);
    grid.attach(button3, 0, 3, 1, 1);
    grid.attach(button4, 0, 4, 1, 1);
    grid.attach(button5, 0, 5, 1, 1);

    dialog.get_content_area()->add(grid);
    
    dialog.add_button("Confirm",1);
    dialog.show_all();
    
    std::vector<std::string> cardNames;
    std::vector<Card> cards;
	Card c;
	std::vector<std::string> tokens;
	std::string temp;
    while(dialog.run()){
        if(button1.get_active()){cardNames.push_back(_user_cards[0]);}
        if(button2.get_active()){cardNames.push_back(_user_cards[1]);}
        if(button3.get_active()){cardNames.push_back(_user_cards[2]);}
        if(button4.get_active()){cardNames.push_back(_user_cards[3]);}
        if(button5.get_active()){cardNames.push_back(_user_cards[4]);}
        int size = cardNames.size();
	    for(int i = 0; i < size; i++){
	        std::string n = cardNames[i];
	        std::stringstream split(n);
	        while(getline(split, temp, ' ')){tokens.push_back(temp);}
	        std::string r = tokens[0];
	        int cr;
	        if(r == "Jack"){cr = 11;}
	        else if(r == "Queen"){cr = 12;}
	        else if(r == "King"){cr = 13;}
	        else if(r == "Ace"){cr = 1;}
	        else{cr = stoi(r);}
	        c.rank = cr;
    
	        std::string s = tokens[2];
	        int cs;
	        if(s == "Clubs"){cs = 0;}
	        else if(s == "Spades"){cs = 1;}
	        else if(s == "Diamonds"){cs = 2;}
	        else if(s == "Hearts"){cs = 3;}
    
	        c.suit = cs;
	        cards.push_back(c);
	    }
        break;
    }
	return cards;
}

void DISPLAY::hide_user_actions()
{
	_check_button->hide();
	_bet_button->hide();
	_fold_button->hide();
	_resign_button->hide();
	_trade_button->hide();
}
