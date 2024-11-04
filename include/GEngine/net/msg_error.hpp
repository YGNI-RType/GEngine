/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** msg_error
*/

#pragma once

#include <stdexcept>

namespace Network {
/**
 * @class MsgError
 * @brief Custom exception class for handling error messages.
 *
 * This class inherits from std::exception and is used to represent
 * error messages in the application. It stores an error message
 * and provides a method to retrieve it.
 */
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
