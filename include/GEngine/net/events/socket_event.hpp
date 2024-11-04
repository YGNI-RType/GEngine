/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** socket_event
*/

#pragma once

#include "../net_socket.hpp"

#ifdef _WIN32
#include <atomic>
#elif defined(__linux__)
#include <sys/eventfd.h>
#else
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(__APPLE__)
#define HAS_NOT_EVENTFD = 1
#endif

namespace Network::Event {
/**
 * @class SocketEvent
 * @brief A class that represents a socket event, inheriting from ASocket.
 *
 * This class provides mechanisms to signal and wait for socket events.
 * It is non-copyable but movable.
 * This mechanism is used to wake up the network thread from the gengine which wants to send a message or use the
 * network (like a connection, record request etc...)
 *
 * @note If the macro HAS_NOT_EVENTFD is defined, additional private members
 *       are included to handle socket events using either HANDLE or SOCKET.
 */
class SocketEvent : public ASocket {
public:
    SocketEvent();
    ~SocketEvent();

    SocketEvent(const SocketEvent &other) = delete;
    SocketEvent &operator=(const SocketEvent &other) = delete;
    SocketEvent(SocketEvent &&other);
    SocketEvent &operator=(SocketEvent &&other);

    void signal();
    void wait();

#ifdef HAS_NOT_EVENTFD
private:
#ifdef NET_USE_HANDLE
    HANDLE m_writeHandle = INVALID_HANDLE_VALUE;
#else
    SOCKET m_sockConnect = -1;
#endif
#endif
};
} // namespace Network::Event
