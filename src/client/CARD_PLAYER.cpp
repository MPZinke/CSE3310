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

#include"CARD_PLAYER.h"


CARD_PLAYER::CARD_PLAYER(int player_number, std::string name, Gtk::Box* parent_box, Gtk::Orientation orientation, int spacing) :
    PLAYER_DISPLAY::PLAYER_DISPLAY(player_number, name, parent_box, orientation, spacing) {
    _cards_and_chip_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    this->pack_start(*_cards_and_chip_box);

    _card_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));

    _cards_and_chip_box->pack_start(*_card_box);
}


CARD_PLAYER::~CARD_PLAYER() {

}


void CARD_PLAYER::assign_cards(std::vector<std::string> card_names) {
    if(card_names.size() == 5) remove_all_cards();
    // validate additional cards == number of free card spots
    else {
        if(card_names.size() > 5) ;  // throw error
        int free_spots = 0;
        for(int x = 0; x < 5; x++) if(!_cards[x]) free_spots++;
        if(free_spots != (int)  card_names.size()) ;  // throw error
    }

    for(int x = 5 - card_names.size(), y = 0; x < 5; x++, y++)
        _cards[x] = new CARDDISPLAY(card_names[y], _card_box);
}


void CARD_PLAYER::remove_all_cards() {
    for(int x = 0; x < 5; x++) {
        if(_cards[x]) {
            delete _cards[x];
            _cards[x] = NULL;
        }
    }
}


void CARD_PLAYER::change_chip_amount(int amount) {
    _chip_box->change_chip_amount(amount);
}


int CARD_PLAYER::chip_amount() {
    return _chip_box->current_value();
}
