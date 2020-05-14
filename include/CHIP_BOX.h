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


#ifndef _CHIP_BOX_
#define _CHIP_BOX_

#include<gtkmm.h>

#include"CHIP.h"
#include"GLOBAL.h"

/**
 * @brief GTMM class to show all chips and their values
 */
class CHIP_BOX : public Gtk::Box {
public:
    CHIP_BOX(Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_HORIZONTAL, int=75);
    CHIP_BOX(int, Gtk::Box*, Gtk::Orientation=Gtk::ORIENTATION_HORIZONTAL, int=75);

    int current_value();

    void change_chip_amount(int);

    Gtk::Box* parent_box();

private:
    int _current_chip_value=DEFAULT_CHIP_AMOUNT;

    CHIP* _white_chip;
    CHIP* _red_chip;
    CHIP* _blue_chip;

    // Gtk stuff
    Gtk::Box* _parent_box;
};

#endif
