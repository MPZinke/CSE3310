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

#include"DISPLAY.h"
#include"GLOBAL.h"
#include"chat_client.h"
#include"asio.hpp"

class clientmanager{
    public:

        //This is where we read from the chat messages
        bool bufferToOut(){
            std::cout << inbuffer.rdbuf();
            return true;
        }

        void gui() { 
            Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("org.gtkmm.poker");
            DISPLAY main_window(4);
            app->run(main_window);
        }

        void start(int argc, char* argv[]){
            
            try
            {
                std::thread client;
                std::thread send;
                std::thread guithread;
                
                this->inbuffer = std::stringstream{};

                asio::io_context io_context;

                tcp::resolver resolver(io_context);
                auto endpoints = resolver.resolve(argv[1], argv[2]);
                chat_client c(io_context, endpoints, inbuffer);

                send = std::thread([&io_context]() {io_context.run();});
                client = std::thread([&] () -> void {        
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
                });
                guithread = std::thread(&clientmanager::gui, this);
                sigc::slot<bool> output = sigc::mem_fun(*this, &clientmanager::bufferToOut);
                auto timeout = Glib::signal_timeout().connect(output, 1);
                send.join();
                client.join();
                guithread.join();
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception: " << e.what() << "\n";
            }
        }

    private:
        std::stringstream inbuffer;
};


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return 1;
    }
    clientmanager mngr{};
    mngr.start(argc, argv);
    return 0;
}

