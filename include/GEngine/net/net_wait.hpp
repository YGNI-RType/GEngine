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

    void applyCallback(void) const {
        return m_callbacks[m_resIndex]();
    }

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