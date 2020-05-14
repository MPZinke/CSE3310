
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

#ifndef _CARDDISPLAY_
#define _CARDDISPLAY_

#include<gtkmm.h>

#include"GLOBAL.h"

/**
 * @brief GTKMM class to show Cards
 */
class CARDDISPLAY {
public:
    CARDDISPLAY(std::string, Gtk::Box*, bool=false);
    CARDDISPLAY(std::string, Gtk::Box*, int, bool=false);
    CARDDISPLAY(std::string, Gtk::Box*, int, int, bool=false);
    ~CARDDISPLAY();

    Gtk::Image* image();
    void image(std::string, bool);
    void image(std::string, int, bool);
    void image(std::string, int, int, bool);

    std::string name();
    void name(std::string);

private:
    std::string _name;
    Gtk::Image* _image;
    Gtk::Box* _parent_box;
};

#endif
