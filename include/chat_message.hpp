//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "json.hpp"

class chat_message {
public:
    enum { header_length = 3 };
    // CSE3310 maximum size of chat message body is defined
    enum { max_body_length = 0xFFF-3};

    chat_message();
    chat_message(nlohmann::json msg);

    nlohmann::json getJson();

    const char* data() const;
    char* data();

    std::size_t length() const;

    const char* body() const;
    char* body();

    std::size_t body_length() const;
    void body_length(std::size_t new_length);

    bool decode_header();
    void encode_header();

private:
    char data_[header_length + max_body_length];
    std::size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP
