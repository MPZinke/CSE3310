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


#ifndef _USER_
#define _USER_

#include<gtkmm.h>

#include"CARDDISPLAY.h"
#include"CHIP_BOX.h"
#include"CARD_PLAYER.h"

/**
 * @brief GTKMM class to display User.
 */
class USER : public CARD_PLAYER {
public:
    USER(int, std::string, Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_VERTICAL, int=0);
    ~USER();

    // ————————————— GAME FUNCTIONS —————————————
    void remove_cards(std::vector<std::string>);
    void fold();


private:
    // ———————————————— GUI —————————————————
};

#endif
