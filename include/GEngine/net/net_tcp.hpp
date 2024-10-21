/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_tcp
*/

#pragma once

#include "net_socket.hpp"
#include "net_queue_heap.hpp"

#include <chrono>

namespace Network {
class TCPManager {
public:

enum Result {
    OK_NOW,
    OK_LATER,
    KO_FULL
};

public:
    TCPManager(SocketTCP &&socket);
    ~TCPManager();

    TCPManager(const TCPManager &other) = delete;
    TCPManager &operator=(const TCPManager &other) = delete;
    TCPManager(TCPManager &&other) = default;
    TCPManager &operator=(TCPManager &&other) = default;

    bool readStream(void);
    Result sendStream(std::unique_ptr<TCPMessage> msg);

    /* Returns true if process */
    bool processMessage(void);

public:
    const SocketTCP &getSocket(void) const {
        return m_socket;
    }

    static size_t getNbManager(void) {
        return mg_nbManager;
    }

    bool setBalanceDownloadUpload(size_t balance);

private:
    static constexpr size_t MAX_SESSION_BYTES = 1024;
    static constexpr size_t MAX_RATE_BYTES = 0xFFFFF; /* EVERY SECOND, WE DECREASE */
    static constexpr size_t MAX_TCP_MSGS = 20;
    static size_t mg_curRateBytes;
    static size_t mg_nbManager;

    SocketTCP m_socket;
    std::chrono::milliseconds m_lastTime;
    NetQueueHeap<TCPMessage, MAX_TCP_MSGS> m_tcpOut;
    size_t m_balanceDownloadUpload = 50; /* In %, tell which of the rate should be dedicated in upload and download */
    TCPMessage m_curMessageIn;
};;
} // namespace Network