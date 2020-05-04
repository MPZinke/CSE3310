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

#include<gtkmm.h>

#include"CARDDISPLAY.h"
#include"CHIP_BOX.h"
#include"GLOBAL.h"
#include"OTHER_PLAYER.h"
#include"PLAYER_DISPLAY.h"
#include"USER.h"

class DISPLAY: public Gtk::Window {
public:
    DISPLAY(int player_number, std::stringstream &inbuffer);
    ~DISPLAY();

    void add_cards_to_player(std::vector<std::string> card_names);
    void clear_all_cards_from_player();
    void remove_cards_from_player(std::vector<std::string> card_names);

    // ————————————— OBJECT CREATION —————————————
    void assign_starting_players_to_all_players_array();
    void assign_new_player_to_all_players_array(int, std::string);

    const int _player_number;
    USER* user;
private:
    int _total_players = 1;  // default to just you
    PLAYER_DISPLAY* all_players[6] = {};

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
};

#endif
