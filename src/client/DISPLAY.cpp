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
    user = new USER(_player_number, "YOU", bottom_row_box);
    user->override_color(*COLOR_WHITE);
    
    std::vector<std::string> init = {
        "Back","Back","Back","Back","Back"
    };
    user->assign_cards(init);
        
    for(int x = 1; x < _player_number; x++) all_players[x]->display_card_backs();


    send = std::thread([&io_context]() {io_context.run();});  

    main_box->show_all();
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
            std::vector<std::string> commandQueue;
            std::string msg = inbuffer.rdbuf()->str();
            if(msg != ""){
                try{
                    nlohmann::json j = nlohmann::json::parse(msg);
                    PLAY play = j[0].get<PLAY>();
                    std::cout << "DEBUG " << msg << " END DEBUG" << std::endl;
                    switch(play.type){
                        case 0: add_money(play);
                        break;
                        case 1:
                        break;
                        case 2:
                        break;
                        case 3: get_cards(play);
                        break;
                        case 4:
                        break;
                        case 5: set_initial(play);
                        break;
                    }
                    main_box->show_all();
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
    
    user->assign_cards(cardNames);
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
