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
#include<string>
#include <utility>

#include "asio.hpp"
#include "chat_message.hpp"
#include "chat_participant.h"
#include "chat_room.h"

#include "GAME_SERVER.h"

using asio::ip::tcp;

//----------------------------------------------------------------------

//Add participant to chat_room and send them all of the messages sent so far
void chat_room::join(chat_participant_ptr participant) {
    participants_.insert(participant);
    game.addPlayer(participant, chat_message{nlohmann::json{{"name", ""}, {"id", ""}}});
    // CSE3310 (server)  previous chat messages are sent to a client
    for (auto msg: recent_msgs_)
        participant->deliver(msg);
}

//Remove participant form the chatroom
void chat_room::leave(chat_participant_ptr participant) {
    game.leave(participant);
    participants_.erase(participant);
}

//Deliver a message to everyone in the chatroom
void chat_room::deliver(const chat_message& msg) {
    game.processInput(msg);
    
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();


    // CSE3310 (server)  messages are sent to all connected clients
    for (auto participant: participants_)
        participant->deliver(msg);
    
}


// message received; validate message and send to game to process
void chat_room::message_received(const chat_message& message)
{
    if(!game.has_started()) ;  // handle premature message
    game.processInput(message);
}
