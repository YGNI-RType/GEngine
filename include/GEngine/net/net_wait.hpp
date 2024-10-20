/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_wait
*/

#include "net_common.hpp"
#include "net_socket.hpp"

#pragma once

namespace Network {

class NetWaitSet {
public:
    bool isSignaled(const ASocket &socket) const;
    void setAlert(const ASocket &socket);

    void reset(void);

public:
    fd_set &getFdSet(void) {
        return m_readSet;
    }


private:
    // std::array<HANDLE *, 1024> m_events;
    fd_set m_readSet;
};

class NetWait {
public:
    NetWait();
    virtual ~NetWait() = default;

    bool wait(uint32_t ms, NetWaitSet &set);

public:
    static void addSocketPool(SOCKET socket);
    static void removeSocketPool(SOCKET socket);


public:
    static SOCKET getHighestSocket(void) {
        return m_highFd;
    }

private:
    static fd_set m_fdSet;
    static SOCKET m_highFd;
};

} // namsepsace Network