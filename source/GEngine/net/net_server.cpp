/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_server
*/

#include "GEngine/net/net_server.hpp"
#include "GEngine/cvar/net.hpp"

#include "GEngine/net/net.hpp"
#include "GEngine/time/time.hpp"

// #ifdef NET_DEBUG
#include <iostream>
// #endif

namespace Network {

uint16_t NetServer::start(uint16_t &currentUnusedPort) {
    // TODO : cloes everything if already initted
    if (m_isRunning)
        return currentUnusedPort;

    /* currentUnusedPort is modified ia side effect */
    m_socketv4 = openSocketTcp(currentUnusedPort, false);
    if (CVar::net_ipv6.getIntValue()) // check if ipv6 is supported
        m_socketv6 = openSocketTcp(currentUnusedPort, true);

    m_isRunning = true;
    return currentUnusedPort;
}

void NetServer::stop(void) {
    m_clients.clear();
    m_isRunning = false;
}

void NetServer::createSets(NetWaitSet &set) {
    if (!isRunning())
        return;
    set.setAlert(m_socketv4, [this]() {
        handleNewClient(m_socketv4);
        return true;
    });
    if (CVar::net_ipv6.getIntValue())
        set.setAlert(m_socketv6, [this]() {
            handleNewClient(m_socketv6);
            return true;
        });

    for (auto &client : m_clients)
        client->createSets(set);
}

void NetServer::respondPingServers(const UDPMessage &msg, SocketUDP &udpsocket, const Address &addr) {
    auto pingReponseMsg = UDPMessage(0, SV_BROADCAST_PING);
    UDPSV_PingResponse data = {.tcpv4Port = m_socketv4.getPort(),
                               .tcpv6Port = CVar::net_ipv6.getIntValue() ? m_socketv6.getPort() : (uint16_t)(-1),
                               .maxPlayers = getMaxClients(),
                               .currentPlayers = getNumClients(),
                               .os = static_cast<uint8_t>(OS_TYPE),
                               .ping = Time::Clock::milliseconds()};

    pingReponseMsg.writeData<UDPSV_PingResponse>(data);
    udpsocket.send(pingReponseMsg, addr);
}

void NetServer::handleNewClient(SocketTCPMaster &socket) {
    UnknownAddress unkwAddr;
    SocketTCP newSocket = socket.accept(unkwAddr);

    if (getNumClients() >= getMaxClients()) {
        newSocket.socketClose();
        return;
    }

    std::shared_ptr<NetClient> cl;
    auto clientAddrType = unkwAddr.getType();
    if (clientAddrType == AT_IPV4)
        cl = std::make_shared<NetClient>(std::make_unique<AddressV4>(unkwAddr.getV4()), std::move(newSocket),
                                         m_socketUdpV4, NET::getEventManager().getSocketEvent());

    else if (clientAddrType == AT_IPV6)
        cl = std::make_shared<NetClient>(std::make_unique<AddressV6>(unkwAddr.getV6()), std::move(newSocket),
                                         m_socketUdpV6, NET::getEventManager().getSocketEvent());
    else
        return; /* impossible */

    // #ifdef NET_DEBUG
    std::cout << "SV: Client connected" << std::endl;
    // #endif
    m_clients.push_back(cl);

    auto msg = TCPMessage(SV_INIT_CONNECTON);
    auto &channel = cl->getChannel();

    msg.writeData<TCPSV_ClientInit>(
        {.challenge = channel.getChallenge(),
         .udpPort = clientAddrType == AT_IPV6 ? m_socketUdpV6.getPort() : m_socketUdpV4.getPort()});

    NET::getEventManager().invokeCallbacks(Event::CT_OnClientConnect, cl);
    channel.sendStream(msg);
}

bool NetServer::handleUDPEvent(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    if (!isRunning())
        return false;

    switch (msg.getType()) {
    case CL_BROADCAST_PING:
        respondPingServers(msg, socket, addr);
        return true;
    default:
        return handleUdpMessageClients(socket, msg, addr);
    }
}

bool NetServer::handleUdpMessageClients(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    for (const auto &client : m_clients) {
        auto &channel = client->getChannel();
        if (!channel.isUDPEnabled())
            continue;

        if (channel.getAddressUDP() != addr)
            continue;

        client->handleClientDatagram(socket, msg);
        return true;
    }
    return false;
}

bool NetServer::handleTCPEvent(const NetWaitSet &set) {
    if (!isRunning())
        return false;

    if (set.isSignaled(m_socketv4)) {
        handleNewClient(m_socketv4);
        return true;
    }

    if (CVar::net_ipv6.getIntValue() && set.isSignaled(m_socketv6)) {
        handleNewClient(m_socketv6);
        return true;
    }

    for (const auto &client : m_clients)
        if (client->handleTCPEvents(set)) {
            if (client->isDisconnected())
                disconnectClient(client.get(), Event::DT_WANTED);
            return true;
        }

    return false;
}

uint32_t NetServer::getMaxClients(void) const {
    return CVar::sv_maxplayers.getIntValue();
}

bool NetServer::sendPackets(void) {
    if (!isRunning())
        return false;

    for (const auto &client : m_clients)
        client->sendPackets();
    return true;
}

void NetServer::sendToAllClients(UDPMessage &msg) {
    for (const auto &client : m_clients)
        client->sendDatagram(msg);
}

void NetServer::sendToClient(NetClient &client, UDPMessage &msg) {
    if (client.isDisconnected())
        return;

    client.sendDatagram(msg);
}

void NetServer::checkTimeouts(void) {
    if (!isRunning())
        return;

    for (const auto &client : m_clients) {
        if (!client->isTimeout())
            continue;

        std::cout << "SV: Client timeout" << std::endl;
        disconnectClient(client.get(), Event::DT_TIMEOUT);
    }
}

void NetServer::disconnectClient(NetClient *client, Event::DisonnectType type) {
    if (!isRunning())
        return;

    std::cout << "SV: Client disconnected" << std::endl;
    Network::Event::DisconnectInfo info = {client, type};
    NET::getEventManager().invokeCallbacks(Event::CT_OnClientDisconnect, info);
    m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
                                   [&client](const std::shared_ptr<NetClient> &cl) { return cl.get() == client; }),
                    m_clients.end());
}

std::string NetServer::getAddress_TS(void) const {
    auto sock = m_socketv4.getSocket();
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(sock, (struct sockaddr *)&addr, &addr_len) == -1)
        return "";

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    return std::string(ip);
}

uint16_t NetServer::getPort_TS(void) const {
    return m_socketv4.getPort();
}

} // namespace Network
