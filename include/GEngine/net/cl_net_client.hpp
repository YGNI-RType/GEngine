/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#pragma once

#include "net_channel.hpp"
#include "net_common.hpp"

#include <memory>
#include <vector>

namespace Network {

struct PingResponse {
    UDPSV_PingResponse response;
    std::unique_ptr<Address> addr;
};

class CLNetClient {

public:
    CLNetClient(SocketUDP &socketUdp, AddressType type)
        : m_socketUdp(socketUdp), m_addrType(type), m_netChannel(NetChannel(false, nullptr, SocketTCP())) {
    };
    ~CLNetClient() = default;

    void init(void);
    void stop(void);

    /* index of the pinged servers */
    bool connectToServer(size_t index);
    bool connectToServer(const std::string &ip, uint16_t port, bool block = false);
    void disconnectFromServer(void);

    void createSets(fd_set &readSet);

    bool handleTCPEvents(fd_set &readSet);
    bool handleUDPEvents(UDPMessage &msg, const Address &addr);

    bool handleServerUDP(UDPMessage &msg, const Address &addr);
    bool handleServerTCP(const TCPMessage &msg);

    void setChallenge(int challenge) {
        m_challenge = challenge;
    }
    int getChallenge(void) const {
        return m_challenge;
    }

    auto &getPingResponses(void) const {
        return m_pingedServers;
    }

    bool isConnected(void) const {
        return m_connectionState >= CON_AUTHORIZING;
    }

    bool sendDatagram(UDPMessage &finishedMsg);

public:
    void pingLanServers(void);
    void getPingResponse(const UDPMessage &msg, const Address &addr);

private:
    int m_challenge = -1;

    bool m_enabled = false;
    clientState m_state = CS_FREE;
    connectionState m_connectionState = CON_UNINITIALIZED;

    SocketUDP &m_socketUdp;
    AddressType m_addrType;

    NetChannel m_netChannel;
    std::vector<PingResponse> m_pingedServers;
};
} // namespace Network
