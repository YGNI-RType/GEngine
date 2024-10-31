/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#include "GEngine/net/cl_net_client.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/time/time.hpp"

#include "GEngine/net/events/ping_result.hpp"

#include <iostream> // todo : remove

namespace Network {

bool CLNetClient::connectToServer(size_t index) {

    /* Connects to something */
    if (index >= m_pingedServers.size())
        return false;

    // std::cout << "CL: connecting to server" << std::endl;

    auto &[response, addr] = m_pingedServers.at(index);

    if (addr->getType() != m_addrType)
        return false;

    auto sock = addr->getType() == AT_IPV4 ? SocketTCP(static_cast<AddressV4 &>(*addr), response.tcpv4Port)
                                           : SocketTCP(static_cast<AddressV6 &>(*addr), response.tcpv6Port);

    sock.setBlocking(false);
    m_netChannel = std::move(NetChannel(false, std::move(addr), std::move(sock)));
    /* todo : some error handling just in case ? */

    m_state = CS_CONNECTED;
    m_connectionState = CON_CONNECTING;
    return true;
}

bool CLNetClient::connectToServer(const std::string &ip, uint16_t port, bool block) {
    std::unique_ptr<Address> addr;

    if (m_addrType == AT_IPV4)
        addr = std::make_unique<AddressV4>(AT_IPV4, ip, port);
    else if (m_addrType == AT_IPV6)
        addr = std::make_unique<AddressV6>(AT_IPV6, ip, port);
    else
        return false;

    /* fix : when the addres is bad, it holds since it blocks, make it unblock by default ? */
    auto sock = m_addrType == AT_IPV4 ? SocketTCP(static_cast<AddressV4 &>(*addr), port, block)
                                      : SocketTCP(static_cast<AddressV6 &>(*addr), port, block);

    // std::cout << "connecting is not ready ?: " << sock.isNotReady() << std::endl;
    m_netChannel = std::move(NetChannel(false, std::move(addr), std::move(sock)));
    //     /* todo : some error handling just in case ? */

    m_state = CS_CONNECTED;
    m_connectionState = CON_CONNECTING;
    return true;
}

void CLNetClient::disconnectFromServer(Event::DisonnectType disconnectType) {
    m_netChannel.setTcpSocket(SocketTCP());

    NET::getEventManager().invokeCallbacks(Event::CT_OnServerDisconnect, disconnectType);

    m_state = CS_FREE;
    m_connectionState = CON_DISCONNECTED;
}

void CLNetClient::createSets(NetWaitSet &set) {
    if (!m_enabled || !m_netChannel.canCommunicate())
        return;

    set.setAlert(m_netChannel.getTcpSocket());
}

void CLNetClient::init(void) {
    m_enabled = true;
}

void CLNetClient::stop(void) {
    m_enabled = false;
}

bool CLNetClient::handleUDPEvents(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    if (!m_enabled)
        return false;

    switch (msg.getType()) {
    case SV_BROADCAST_PING:
        getPingResponse(msg, addr);
        return true;
    default:
        m_state = CS_ACTIVE;
        return handleServerUDP(socket, msg, addr);
    }
}

bool CLNetClient::handleServerUDP(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    size_t readOffset = 0;

    if (!m_netChannel.isEnabled() || !m_netChannel.isUDPEnabled() ||
        addr != m_netChannel.getAddressUDP()) // why sending udp packets to the client ? who are you ?
        return false;

    if (!m_netChannel.readDatagram(socket, msg, readOffset))
        return true;

    if (m_netRecord.isRecording())
        m_netRecord.update(msg);

    switch (msg.getType()) {
    case SV_SNAPSHOT:
        pushIncommingDataAck(msg, readOffset);
        break;
    default:
        pushIncommingData(msg, readOffset);
        break;
    }
    return true;
}

bool CLNetClient::handleTCPEvents(const NetWaitSet &set) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return false;

    auto &sock = m_netChannel.getTcpSocket();
    if (!set.isSignaled(sock))
        return false;

    TCPMessage msg(0);
    if (!m_netChannel.readStream(msg))
        return false;

    if (m_netChannel.isDisconnected()) {
        disconnectFromServer(Event::DT_WANTED); /* ensure proper disconnection */
        return true;
    }

    if (m_netRecord.isRecording())
        m_netRecord.update(msg);

    return handleServerTCP(msg);
}

bool CLNetClient::handleServerTCP(const TCPMessage &msg) {
    switch (msg.getType()) {
    case SV_INIT_CONNECTON:
        TCPSV_ClientInit recvData;
        msg.readData<TCPSV_ClientInit>(recvData);

        m_netChannel.setChallenge(recvData.challenge);
        m_connectionState = CON_AUTHORIZING;

        m_netChannel.createUdpAddress(recvData.udpPort);

        NET::getEventManager().invokeCallbacks(Event::CT_OnServerConnect, 0);

        {
            TCPCL_ConnectInformation sendData = {.udpPort = m_socketUdp.getPort()};
            auto sendMsg = TCPMessage(CL_CONNECT_INFORMATION);
            sendMsg.writeData<TCPCL_ConnectInformation>(sendData);
            m_netChannel.sendStream(sendMsg);
        }

        return true;
    case SV_YOU_ARE_READY:
        m_state = CS_PRIMED;
        m_connectionState = CON_ACTIVE;
        NET::getEventManager().invokeCallbacks(Event::CT_OnServerReady, 0);
        return true;
    default:
        pushIncommingStream(msg, 0);
        break;
    }
    return true;
}

