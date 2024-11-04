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

enum NetUnexpectedEvent {
    EL_NONE,
    EL_ERR_UNDEFINED,
    EL_ERR_COMPRESSION,
    EL_ERR_RECORDING,
    EL_ERR_SOCKET,
    EL_ERR_SOCKET_WRONG_ADDRESS,
    EL_RECORDING_END
};

/**
 * @class NetException
 * @brief Custom exception class for network-related errors.
 *
 * This class extends the standard std::exception to provide additional
 * information about network-related errors, including an error message,
 * the location of the exception, and whether the error is fatal.
 * This tells if the network operation should be fatal, and if it should be
 */
class NetException : public std::exception {
public:
    NetException(const std::string &message, NetUnexpectedEvent location = EL_ERR_UNDEFINED, bool fatal = false)
        : m_message(message)
        , m_location(location)
        , m_fatal(fatal) {
    }
    ~NetException() = default;

    const char *what() const noexcept override {
        return m_message.c_str();
    }

    NetUnexpectedEvent getLocation(void) const {
        return m_location;
    }

    bool shouldRetry(void) const {
        return !m_fatal;
    }

private:
    std::string m_message;
    NetUnexpectedEvent m_location;
    bool m_fatal;
};
} // namespace Network
