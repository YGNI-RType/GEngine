/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** msg_error
*/

#pragma once

#include <stdexcept>

namespace Network {
class MsgError : public std::exception {
public:
    explicit MsgError(const std::string &msg)
        : m_msg(msg) {
    }

    const char *what() const noexcept override {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};
} // namespace Network
