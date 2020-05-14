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


#ifndef _OTHER_PLAYER_
#define _OTHER_PLAYER_

#include<gtkmm.h>

#include"CARDDISPLAY.h"
#include"CHIP_BOX.h"
#include"CARD_PLAYER.h"

/**
 * @brief GTKMM class for showing the other players.
 */
class OTHER_PLAYER : public CARD_PLAYER {
public:
    OTHER_PLAYER(int, std::string, Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_VERTICAL, int=0);
    ~OTHER_PLAYER();

    void assign_stacked_cards(std::vector<std::string> card_labels);
    void change_chip_amount(int amount);
    void display_card_backs();

private:
    Gtk::Label* _chip_amount_label;

    int _current_chip_amount;
    CHIP_BOX* _chip_box;
};

#endif