void CLNetClient::checkTimeouts(void) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return;

    if (m_state >= CS_ACTIVE && m_netChannel.isTimeout()) {
        std::cout << "CL: timeout" << std::endl;
        disconnectFromServer(Event::DT_TIMEOUT);
    }
}

void CLNetClient::getPingResponse(const UDPMessage &msg, const Address &addr) {
    UDPSV_PingResponse data;
    msg.readData<UDPSV_PingResponse>(data);

    std::unique_ptr<Address> addrPtr;
    if (addr.getType() == AT_IPV4)
        addrPtr = std::make_unique<AddressV4>(static_cast<const AddressV4 &>(addr));
    else if (addr.getType() == AT_IPV6)
        addrPtr = std::make_unique<AddressV6>(static_cast<const AddressV6 &>(addr));

    Event::PingInfo pinginfo = {addrPtr->toString(),
                                addrPtr->getPort(),
                                data.currentPlayers,
                                data.maxPlayers,
                                data.os,
                                Time::Clock::milliseconds() - m_pingSendTime};
    NET::getEventManager().invokeCallbacks(Event::CT_OnPingResult, pinginfo);

    m_pingedServers.push_back({data, std::move(addrPtr)});
}

void CLNetClient::pingLanServers(void) {
    m_pingedServers.clear();
    m_pingSendTime = Time::Clock::milliseconds();

    for (uint16_t port = DEFAULT_PORT; port < DEFAULT_PORT + MAX_TRY_PORTS; port++) {
        auto message = UDPMessage(0, CL_BROADCAST_PING);

        if (m_addrType == AT_IPV4)
            m_socketUdp.send(message, AddressV4(AT_BROADCAST, port));
        else if (m_addrType == AT_IPV6)
            m_socketUdp.send(message, AddressV6(AT_MULTICAST, port));
    }
}

bool CLNetClient::sendDatagram(UDPMessage &msg) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return false;

    return m_netChannel.sendDatagram(m_socketUdp, msg);
}

/** Net Queue **/

bool CLNetClient::pushData(const UDPMessage &msg) {
    return m_packOutData.fullpush(msg, 0);
}
bool CLNetClient::pushStream(const TCPMessage &msg) {
    return m_tcpOut.push(std::make_unique<TCPMessage>(msg), 0);
}

bool CLNetClient::popIncommingData(UDPMessage &msg, size_t &readCount, bool shouldAck) {
    if (shouldAck)
        return m_packInDataAck.pop(msg, readCount, msg.getType());
    return m_packInData.pop(msg, readCount, msg.getType());
}

bool CLNetClient::popIncommingStream(TCPMessage &msg, size_t &readCount) {
    return m_tcpIn.pop(msg, readCount);
}

bool CLNetClient::retrieveWantedOutgoingData(UDPMessage &msg, size_t &readCount) {
    return m_packOutData.pop(msg, readCount);
}
bool CLNetClient::retrieveWantedOutgoingStream(TCPMessage &msg, size_t &readCount) {
    return m_tcpOut.pop(msg, readCount);
}

bool CLNetClient::pushIncommingDataAck(const UDPMessage &msg, size_t readCount) {
    return m_packInDataAck.fullpushlast(msg, readCount);
}

bool CLNetClient::pushIncommingData(const UDPMessage &msg, size_t readCount) {
    return m_packInData.push(msg, readCount);
}

bool CLNetClient::pushIncommingStream(const TCPMessage &msg, size_t readCount) {
    return m_tcpIn.push(std::make_unique<TCPMessage>(msg), readCount);
}

/***************/

bool CLNetClient::sendPackets(void) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return false;

    size_t byteSent = 0;
    size_t outDataSz = m_packOutData.size();

    for (size_t i = 0; i < outDataSz; i++) {
        UDPMessage msg(0, 0);
        size_t readOffset;
        if (!retrieveWantedOutgoingData(msg, readOffset))
            return false;

        size_t size = msg.getSize();
        if (!sendDatagram(msg))
            return false; /* how */
        byteSent += size;
    }

    size_t outStreamSz = m_tcpOut.size();
    for (size_t i = 0; i < outStreamSz; i++) {
        TCPMessage msg(0);
        size_t readOffset;
        if (!retrieveWantedOutgoingStream(msg, readOffset))
            return false;

        if (!m_netChannel.sendStream(msg))
            return false;
    }
    return true;
}

void CLNetClient::refreshSnapshots(void) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return;

    m_netChannel.reloadAck();
}

} // namespace Network
