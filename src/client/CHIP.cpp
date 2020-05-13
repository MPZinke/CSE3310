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

#include"CHIP.h"


CHIP::CHIP(int chip_count, std::string image_name) :
    Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0), _chip_count{chip_count} {
    label();
    image(image_name);
}


CHIP::CHIP(int chip_count, std::string image_name, int image_size) :
    Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0), _chip_count{chip_count} {
    label();
    image(image_name);
}


void CHIP::image(std::string image_name, int image_size) {
    Glib::RefPtr<Gdk::Pixbuf> tmp = Gdk::Pixbuf::create_from_file(image_name, image_size, image_size, false);
    _image = Gtk::manage(new Gtk::Image{tmp});
    this->pack_start(*_image, Gtk::PACK_SHRINK, 10);
}


void CHIP::label() {
    _chip_count_label = Gtk::manage(new Gtk::Label(std::to_string(_chip_count)));
    _chip_count_label->override_color(*COLOR_WHITE);
    this->pack_start(*_chip_count_label, Gtk::PACK_SHRINK, 10);
}


void CHIP::change_chip_count(int count) {
    _chip_count_label->set_text(std::to_string(count));
}
