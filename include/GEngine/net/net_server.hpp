/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_server
*/

#pragma once

#include "events/disconnection.hpp"
#include "events/socket_event.hpp"
#include "net_client.hpp"
#include "net_socket.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace Network {
/**
 * @class NetServer
 * @brief Manages network server operations for both IPv4 and IPv6.
 *
 * The NetServer class handles the initialization, management, and communication
 * of a network server using both TCP and UDP protocols. It supports operations
 * such as starting and stopping the server, handling new client connections,
 * sending and receiving messages, and managing client timeouts.
 *
 * @note This class currently does not support loopback clients for listen servers.
 *
 * @param socketUdpV4 Reference to the UDP socket for IPv4.
 * @param socketUdpV6 Reference to the UDP socket for IPv6.
 */
class NetServer {
public:
    NetServer(SocketUDP &socketUdpV4, SocketUDP &socketUdpV6)
        : m_socketUdpV4(socketUdpV4)
        , m_socketUdpV6(socketUdpV6) {};
    /* todo : add to support loopback client for listen servers */
    ~NetServer() = default;

    /**
     * @brief Starts the network server.
     *
     * This function initializes and starts the network server using the provided
     * local IP addresses and an unused port.
     *
     * @param g_localIPs A vector of IP addresses representing the local IPs to bind the server to.
     * @param currentUnusedPort The port number that is currently unused and will be used to start the server.
     * @return uint16_t The port number on which the server was successfully started.
     */
    uint16_t start(const std::vector<IP> &g_localIPs, uint16_t currentUnusedPort);
    /**
     * @brief Starts the server on the specified port.
     *
     * This function attempts to start the server using the provided port. If the
     * port is already in use, it will attempt to find the next available port.
     *
     * @param currentUnusedPort A reference to a uint16_t that specifies the port
     *                          to start the server on. If the specified port is
     *                          unavailable, this value will be updated to the
     *                          next available port.
     * @return uint16_t The port number on which the server was successfully started.
     */
    uint16_t start(uint16_t &currentUnusedPort);
    /**
     * @brief Stops the server.
     *
     * This function stops the server, terminating any ongoing connections
     * and releasing any resources that were allocated for the server's operation.
     */
    void stop(void);

    /**
     * @brief Creates and initializes the sets for network operations.
     *
     * This function is responsible for setting up the necessary sets for
     * network operations, specifically the read set. It prepares the
     * NetWaitSet object to be used in network communication.
     *
     * @param readSet A reference to a NetWaitSet object that will be
     *                initialized and populated with the necessary
     *                descriptors for reading.
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
     * @return true if the event was handled successfully, false otherwise.
     */
    bool handleTCPEvent(const NetWaitSet &readSet);
    bool handleTCPEvent(void);
    /**
     * @brief Handles an incoming UDP event.
     *
     * This function processes a UDP message received on the specified socket from the given address.
     *
     * @param socket The UDP socket on which the message was received.
     * @param msg The UDP message that was received.
     * @param addr The address from which the message was sent.
     * @return true if the event was handled successfully, false otherwise.
     */
    bool handleUDPEvent(SocketUDP &socket, UDPMessage &msg, const Address &addr);

    const SocketTCPMaster &getSocketV4(void) const {
        return m_socketv4;
    }
    const SocketTCPMaster &getSocketV6(void) const {
        return m_socketv6;
    }

    uint32_t getNumClients(void) const {
        return m_clients.size();
    }
    uint32_t getMaxClients(void) const;

    bool isRunning(void) const {
        return m_isRunning;
    }

    void handleNewClient(SocketTCPMaster &socket);

    void respondPingServers(const UDPMessage &msg, SocketUDP &udpsocket, const Address &addr);
    bool handleUdpMessageClients(SocketUDP &socket, UDPMessage &msg, const Address &addr);

    bool sendPackets(void);
    void sendToAllClients(UDPMessage &msg);
    void sendToClient(NetClient &client, UDPMessage &msg);

    void checkTimeouts(void);
    void disconnectClient(NetClient *client, Event::DisonnectType info);

    std::string getAddress_TS(void) const;
    uint16_t getPort_TS(void) const;

private:
    bool m_isRunning = false;

    SocketTCPMaster m_socketv4;
    SocketTCPMaster m_socketv6;

    SocketUDP &m_socketUdpV4;
    SocketUDP &m_socketUdpV6;

    std::vector<std::shared_ptr<NetClient>> m_clients;
};

} // namespace Network
