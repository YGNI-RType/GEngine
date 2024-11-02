/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_exception
*/

#pragma once

#include <exception>
#include <string>

namespace Network {

enum ExceptionLocation {
    EL_NONE,
    EL_UNDEFINED,
    EL_COMPRESSION,
    EL_RECORDING,
    EL_SOCKET,
    EL_SOCKET_WRONG_ADDRESS,
};

class NetException : public std::exception {
public:
    NetException(const std::string &message, ExceptionLocation location = EL_UNDEFINED, bool fatal = false)
        : m_message(message)
        , m_location(location)
        , m_fatal(fatal) {
    }
    ~NetException() = default;

    const char *what() const noexcept override {
        return m_message.c_str();
    }

    ExceptionLocation getLocation(void) const {
        return m_location;
    }

    bool shouldRetry(void) const {
        return !m_fatal;
    }

private:
    std::string m_message;
    ExceptionLocation m_location;
    bool m_fatal;
};
} // namespace Network
