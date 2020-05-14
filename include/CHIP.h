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


#ifndef _CHIP_
#define _CHIP_

#include<gtkmm.h>

#include"GLOBAL.h"

/**
 * @brief GTKMM class to show chips
 */
class CHIP : public Gtk::Box {
public:
    CHIP(int, std::string);
    CHIP(int, std::string, int);

    void change_chip_count(int);


private:
    void image(std::string, int=75);

    void label();

    int _chip_count;

    Gtk::Label* _chip_count_label;
    Gtk::Image* _image;
};

#endif
