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


#ifndef _CLIENT_GUI_DISPLAY_
#define _CLIENT_GUI_DISPLAY_

#include <cstdlib>
#include <deque>
#include <iostream>
#include<gtkmm.h>
#include <thread>


#include "USER.h"
#include "OTHER_PLAYER.h"
#include "CARDDISPLAY.h"
#include "CHIP_BOX.h"
#include "GLOBAL.h"
#include "PLAYER_DISPLAY.h"
#include "asio.hpp"
#include "chat_message.hpp"
#include "json.hpp"
#include "PLAY.h"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

/**
 * @brief Main class of client-side, takes role of both chat client and visual for program.
 */
class DISPLAY: public Gtk::Window {
public:
   public:
	DISPLAY(int player_number, asio::io_context& io_context, const tcp::resolver::results_type& endpoints, std::stringstream& inbuffer);
	~DISPLAY();
	
	std::vector<std::string> get_cards(PLAY play);
	void add_money(PLAY play);
	void set_initial(PLAY play);


	void add_cards_to_player(std::vector<std::string> card_names);
	void clear_all_cards_from_player();
	void remove_cards_from_player(std::vector<std::string> card_names);

	// ————————————— OBJECT CREATION —————————————
	void assign_starting_players_to_all_players_array();
	void assign_new_player_to_all_players_array(int, std::string);

	int _player_number;
	USER* user;
	std::vector<std::string> _user_cards;
private:
	int _total_players = 1;  // default to just you
	PLAYER_DISPLAY* all_players[6] = {};

	std::thread send;

	Gtk::Box* main_box;

	std::stringstream &inbuffer;

	// ———————————— OTHER PLAYERS & POT ————————————

	// —————— GUI ——————
	// ———— TOP ROW ————
	Gtk::Box* top_row_box;
	// —— CELLS ——
	Gtk::Box* top_left_box;
	Gtk::Box* top_center_box;
	Gtk::Box* top_right_box;

	// ———— MIDDLE ROW ————
	Gtk::Box* middle_row;
	// —— CELLS ——
	Gtk::Box* middle_left_box;
	Gtk::Box* pot_box;
	Gtk::Label* pot_label;
	CHIP_BOX* pot;
	Gtk::Box* middle_right_box;

	// ———— BOTTOM ROW ————
	Gtk::Box* bottom_row_box;
	// —— BUTTONS FUNCTIONS ——
	Gtk::Box* _player_actions_box;
	Gtk::Button* _check_button;//check button
	Gtk::Button* _bet_button;//bet button
	Gtk::Button* _raise_button;//raise button
	Gtk::Button* _call_button;//call button
	Gtk::Button* _fold_button;//fold button
	Gtk::Button* _resign_button;
	Gtk::Button* _trade_button;//trade button
	void check();
	void bet();
	int bet_window();
	void raise();
	void call();
	void fold();
	void resign();
	void trade();
	std::vector<Card> trade_window();
	void hide_user_actions();
    void send_to_server(PLAY play);


 public:
	// CSE3310 (client) message is sent to the chat server.
	void write(const chat_message& msg);
	void close();


private:
	void do_connect(const tcp::resolver::results_type& endpoints);
	void do_read_header();
	// CSE3310 (client) message body is received from the server
	void do_read_body();
	void do_write();

private:
	asio::io_context& io_context_;
	tcp::socket socket_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

#endif
