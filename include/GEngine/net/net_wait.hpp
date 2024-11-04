/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_wait
*/

#include "net_common.hpp"
#include "net_socket.hpp"

#include <functional>

#pragma once

namespace Network {

/**
 * @class NetWaitSet
 * @brief A class that manages network wait sets for handling socket events.
 *
 * The NetWaitSet class provides methods to manage and monitor multiple socket events.
 * It supports both handle-based and fd_set-based implementations, depending on the
 * platform and configuration.
 *
 * @note The class uses conditional compilation to support different platforms.
 */
class NetWaitSet {
public:
    static constexpr size_t IS_QUEUE = 0;

public:
    bool isSignaled(const ASocket &socket) const;

    void setAlert(const ASocket &socket, std::function<bool(void)> &&callback);

    void reset(void);

#ifdef NET_USE_HANDLE
public:
    size_t getCount(void) const {
        return m_count;
    }

    const HANDLE *getHandles(void) const {
        return m_events.data();
    }

    WSANETWORKEVENTS &getNetEvents(void) {
        return m_netEvents;
    }

    void setResultIndex(uint64_t res) {
        m_resIndex = res;
    }

    bool applyCallback(void) const;

private:
    static constexpr size_t MAX_SOCKETS = MAXIMUM_WAIT_OBJECTS;

    std::array<HANDLE, MAX_SOCKETS> m_events;
    std::array<std::function<bool(void)>, MAX_SOCKETS> m_callbacks;
    size_t m_resIndex = -1;
    size_t m_count = 0;
    WSANETWORKEVENTS m_netEvents;
#else
public:
    fd_set &getFdSet(void) {
        return m_readSet;
    }

private:
    fd_set m_readSet;
#endif
};

/**
 * @class NetWait
 * @brief A class to manage network wait operations.
 *
 * The NetWait class provides functionality to wait for network events on a set of sockets.
 * It also manages a pool of sockets and provides utility functions to add or remove sockets
 * from this pool.
 * So this is used to encapsulate select and WSAWaitForMultipleEvents.
 *
 * @note This class uses different implementations based on whether NET_USE_HANDLE is defined.
 */
class NetWait {
public:
    NetWait();
    virtual ~NetWait() = default;

    bool wait(uint32_t ms, NetWaitSet &set);

public:
    static void addSocketPool(ASocket &socket);
    static void removeSocketPool(const ASocket &socket);

#ifdef NET_USE_HANDLE

#else
public:
    static SOCKET getHighestSocket(void) {
        return m_highFd;
    }

private:
    static fd_set m_fdSet;
    static SOCKET m_highFd;
#endif
};

} // namespace Network