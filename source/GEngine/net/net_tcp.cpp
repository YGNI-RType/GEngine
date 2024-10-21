/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_tcp
*/

#include "GEngine/net/net_tcp.hpp"

namespace Network {

int64_t TCPManager::mg_curRateBytes;
size_t TCPManager::mg_nbManager = 0;
size_t TCPManager::mg_sendTotalBytes = 0;
std::chrono::milliseconds TCPManager::m_lastTime = std::chrono::milliseconds(0);

TCPManager::TCPManager(SocketTCP &&socket)
    : m_socket(std::move(socket))
    , m_curMessageIn({0}) {
    mg_nbManager++;
}

TCPManager::~TCPManager() {
    mg_nbManager--;
}

bool TCPManager::setBalanceDownloadUpload(size_t balance) {
    if (balance > 100)
        return false;

    m_balanceDownloadUpload = balance;
    return true;
}

TCPManager::Result TCPManager::sendStream(std::unique_ptr<TCPMessage> msg) {
    if (m_tcpOut.size() >= MAX_TCP_MSGS)
        return KO_FULL;

    if (!m_tcpOut.empty())
        return OK_LATER;

    m_tcpOut.push(std::move(msg));
    return mg_curRateBytes > msg->getSize() ? OK_NOW : OK_LATER;
}

bool TCPManager::processMessage(void) {
    const size_t deltaTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count() -
        m_lastTime.count();
    mg_curRateBytes = CF_NET_MAX(0, mg_curRateBytes - MAX_RATE_BYTES * (deltaTime * 100 / MAX_RATE_BYTES));

    const size_t nbBytesToSendMax = CF_NET_MIN(MAX_SESSION_BYTES, (MAX_RATE_BYTES - mg_curRateBytes) / mg_nbManager);
    const size_t nbBytesToSend = nbBytesToSendMax * m_balanceDownloadUpload / 100;
    const size_t nbBytesToRecv = nbBytesToSendMax - nbBytesToSend;

    /* Receiving */
    bool newMessageComplete = false;
    size_t oldOffset = m_curMessageInOffset;
    bool hasMinimum = m_socket.receivePartial(m_curMessageIn, nbBytesToRecv, m_curMessageInOffset);
    if (hasMinimum && m_curMessageInOffset < m_curMessageInOffset == m_curMessageIn.curSize)
        newMessageComplete = true;

    /* Sending */
    m_socket.sendPartial(*m_tcpOut.front(), nbBytesToSend, m_curMessageOutOffset);
    if (m_curMessageOutOffset == m_tcpOut.front()->getSize()) {
        m_tcpOut.pop();
        m_curMessageOutOffset = 0;
    }
    return newMessageComplete;
}

void TCPManager::getMessageIn(TCPMessage &message) const {
    message.setSerialize(m_curMessageIn);
}

void TCPManager::resetMessageIn(void) {
    m_curMessageIn = {0};
    m_curMessageInOffset = 0;
}

} // namespace Network