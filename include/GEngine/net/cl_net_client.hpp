/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#pragma once

#include "events/disconnection.hpp"
#include "net_channel.hpp"
#include "net_common.hpp"
#include "net_queue.hpp"
#include "net_queue_heap.hpp"
#include "net_record.hpp"
#include "net_wait.hpp"

#include <memory>
#include <vector>

namespace Network {

struct PingResponse {
    UDPSV_PingResponse response;
    std::unique_ptr<Address> addr;
};

/**
 * @class CLNetClient
 * @brief Manages network client operations for UDP and TCP communication.
 *
 * The CLNetClient class handles the initialization, connection, and communication
 * with a server using both UDP and TCP protocols. It manages the sending and
 * receiving of packets, handles events, and maintains the state of the connection.
 *
 * @details
 * - The class supports connecting to a server using an IP address and port.
 * - It can handle both incoming and outgoing data streams.
 * - It provides mechanisms to handle timeouts and server pings.
 * - The class maintains various states to track the connection status.
 *
 * @see SocketUDP
 * @see NetChannel
 * @see NetQueue
 * @see NetQueueHeap
 * @see NetRecord
 */
class CLNetClient {
public:
    CLNetClient(SocketUDP &socketUdp, AddressType type, Event::SocketEvent &socketEvent)
        : m_socketUdp(socketUdp)
        , m_addrType(type)
        , m_netChannel(NetChannel(false, nullptr, SocketTCP()))
        , m_packOutData(socketEvent)
        , m_packInData(socketEvent)
        , m_packInDataAck(socketEvent)
        , m_tcpIn(socketEvent)
        , m_tcpOut(socketEvent) {};
    ~CLNetClient() = default;

    /**
     * @brief Initializes the network client.
     *
     * This function sets up the necessary configurations and resources
     * required for the network client to operate. It should be called
     * before any network operations are performed.
     */
    void init(void);
    /**
     * @brief Stops the network client.
     *
     * This function is responsible for stopping the network client and
     * performing any necessary cleanup operations.
     */
    void stop(void);
    bool isEnabled(void) const {
        return m_enabled;
    }
    NetRecord &getRecord(void) {
        return m_netRecord;
    }

    /**
     * @brief Connects to a server at the specified index.
     *
     * This function attempts to establish a connection to a server identified by the given index, used by the ping
     * response.
     *
     * @param index The index of the server to connect to.
     * @return true if the connection is successful, false otherwise.
     */
    bool connectToServer(size_t index);
    bool connectToServer(const std::string &ip, uint16_t port, bool block = false);
    void disconnectFromServer(Event::DisonnectType disconnectType);

    /**
     * @brief Creates and initializes the network wait sets for reading.
     *
     * This function sets up the provided NetWaitSet for monitoring network
     * events that are ready for reading. It configures the necessary file
     * descriptors and prepares the set for use in network operations.
     *
     * @param readSet A reference to the NetWaitSet that will be configured
     *                for reading network events.
     */
    void createSets(NetWaitSet &readSet);

    /**
     * @brief Handles TCP events based on the provided read set.
     *
     * This function processes TCP events by examining the given read set.
     * It performs necessary actions based on the events detected in the read set.
     *
     * @param readSet A constant reference to a NetWaitSet object that contains the set of file descriptors to be
     * monitored for reading.
     * @return true if the TCP events were handled successfully, false otherwise.
     */
    bool handleTCPEvents(const NetWaitSet &readSet);
    /**
     * @brief Handles UDP events received from a given socket.
     *
     * This function processes incoming UDP messages and performs necessary actions
     * based on the message content and the sender's address.
     *
     * @param socket The UDP socket from which the message is received.
     * @param msg The UDP message that needs to be handled.
     * @param addr The address of the sender of the UDP message.
     * @return true if the event was handled successfully, false otherwise.
     */
    bool handleUDPEvents(SocketUDP &socket, UDPMessage &msg, const Address &addr);

