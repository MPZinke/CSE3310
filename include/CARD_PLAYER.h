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


#ifndef _CARD_PLAYER_
#define _CARD_PLAYER_

#include<gtkmm.h>

#include"CARDDISPLAY.h"
#include"CHIP_BOX.h"
#include"PLAYER_DISPLAY.h"

class CARD_PLAYER : public PLAYER_DISPLAY {
public:
    CARD_PLAYER(int, std::string, Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_VERTICAL, int=0);
    ~CARD_PLAYER();

    void assign_cards(std::vector<std::string>);

    // ———————————— GETTER FUNCTIONS —————————————
    void change_chip_amount(int);
    int chip_amount();
    Gtk::Box* _cards_and_chip_box;
    Gtk::Box* _card_box;

protected:
    void remove_all_cards();

    CARDDISPLAY* _cards[5] = {NULL};

    CHIP_BOX* _chip_box;

};

#endif
