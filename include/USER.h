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

class USER : public CARD_PLAYER {
public:
    USER(int, std::string, Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_VERTICAL, int=0);
    ~USER();

    // ————————————— GAME FUNCTIONS —————————————
    void remove_cards(std::vector<std::string>);


    // ———————————— DISPLAY FUNCTIONS —————————————
    void hide_user_actions();
    void show_user_actions();
    
    void hide_init_actions();
    void show_init_actions();
    
    void hide_sec_actions();
    void show_sec_actions();

    // ———————————— BUTTONS FUNCTIONS ————————————
    void fold();
    void check();

private:
    // ———————————————— GUI —————————————————
    Gtk::Box* _player_actions_box;
    Gtk::Button* _check_button;//check button
    Gtk::Button* _bet_button;//bet button
    Gtk::Button* _raise_button;//raise button
    Gtk::Button* _call_button;//call button
    Gtk::Button* _fold_button;//fold button
    Gtk::Button* _trade_button;//trade button
};

#endif
