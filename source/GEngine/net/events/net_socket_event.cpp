/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_event_socket
*/

#include "GEngine/net/events/socket_event.hpp"
#include "GEngine/net/net_wait.hpp"

#include <stdexcept>

#ifdef HAS_NOT_EVENTFD
#if defined(__APPLE__) || defined(__unix__)
#define closesocket close
#include <unistd.h>
#else
#include <cstring>
#endif
#endif

namespace Network::Event {

SocketEvent::SocketEvent() {
#ifndef NET_USE_HANDLE
#ifdef HAS_NOT_EVENTFD
#if defined(__APPLE__) || defined(__unix__)
    int pipefd[2];
    if (pipe(pipefd) == -1)
        throw std::runtime_error("Failed to create pipe");

    int flags = fcntl(pipefd[1], F_GETFL, 0);
    fcntl(pipefd[1], F_SETFL, flags | O_NONBLOCK);
#if __linux__
    int pipe_sz = fcntl(pipefd[1], F_SETPIPE_SZ, 1);
#endif

    m_sock = pipefd[0];
    m_sockConnect = pipefd[1];
#endif
#else
    m_sock = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (m_sock == -1)
        throw std::runtime_error("Failed to create eventfd");
#endif
    NetWait::addSocketPool(*this);
#else
    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // if (!CreatePipe(&m_handle, &m_writeHandle, &saAttr, 0))
    //     throw std::runtime_error("Failed to create pipe");

    // m_sock = 666; //socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_handle = CreateEvent(NULL, FALSE, FALSE, "net_event_wake");
#endif
}

SocketEvent::~SocketEvent() {
#ifdef HAS_NOT_EVENTFD
#ifdef NET_USE_HANDLE
    if (m_writeHandle != INVALID_HANDLE_VALUE)
        CloseHandle(m_writeHandle);
#else
    if (m_sockConnect != -1)
        closesocket(m_sockConnect);
#endif
#endif
}

SocketEvent::SocketEvent(SocketEvent &&other)
    : ASocket(std::move(other)) {
#ifdef HAS_NOT_EVENTFD
#ifdef NET_USE_HANDLE
    m_writeHandle = other.m_writeHandle;
    other.m_writeHandle = INVALID_HANDLE_VALUE;
#else
    m_sockConnect = other.m_sockConnect;
    other.m_sockConnect = -1;
#endif
#endif
}

SocketEvent &SocketEvent::operator=(SocketEvent &&other) {
    if (this != &other) {
        ASocket::operator=(std::move(other));
#ifdef HAS_NOT_EVENTFD
#ifdef NET_USE_HANDLE
        m_writeHandle = other.m_writeHandle;
        other.m_writeHandle = INVALID_HANDLE_VALUE;
#else
        m_sockConnect = other.m_sockConnect;
        other.m_sockConnect = -1;
#endif
#endif
    }
    return *this;
}

void SocketEvent::signal() {
#ifdef HAS_NOT_EVENTFD
    char buf[1] = {0};
#ifdef NET_USE_HANDLE
    DWORD dwWrite;
    // WriteFile(m_writeHandle, buf, sizeof(buf), &dwWrite, NULL);
    SetEvent(m_handle);
#else
    write(m_sockConnect, buf, sizeof(buf));
#endif
#else
    eventfd_write(m_sock, 1);
#endif
}

void SocketEvent::wait() {
#ifdef HAS_NOT_EVENTFD
    char buf[1];
#ifdef NET_USE_HANDLE
    DWORD dwRead;
    // ReadFile(m_handle, buf, sizeof(buf), &dwRead, NULL);
    ResetEvent(m_handle);
#else
    read(m_sock, buf, sizeof(buf));
#endif
#else
    eventfd_t c;
    eventfd_read(m_sock, &c);
#endif
}
} // namespace Network::Event
