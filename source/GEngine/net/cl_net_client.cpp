/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#include "GEngine/net/cl_net_client.hpp"

namespace Network {
void CLNetClient::handleUDPEvents(SocketUDP &socket, const UDPMessage &msg, const Address &addr) {
    switch (msg.getType()) {
    case SV_BROADCAST_PING:

        break;
        // handlePingResponse(msg, addr);
    default: // handleUdpMessage(msg, addr);
        break;
    }
}

void CLNetClient::getPingResponse(const UDPMessage &msg, const Address &addr) {
    UDPSV_PingResponse data;
    msg.readData<UDPSV_PingResponse>(data);

    m_pingedServers.push_back(data);
}

void CLNetClient::pingLanServers(SocketUDP &socketUDP, AddressType type) {
    m_pingedServers.clear();
    for (uint16_t port = DEFAULT_PORT; port < DEFAULT_PORT + MAX_TRY_PORTS; port++) {
        auto message = UDPMessage(0, CL_BROADCAST_PING);

        if (type == AT_IPV4)
            socketUDP.send(message, AddressV4(AT_BROADCAST, port));
        else
            socketUDP.send(message, AddressV6(AT_MULTICAST, port));
    }
}

} // namespace Network
