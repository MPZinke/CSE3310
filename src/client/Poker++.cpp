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

#include"DISPLAY.h"
#include"GLOBAL.h"
#include"chat_client.h"
#include"asio.hpp"

/*int main(int argc, char* argv[]) {
    try {
        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("localhost", "10000");
        chat_client c(io_context, endpoints);

        std::thread t([&io_context]() {
            io_context.run();
        });

        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.poker");
        DISPLAY main_window(4);

        app->run(main_window);

        c.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception " << e.what() << "\n";
    }

    return 0;
}*/
int main(int argc, char* argv[])
{
    try
    {
        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.poker");
        DISPLAY main_window(4);

        app->run(main_window);


        /*if (argc != 3)
        {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }*/

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(/*argv[1]*/ "localhost" , /*argv[2]*/ "10000");
        chat_client c(io_context, endpoints);

        std::thread t([&io_context]() {
            io_context.run();
        });

        char line[chat_message::max_body_length + 1];
        while (std::cin.getline(line, chat_message::max_body_length + 1))
        {
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

