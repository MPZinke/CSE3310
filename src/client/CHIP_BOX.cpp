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

#include"CHIP_BOX.h"


CHIP_BOX::CHIP_BOX(Gtk::Box* parent_box, Gtk::Orientation orientation, int chip_size) :
    Gtk::Box(orientation, 0), _parent_box{parent_box} {
    _parent_box->pack_start(*this);

    int blue_chip_amount = _current_chip_value / BLUE_CHIP_VAL;
    int red_chip_amount = (_current_chip_value - (blue_chip_amount * BLUE_CHIP_VAL)) / RED_CHIP_VAL;
    int white_chip_amount = _current_chip_value % RED_CHIP_VAL;

    _blue_chip = new CHIP(blue_chip_amount, "./Images/Chips/Blue.png", chip_size);
    this->pack_start(*_blue_chip, Gtk::PACK_SHRINK, 5);
    _red_chip = new CHIP(red_chip_amount, "./Images/Chips/Red.png", chip_size);
    this->pack_start(*_red_chip, Gtk::PACK_SHRINK, 5);
    _white_chip = new CHIP(white_chip_amount, "./Images/Chips/White.png", chip_size);
    this->pack_start(*_white_chip, Gtk::PACK_SHRINK, 5);
}


CHIP_BOX::CHIP_BOX(int start_chips, Gtk::Box* parent_box, Gtk::Orientation orientation, int chip_size) :
    Gtk::Box(orientation, 0), _current_chip_value{start_chips}, _parent_box{parent_box} {
    _parent_box->pack_start(*this);

    int blue_chip_amount = _current_chip_value / BLUE_CHIP_VAL;
    int red_chip_amount = (_current_chip_value - (blue_chip_amount * BLUE_CHIP_VAL)) / RED_CHIP_VAL;
    int white_chip_amount = _current_chip_value % RED_CHIP_VAL;

    _blue_chip = new CHIP(blue_chip_amount, "./Images/Chips/Blue.png", chip_size);
    this->pack_start(*_blue_chip, Gtk::PACK_SHRINK, 5);
    _red_chip = new CHIP(red_chip_amount, "./Images/Chips/Red.png", chip_size);
    this->pack_start(*_red_chip, Gtk::PACK_SHRINK, 5);
    _white_chip = new CHIP(white_chip_amount, "./Images/Chips/White.png", chip_size);
    this->pack_start(*_white_chip, Gtk::PACK_SHRINK, 5);
}


int CHIP_BOX::current_value() {
    return _current_chip_value;
}

void CHIP_BOX::change_chip_amount(int amount) {
    _current_chip_value = _current_chip_value + amount;

    int blue_chip_amount = _current_chip_value / BLUE_CHIP_VAL;
    int red_chip_amount = (_current_chip_value - (blue_chip_amount * BLUE_CHIP_VAL)) / RED_CHIP_VAL;
    int white_chip_amount = _current_chip_value % RED_CHIP_VAL;

    _white_chip->change_chip_count(white_chip_amount);
    _red_chip->change_chip_count(red_chip_amount);
    _blue_chip->change_chip_count(blue_chip_amount);
}