    /**
     * @brief Handles incoming UDP messages from the server.
     *
     * @param socket The UDP socket used for communication.
     * @param msg The UDP message received from the server.
     * @param addr The address of the server.
     * @return true if the message was handled successfully, false otherwise.
     */
    bool handleServerUDP(SocketUDP &socket, UDPMessage &msg, const Address &addr);
    /**
     * @brief Handles a sanitized UDP message from the server.
     *
     * This function processes a UDP message received from the server, starting from the specified read offset.
     *
     * @param msg The UDP message received from the server.
     * @param readOffset The offset from which to start reading the message.
     * @return true if the message was successfully handled, false otherwise.
     */
    bool handleServerUDPSanitized(const UDPMessage &msg, size_t readOffset);
    /**
     * @brief Handles the incoming TCP message from the server.
     *
     * This function processes the TCP message received from the server and performs
     * the necessary actions based on the message content.
     *
     * @param msg The TCP message received from the server.
     * @return true if the message was handled successfully, false otherwise.
     */
    bool handleServerTCP(const TCPMessage &msg);

    void checkTimeouts(void);

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

    void refreshSnapshots(void);

public:
    bool sendPackets(void);
    bool sendDatagram(UDPMessage &finishedMsg);

public:
    /**
     * @brief Pings all LAN servers to check their availability.
     *
     * This function sends a ping request to all servers on the local area network (LAN)
     * to determine their availability and response time. It is useful for discovering
     * and listing active servers within the same network.
     */
    void pingLanServers(void);
    /**
     * @brief Handles the response to a ping request.
     *
     * This function processes the incoming UDP message that contains the ping response
     * and the address from which the response was received.
     *
     * @param msg The UDP message containing the ping response.
     * @param addr The address from which the ping response was received.
     */
    void getPingResponse(const UDPMessage &msg, const Address &addr);

public:
    /** Net Queue **/

    bool pushData(const UDPMessage &msg);
    bool pushStream(const TCPMessage &msg);
    bool popIncommingData(UDPMessage &msg, size_t &readCount, bool shouldAck);
    bool popIncommingStream(TCPMessage &msg, size_t &readCount);
    size_t getSizeIncommingData(bool ack) const {
        if (ack)
            return m_packInDataAck.size();
        return m_packInData.size();
    }
    size_t getSizeIncommingData(uint8_t type, bool ack) const {
        if (ack)
            return m_packInDataAck.size(type);
        return m_packInData.size(type);
    }

    /**
     * @brief Retrieves the current ping time stamp.
     *
     * This function returns the ping time stamp from the network channel. Since this is from a different thread, the
     * net_channel is handling the safety.
     *
     * @return uint16_t The current ping time stamp.
     */
    uint16_t getPing_TS(void) const {
        return m_netChannel.getPing_TS();
    }

private:
    bool retrieveWantedOutgoingData(UDPMessage &msg, size_t &readCount);
    bool retrieveWantedOutgoingStream(TCPMessage &msg, size_t &readCount);
    bool pushIncommingData(const UDPMessage &msg, size_t readCount);
    bool pushIncommingDataAck(const UDPMessage &msg, size_t readCount);
    bool pushIncommingStream(const TCPMessage &msg, size_t readCount);

private:
    int m_challenge = -1;

    bool m_enabled = false;
    clientState m_state = CS_FREE;
    connectionState m_connectionState = CON_UNINITIALIZED;

    /* todo : change based on average size */
    NetQueue<UDPMessage, 2, 1400> m_packOutData;    /* todo : get the size of Usercmd + own voip / */
    NetQueue<UDPMessage, 32, 1400> m_packInData;    /* voiceip etc.. */
    NetQueue<UDPMessage, 2, 17000> m_packInDataAck; /* snapshot */

    NetQueueHeap<TCPMessage, 5> m_tcpIn;
    NetQueueHeap<TCPMessage, 5> m_tcpOut;

    SocketUDP &m_socketUdp;
    AddressType m_addrType;

    NetChannel m_netChannel;
    NetRecord m_netRecord;

    std::vector<PingResponse> m_pingedServers;
    uint64_t m_pingSendTime = 0;

    size_t m_lastConnectTime = 0;

    /* in bytes from data (header do not count), can be updated via cvar */
    size_t m_maxRate = 10000;
};
} // namespace Network
