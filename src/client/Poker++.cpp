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
#include<iostream>
#include<thread>
#include<sstream>
#include <string>

#include"DISPLAY.h"
#include"GLOBAL.h"
#include"asio.hpp"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <host> <port> <id>\n";
        return 1;
    }
    try
    {
        std::thread send;

        std::stringstream inbuffer{};

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        std::string winName = "org.gtkmm.poker";
        winName.append(argv[3]);
        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(winName);
        DISPLAY main_window(4, io_context, endpoints, inbuffer);
        app->run(main_window);
        send.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

