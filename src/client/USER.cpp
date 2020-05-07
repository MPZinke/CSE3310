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

#include"USER.h"


USER::USER(int player_number, std::string name, Gtk::Box* parent_box, Gtk::Orientation orientation, int spacing) :
    CARD_PLAYER(player_number, name, parent_box, orientation, spacing) {
    _chip_box = new CHIP_BOX(_cards_and_chip_box);
}


USER::~USER() {
    delete _chip_box;
}


void USER::remove_cards(std::vector<std::string> cards_to_remove) {
    // delete card object
    for(int x = 0; x < (int) cards_to_remove.size(); x++) {
        for(int y = 0; y < 5; y++) {
            if(_cards[x] && _cards[x]->name() == cards_to_remove[x]) {
                delete _cards[x];
                _cards[x] = NULL;
            }
        }
    }

    // shift cards to beginning
    for(int x = 0; x < 5; x++) {
        if(!_cards[x]) {
            for(int y = x+1; y < 5; y++) {
                if(_cards[y]) {
                    _cards[x] = _cards[y];
                    _cards[y] = NULL;
                    break;
                }
            }
        }
    }
}


// —————————————— PLAY FUNCTIONS ———————————————

void USER::fold() {
    remove_all_cards();
}
