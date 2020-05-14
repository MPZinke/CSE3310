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


#ifndef _PLAYER_DISPLAY_
#define _PLAYER_DISPLAY_

#include<gtkmm.h>

/**
 * @brief GTKMM class to show a player.
 */
class PLAYER_DISPLAY : public Gtk::Box {
public:
    PLAYER_DISPLAY(int, std::string, Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_VERTICAL, int=0);
    ~PLAYER_DISPLAY();

    Gtk::Label* name_label();
    void name_label(std::string);

    virtual void display_card_backs() {}

private:
    int _player_number;
    std::string _name;

    Gtk::Label* _name_label;
    Gtk::Box* _parent_box;
};

#endif
