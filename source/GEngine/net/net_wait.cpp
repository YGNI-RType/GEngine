/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_wait
*/

#include "GEngine/net/net_wait.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/net/net_socket_error.hpp"

#include <algorithm>

namespace Network {

bool NetWaitSet::isSignaled(const ASocket &socket) const {
#ifdef NET_USE_HANDLE
    if (m_resIndex > MAX_SOCKETS)
        throw SocketException("m_resIndex > MAX_SOCKETS");

    SOCKET sock = socket.getSocket();
    if (sock == -1)
        return m_resIndex == IS_QUEUE;

    WSANETWORKEVENTS networkEvents;
    if (WSAEnumNetworkEvents(sock, socket.getHandle(), &networkEvents) == SOCKET_ERROR)
        throw SocketException(GetLastError());

    if (!(networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_ACCEPT))
        return false;

    if (networkEvents.iErrorCode[FD_READ_BIT] != 0 && networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
        throw SocketException(GetLastError());

    return true;
#else
    return FD_ISSET(socket.getSocket(), &m_readSet);
#endif
}

void NetWaitSet::setAlert(const ASocket &socket) {
#ifdef NET_USE_HANDLE
    if (m_count >= MAX_SOCKETS)
        return;

    m_events[m_count++] = socket.getHandle();
#else
    FD_SET(socket.getSocket(), &m_readSet);
#endif
}

void NetWaitSet::reset(void) {
#ifdef NET_USE_HANDLE
    /* map with CloseHandle */
    std::fill(std::begin(m_events), std::end(m_events), INVALID_HANDLE_VALUE);
    m_count = 0;
    m_resIndex = -1;
#else
    FD_ZERO(&m_readSet);
#endif
}

/************************************************************/

#ifdef NET_USE_HANDLE
#else
SOCKET NetWait::m_highFd = -1;
fd_set NetWait::m_fdSet;
#endif

NetWait::NetWait() {
#ifdef NET_USE_HANDLE
#else
    FD_ZERO(&m_fdSet);
#endif
}

bool NetWait::wait(uint32_t ms, NetWaitSet &set) {
#ifdef NET_USE_HANDLE
    /* TODO : WARNING: can make multiple threads that wake for each one a WaitForMultipleObjects, car on est niqué avec
     * un max de 64 sockets (~30 joeurs max) */
    DWORD dwEvent = WSAWaitForMultipleEvents(set.getCount(), set.getHandles(), FALSE, ms, FALSE);

    switch (dwEvent) {
    case WAIT_FAILED:
        throw SocketException(GetLastError());

    case WAIT_TIMEOUT:
        return false;
    }

    set.setResultIndex(dwEvent);
#else
    SOCKET highest = NetWait::getHighestSocket();
    struct timeval timeout = {.tv_sec = static_cast<long>(ms / 1000u), .tv_usec = static_cast<int>((ms % 1000) * 1000)};
    int res = select(highest + 1, &set.getFdSet(), nullptr, nullptr, &timeout);
    if (res == -1)
        throw SocketException(socketError);

    else if (res == 0)
        return false;
#endif
    return true;
}

void NetWait::addSocketPool(ASocket &socket) {
#ifdef NET_USE_HANDLE
    socket.createHandle();
#else
    auto sock = socket.getSocket();

    FD_SET(sock, &m_fdSet);
    if (sock > m_highFd)
        m_highFd = sock;
#endif
}

void NetWait::removeSocketPool(const ASocket &socket) {
#ifndef NET_USE_HANDLE
    auto sock = socket.getSocket();
    if (sock != m_highFd)
        return;

    while (!FD_ISSET(m_highFd, &m_fdSet))
        (m_highFd)--;
#endif
}
} // namespace Network