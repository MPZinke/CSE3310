//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include "chat_message.hpp"

#include <iostream>
#include <sstream>

chat_message::chat_message()
    : body_length_(0) {
}

chat_message::chat_message(nlohmann::json msg) {
    std::string msgstr = msg.dump();
    const char* str = msgstr.c_str();
    body_length(std::strlen(str));
    std::memcpy(body(), str, body_length());
    encode_header();
}

nlohmann::json chat_message::getJson() {
    std::string message = body();
    message = message.substr(0, body_length_);
    message = message.substr(0, length());
    return nlohmann::json::parse(message);
}

const char* chat_message::data() const {
    return data_;
}

char* chat_message::data() {
    return data_;
}

std::size_t chat_message::length() const {
    return header_length + body_length_;
}

const char* chat_message::body() const {
    return data_ + header_length;
}

char* chat_message::body() {
    return data_ + header_length;
}

std::size_t chat_message::body_length() const {
    return body_length_;
}

void chat_message::body_length(std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

bool chat_message::decode_header() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::stoi(header, NULL, 16);
    if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    return true;
}

void chat_message::encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%3X", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}

