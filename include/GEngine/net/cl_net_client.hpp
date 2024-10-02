/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#pragma once

#include "net_channel.hpp"
#include "net_common.hpp"

#include <vector>
#include <memory>

namespace Network {

struct PingResponse {
    UDPSV_PingResponse response;
    std::unique_ptr<Address> addr;
};

class CLNetClient {

public:
    CLNetClient() = default;
    ~CLNetClient() = default;

    void init(void);
    void stop(void);

    /* index of the pinged servers */
    void connectToServer(size_t index);
    void connectToServer(const std::string &ip, uint16_t port);
    void disconnectFromServer(void);

    bool handleTCPEvents(fd_set &readSet);
    bool handleUDPEvents(SocketUDP &socket, const UDPMessage &msg, const Address &addr);

    bool handleServerUDP(SocketUDP &socket, const UDPMessage &msg, const Address &addr);

    void setChallenge(int challenge) { m_challenge = challenge; }
    int getChallenge(void) const { return m_challenge; }

    auto &getPingResponses(void) const { return m_pingedServers; }

    bool isConnected(void) const { return m_connectionState >= CON_AUTHORIZING; }


public:
    void pingLanServers(SocketUDP &socketUDP, AddressType type);
    void getPingResponse(const UDPMessage &msg, const Address &addr);

private:
    int m_challenge = -1;

    bool m_enabled = false;
    clientState m_state = CS_FREE;
    connectionState m_connectionState = CON_UNINITIALIZED;

    NetChannel m_netChannel;
    std::vector<PingResponse> m_pingedServers;
};
} // namespace Network
