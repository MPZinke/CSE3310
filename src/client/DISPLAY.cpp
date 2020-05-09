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
	bottom_row_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
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

	_fold_button = Gtk::manage(new Gtk::Button("Fold"));
	_fold_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::fold));
	_player_actions_box->pack_start(*_fold_button);

	//Trade shouldn't be available in first round of betting
	_trade_button = Gtk::manage(new Gtk::Button("Trade"));
	_player_actions_box->pack_start(*_trade_button);

	_out_button = Gtk::manage(new Gtk::Button("Out"));
	_out_button->signal_clicked().connect(sigc::mem_fun(*this, &DISPLAY::out));
	_player_actions_box->pack_start(*_out_button);

	// ———— CARD SELECTION ————
	card_select_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
	_player_actions_box->pack_start(*card_select_box);

	select_card_button1 = Gtk::manage(new Gtk::Button("Select Card 1"));
	select_card_button1->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &DISPLAY::select_card), 0));
	card_select_box->pack_start(*select_card_button1);
	select_card_button2 = Gtk::manage(new Gtk::Button("Select Card 2"));
	select_card_button2->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &DISPLAY::select_card), 1));
	card_select_box->pack_start(*select_card_button2);
	select_card_button3 = Gtk::manage(new Gtk::Button("Select Card 3"));
	select_card_button3->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &DISPLAY::select_card), 2));
	card_select_box->pack_start(*select_card_button3);
	select_card_button4 = Gtk::manage(new Gtk::Button("Select Card 4"));
	select_card_button4->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &DISPLAY::select_card), 3));
	card_select_box->pack_start(*select_card_button4);
	select_card_button5 = Gtk::manage(new Gtk::Button("Select Card 5"));
	select_card_button5->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &DISPLAY::select_card), 4));
	card_select_box->pack_start(*select_card_button5);

	// ———— BET AMOUNT ————
	bet_amount = Gtk::manage(new Gtk::Entry());
	_player_actions_box->pack_start(*bet_amount);


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
			inbuffer.write(read_msg_.body(), read_msg_.body_length());
			inbuffer << "\n";
			std::string msg = inbuffer.rdbuf()->str();
			if(msg != ""){
				try{
					msg = msg.substr(1, msg.find_last_of('}'));
					std::cout << "DEBUG " << msg << " END DEBUG" << std::endl;
					nlohmann::json j = nlohmann::json::parse(msg);
					PLAY play = j.get<PLAY>();
					if(play.type == BET)
					{
						// add_money(play);
						_bet_button->show();
						_check_button->show();
						_fold_button->show();
						_out_button->show();
						bet_amount->show();
					}
					else if(play.type == TRADE)
					{
						get_cards(play);
						_trade_button->show();
						_check_button->show();
						_fold_button->show();
						_out_button->show();
						card_select_box->show();
					}
					else if(play.type == OUT)
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

void DISPLAY::get_cards(PLAY play){
	std::vector<Card> cards = play.tradedCards;
	std::vector<std::string> cardNames;
	Card c;
	
	for(int i = 0; i < 5; i++){
		c = cards[i];
		cardNames.push_back(c.toEnglish());
	}

    hide_user_actions();
	user->assign_cards(cardNames);
    user->show_all();
}

void DISPLAY::add_money(PLAY play){
	user->change_chip_amount(abs(play.bet));
}

void DISPLAY::set_initial(PLAY play){
	_player_number = stoi(play.ID);
	get_cards(play);
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
	auto message = nlohmann::json{play};
	std::cout << "Writing message...";
	write(chat_message{message});
	std::cout << "DONE\n";
	hide_user_actions();
}



void DISPLAY::select_card(int card_number)
{
	int total_selected = 0;
	for(int x = 0; x < 5; x++) if(card_selected[x]) total_selected++;
	if(total_selected == 3 && !card_selected[card_number]) return;

	Gtk::Button* buttons[] =	{
									select_card_button1, 
									select_card_button2, 
									select_card_button3,
									select_card_button4,
									select_card_button5
								};
	card_selected[card_number] = !card_selected[card_number];

	if(card_selected[card_number]) buttons[card_number]->override_color(*COLOR_RED);
	else buttons[card_number]->override_color(*COLOR_BLACK);
}


void DISPLAY::bet()
{
	int amount = 0;
	try
	{
		amount = std::stoi(bet_amount->get_text().raw());
	}
	catch(std::exception)
	{
		std::cout << "You can't follow instructions\n";
		amount = 1000;  // idiot tax;
	}
	PLAY play(BET, amount);
	play.ID = std::to_string(_player_number);
	auto message = nlohmann::json{play};
	send_to_server(play);
	bet_amount->hide();
}


void DISPLAY::check()
{
	PLAY play(CHECK, 0);
	play.ID = std::to_string(_player_number);
	auto message = nlohmann::json{play};
	send_to_server(play);
}


void DISPLAY::fold()
{
	PLAY play(FOLD, 0);
	play.ID = std::to_string(_player_number);
	auto message = nlohmann::json{play};
	send_to_server(play);
}

//Quit game
void DISPLAY::out()
{
	PLAY play(OUT, 0);
    play.ID = std::to_string(_player_number);
    send_to_server(play);
}


void DISPLAY::trade()
{
	// get cards
	hide_user_actions();
}


void DISPLAY::hide_user_actions()
{
	_check_button->hide();
	_bet_button->hide();
	_fold_button->hide();
	_out_button->hide();
	_trade_button->hide();

	bet_amount->hide();
	card_select_box->hide();
}
