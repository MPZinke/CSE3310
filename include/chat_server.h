#ifndef chat_server_inc
#define chat_server_inc
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
#include "chat_participant.h"
#include "chat_message.hpp"
#include "chat_room.h"

class chat_server {
public:
    chat_server(asio::io_context& io_context,
                const tcp::endpoint& endpoint);

protected:
    void do_accept();

    tcp::acceptor acceptor_;
    chat_room room_;
};

#endif
