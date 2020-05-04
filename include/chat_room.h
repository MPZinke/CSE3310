#ifndef chat_room_inc
#define chat_room_inc
//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"

#include "GAME_SERVER.h"

using asio::ip::tcp;

//----------------------------------------------------------------------

class chat_room {
public:
    void join(chat_participant_ptr participant);
    void leave(chat_participant_ptr participant);

    void deliver(const chat_message& msg);
    void message_received(const chat_message& msg);

private:
    GAME_SERVER game;
    std::set<chat_participant_ptr> participants_;
    // CSE3310 (server) maximum number of messages are stored
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

#endif
