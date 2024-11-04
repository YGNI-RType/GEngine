/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socketError
*/

#pragma once

#include <exception>

#ifdef _WIN32
#define socketErrorStr(code) std::to_string(code)
#else
#include <cstring>
#define socketErrorStr(code) std::strerror(code)
#endif

namespace Network {

/**
 * @class SocketException
 * @brief Exception class for socket-related errors.
 *
 * This class represents an exception that is thrown when a socket error occurs.
 * It provides an error message and a flag indicating whether the operation should be retried.
 */
class SocketException : public std::exception {
public:
    SocketException(const char *msg, bool shouldRetry = true)
        : m_msg(msg)
        , m_shouldRetry(shouldRetry) {
    }
    SocketException(int code, bool shouldRetry = true)
        : m_msg(socketErrorStr(code))
        , m_code(code)
        , m_shouldRetry(shouldRetry) {
    }

    const char *what() const noexcept override {
        return m_msg.c_str();
    }

    bool shouldRetry() const {
        return m_shouldRetry;
    }

    uint16_t getCode() const {
        return m_code;
    }

private:
    std::string m_msg;
    bool m_shouldRetry;
    int m_code;
};

/**
 * @class SocketDisconnected
 * @brief Exception class for handling socket disconnection errors.
 *
 * This class represents an exception that is thrown when an incoming socket gets disconnected.
 * It inherits from the SocketException class.
 */
class SocketDisconnected : public SocketException {
public:
    SocketDisconnected()
        : SocketException("Incomming Socket disconnected") {
    }
};
} // namespace Network
