/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_tcp
*/

#include "GEngine/net/net_tcp.hpp"

namespace Network {
    size_t TCPManager::mg_nbManager = 0;

    TCPManager::TCPManager(SocketTCP &&socket)
        : m_socket(std::move(socket)), m_lastTime(std::chrono::milliseconds(0)), m_curMessageIn(0), m_curMessageOut(0)
    {
        mg_nbManager++;
    }

    TCPManager::~TCPManager()
    {
        mg_nbManager--;
    }

    bool TCPManager::setBalanceDownloadUpload(size_t balance)
    {
        if (balance > 100)
            return false;
        m_balanceDownloadUpload = balance;
        return true;
    }

    TCPManager::Result TCPManager::sendStream(std::unique_ptr<TCPMessage> msg)
    {
        if (m_tcpOut.size() >= MAX_TCP_MSGS)
            return KO_FULL;

        if (!m_tcpOut.empty())
            return OK_LATER;

        return mg_curRateBytes > msg->getSize() ? OK_NOW : OK_LATER;
    }


} // namespace Network