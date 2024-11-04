/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_client
*/

#pragma once

#include "events/socket_event.hpp"
#include "net_channel.hpp"
#include "net_queue.hpp"
#include "net_queue_heap.hpp"
#include "net_wait.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Network {

#define MAX_SZ_NAME 64
#define PACKET_BACKUP 32

/**
 * @class NetClientInfo
 * @brief This class represents the information of a network client.
 *
 * The NetClientInfo class encapsulates various attributes related to a network client,
 * including the client's name, request rate, snapshot rate, and whether high-frequency
 * data transmission is enabled.
 */
class NetClientInfo {
public:
    NetClientInfo() = default;
    ~NetClientInfo() = default;

    void setName(const std::string &name);
    const std::string &getName(void) const;

private:
    char m_name[MAX_SZ_NAME];

    uint16_t rate;             /* rate of requests, if lan don't care */
    uint16_t snapshotRate;     /* rate of sending snapshots (msec) */
    bool transmitHighFreqData; /* voip, webcam feed etc...*/
};

/* Since the engine and the snapshots are not defined in advance, we store them in heap with a predefined size */
class NetClientSnapshot {
public:
    NetClientSnapshot(size_t size);
    ~NetClientSnapshot() = default;

private:
    std::vector<byte_t> m_data;
};

/**
 * @class NetClient
 * @brief Manages network client operations including sending and receiving data over TCP and UDP.
 *
 * The NetClient class handles the communication with a network server using both TCP and UDP protocols.
 * It provides methods to send and receive packets, manage connection states, and handle network events.
 * The class ensures thread safety for certain operations and maintains internal queues for incoming and outgoing data.
 *
 * @note This class is designed to be thread-safe for specific operations.
 *
 * @todo Adjust the sizes of the internal queues based on average data size requirements.
 */
class NetClient {
public:
    NetClient(std::unique_ptr<Address> addr, SocketTCP &&socket, SocketUDP &socketudp, Event::SocketEvent &socketEvent);
    ~NetClient() = default;

    NetChannel &getChannel(void) {
        return m_channel;
    }

    SocketUDP &getSocketUdp(void) {
        return m_socketUdp;
    }

    bool isDisconnected(void) const {
        return m_channel.isDisconnected();
    }

    bool isTimeout(void) const;

    /**
     * @brief Handles TCP events based on the provided NetWaitSet.
     *
     * This function processes TCP events that are ready for handling as indicated
     * by the NetWaitSet. It performs necessary actions to manage these events.
     *
     * @param set A reference to a NetWaitSet object that contains the set of TCP events
     *            to be handled.
     * @return true if the events were handled successfully, false otherwise.
     */
    bool handleTCPEvents(const NetWaitSet &set);
    bool handleClientStream(void);
    /**
     * @brief Handles the incoming datagram from a client.
     *
     * This function processes the datagram received from a client through the specified UDP socket.
     * It interprets the message and performs necessary actions based on the content of the message.
     *
     * @param socket The UDP socket through which the datagram is received.
     * @param msg The message received from the client.
     * @return true if the datagram was handled successfully, false otherwise.
     */
    bool handleClientDatagram(SocketUDP &socket, UDPMessage &msg);

public:
    bool sendPackets(void);
    bool sendDatagram(UDPMessage &msg);
    void sendStream(const TCPMessage &msg);

public:
    /** THREAD SAFE **/

    bool pushData(const UDPMessage &msg, bool shouldAck);
    bool pushStream(const TCPMessage &msg);
    bool popIncommingData(UDPMessage &msg, size_t &readCount);
    bool popIncommingStream(TCPMessage &msg, size_t &readCount);
    size_t getSizeIncommingData(void) const {
        return m_packInData.size();
    }

    /**
     * @brief Retrieves the current ping timestamp.
     *
     * This function returns the ping timestamp from the network channel.
     *
     * @return uint16_t The current ping timestamp.
     */
    uint16_t getPing_TS(void) const {
        return m_channel.getPing_TS();
    }
    /**
     * @brief Retrieves the address in a thread-safe manner.
     *
     * This function calls the getAddress_TS method of the m_channel object
     * to obtain the address. The operation is performed in a thread-safe
     * manner to ensure data integrity when accessed concurrently.
     *
     * @return A string containing the address.
     */
    std::string getAddress_TS(void) const {
        return m_channel.getAddress_TS();
    }

private:
    bool retrieveWantedOutgoingData(UDPMessage &msg, size_t &readCount);
    bool retrieveWantedOutgoingDataAck(UDPMessage &msg, size_t &readCount);
    bool retrieveWantedOutgoingStream(TCPMessage &msg, size_t &readCount);
    bool pushIncommingData(const UDPMessage &msg, size_t readCount);
    bool pushIncommingStream(const TCPMessage &msg, size_t readCount);

private:
    NetChannel m_channel;
    NetClientInfo m_info;

    int m_challenge = -1; /* challenge for authoring / avoid ddos */
    clientState m_state = CS_FREE;
    connectionState m_connectionState = CON_UNINITIALIZED;

    /* todo : change based on average size */
    NetQueue<UDPMessage, 24, 1400> m_packInData;     /* todo : get the size of Usercmd + own voip / */
    NetQueue<UDPMessage, 32, 1400> m_packOutData;    /* voiceip etc.. */
    NetQueue<UDPMessage, 1, 17000> m_packOutDataAck; /* snapshot */

    NetQueueHeap<TCPMessage, 5> m_tcpIn;
    NetQueueHeap<TCPMessage, 5> m_tcpOut;

    /* sends CMD, not any data */

    SocketUDP &m_socketUdp;

    uint16_t ping = 0;
};
} // namespace Network
