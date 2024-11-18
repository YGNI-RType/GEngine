/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_socket_system
*/

#include "GEngine/net/net_socket_system.hpp"

namespace Network {
SocketSTD::SocketSTD(int stdNumber) {
#ifdef NET_USE_HANDLE
    m_handle = GetStdHandle(stdNumber);
#else
    m_sock = stdNumber;
#endif
}

void SocketSTD::setStd(int stdNumber) {
#ifdef NET_USE_HANDLE
    m_handle = GetStdHandle(stdNumber);
#else
    m_sock = stdNumber;
#endif
}
} // namespace Network