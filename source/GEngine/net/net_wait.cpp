/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_wait
*/

#include "GEngine/net/net_wait.hpp"
#include "GEngine/net/net_socket_error.hpp"
#include "GEngine/net/net.hpp"

namespace Network {

bool NetWaitSet::isSignaled(const ASocket &socket) const {
    return FD_ISSET(socket.getSocket(), &m_readSet);
}

void NetWaitSet::setAlert(const ASocket &socket) {
    FD_SET(socket.getSocket(), &m_readSet);
}

void NetWaitSet::reset(void) {
    FD_ZERO(&m_readSet);
}

/************************************************************/

SOCKET NetWait::m_highFd = -1;
fd_set NetWait::m_fdSet;

NetWait::NetWait() {
    FD_ZERO(&m_fdSet);
}

bool NetWait::wait(uint32_t ms, NetWaitSet &set) {
    SOCKET highest = NetWait::getHighestSocket();

    /* The usage of select : both on windows and unix systems */
    // #ifdef _WIN32
    //     HANDLE *pHandles = nullptr;
    //     DWORD  max = 0;
    //     DWORD dwEvent = WaitForMultipleObjects(highest, pHandles, FALSE, ms);

    //     switch (dwEvent) {
    //     case WAIT_FAILED:
    //         throw SocketException(socketError);

    //     case WAIT_TIMEOUT:
    //         return false;
    //     }
    // #else
        struct timeval timeout = {.tv_sec = static_cast<long>(ms / 1000u), .tv_usec = static_cast<int>((ms % 1000) * 1000)};
        int res = select(highest + 1, &set.getFdSet(), nullptr, nullptr, &timeout);
        if (res == -1)
            throw SocketException(socketError);

        else if (res == 0)
            return false;

    // #endif
    return true;
}

void NetWait::addSocketPool(SOCKET socket) {
    FD_SET(socket, &m_fdSet);
    if (socket > m_highFd)
        m_highFd = socket;
}

void NetWait::removeSocketPool(SOCKET socket) {
    if (socket != m_highFd)
        return;
    
    while (!FD_ISSET(m_highFd, &m_fdSet))
        (m_highFd)--;
}
}