/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_client
*/

#include "GEngine/net/net_client.hpp"
#include "GEngine/net/net.hpp"

#include <algorithm>

// temp
#include <iostream>

namespace Network {
NetClient::NetClient(std::unique_ptr<Address> addr, SocketTCP &&socket, SocketUDP &socketudp,
                     Event::SocketEvent &socketEvent)
    : m_channel(true, std::move(addr), std::move(socket))
    , m_socketUdp(socketudp)
    , m_packOutData(socketEvent)
    , m_packOutDataAck(socketEvent)
    , m_packInData(socketEvent)
    , m_tcpIn(socketEvent)
    , m_tcpOut(socketEvent)
    , m_state(CS_CONNECTED) {
}

void NetClient::createSets(NetWaitSet &set) {
    if (!m_channel.isEnabled())
        return;

    auto &socket = m_channel.getTcpSocket();
    set.setAlert(socket, [this]() { return handleClientStream(); });
}

bool NetClient::isTimeout(void) const {
    if (m_state < CS_ACTIVE)
        return false;

    return m_channel.isTimeout();
}

void NetClient::sendStream(const TCPMessage &msg) {
    if (!m_channel.isEnabled())
        return;

    m_channel.sendStream(msg);
}

bool NetClient::sendDatagram(UDPMessage &msg) {
    if (!m_channel.isEnabled())
        return false;

    return m_channel.sendDatagram(m_socketUdp, msg);
}

bool NetClient::handleClientStream(void) {
    TCPMessage msg(0);

    if (!m_channel.readStream(msg))
        return false;

    if (m_channel.isDisconnected())
        return true;

    switch (msg.getType()) {
    case CL_CONNECT_INFORMATION: {
        TCPCL_ConnectInformation recvData;
        msg.readData<TCPCL_ConnectInformation>(recvData);

        m_channel.createUdpAddress(recvData.udpPort);

        NET::getEventManager().invokeCallbacks(Event::CT_OnClientReady, this);

        auto msg = TCPMessage(SV_YOU_ARE_READY);
        m_channel.sendStream(msg);
        m_state = CS_PRIMED;
        return true;
    }
    default:
        pushIncommingStream(msg, 0);
        break;
    }
    return true;
}

bool NetClient::handleClientDatagram(SocketUDP &socket, UDPMessage &msg) {
    size_t readOffset = 0;

    if (!m_channel.isUDPEnabled() || !m_channel.readDatagram(socket, msg, readOffset))
        return false;

    switch (msg.getType()) {
    default:
        m_state = CS_ACTIVE;
        return pushIncommingData(msg, readOffset);
    }
    return false;
}

bool NetClient::handleTCPEvents(const NetWaitSet &set) {
    auto &socket = m_channel.getTcpSocket();
    if (!set.isSignaled(socket))
        return false;

    return handleClientStream();
}

/** Net Queue **/

bool NetClient::pushData(const UDPMessage &msg, bool shouldAck) {
    if (shouldAck)
        return m_packOutDataAck.fullpush(msg, 0);
    return m_packOutData.push(msg, 0);
}

bool NetClient::pushStream(const TCPMessage &msg) {
    return m_tcpOut.push(std::make_unique<TCPMessage>(msg), 0);
}

bool NetClient::popIncommingData(UDPMessage &msg, size_t &readCount) {
    return m_packInData.pop(msg, readCount, msg.getType());
}

bool NetClient::popIncommingStream(TCPMessage &msg, size_t &readCount) {
    return m_tcpIn.pop(msg, readCount);
}

bool NetClient::retrieveWantedOutgoingData(UDPMessage &msg, size_t &readCount) {
    return m_packOutData.pop(msg, readCount);
}

bool NetClient::retrieveWantedOutgoingStream(TCPMessage &msg, size_t &readCount) {
    return m_tcpOut.pop(msg, readCount);
}

bool NetClient::retrieveWantedOutgoingDataAck(UDPMessage &msg, size_t &readCount) {
    return m_packOutDataAck.pop(msg, readCount);
}

bool NetClient::pushIncommingData(const UDPMessage &msg, size_t readCount) {
    return m_packInData.fullpush(msg, readCount);
}

bool NetClient::pushIncommingStream(const TCPMessage &msg, size_t readCount) {
    return m_tcpIn.push(std::make_unique<TCPMessage>(msg), readCount);
}

/***************/

bool NetClient::sendPackets(void) {
    if (isDisconnected() || !m_channel.isEnabled())
        return false;

    size_t byteSent = 0;

    size_t nbPakcetUdp = m_packOutData.size();
    size_t nbPakcetUdpAck = m_packOutDataAck.size();

    std::vector<std::pair<size_t, bool (Network::NetClient::*)(Network::UDPMessage &, size_t &)>> vecFuncs = {
        std::make_pair<>(nbPakcetUdp, &NetClient::retrieveWantedOutgoingData),
        std::make_pair<>(nbPakcetUdpAck, &NetClient::retrieveWantedOutgoingDataAck)};

    while (!vecFuncs.empty()) {
        size_t readCount;
        UDPMessage msg(0, 0);
        auto retrieveFunc = vecFuncs.front();
        size_t maxPacket = retrieveFunc.first;

        for (size_t i = 0; i < maxPacket; i++) {
            if (!(this->*retrieveFunc.second)(msg, readCount))
                break;

            size_t size = msg.getSize();
            if (!sendDatagram(msg))
                return false; /* how */
            byteSent += size;
        }
        vecFuncs.erase(vecFuncs.begin());
    }

    size_t outStreamSz = m_tcpOut.size();
    for (size_t i = 0; i < outStreamSz; i++) {
        TCPMessage msg(0);
        size_t readOffset;
        if (!retrieveWantedOutgoingStream(msg, readOffset))
            return false;

        if (!m_channel.sendStream(msg))
            return false;
    }
    return true;
}

} // namespace Network
