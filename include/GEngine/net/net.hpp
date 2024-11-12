/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#pragma once

#include "cl_net_client.hpp"
#include "msg.hpp"
#include "net_address.hpp"
#include "net_client.hpp"
#include "net_common.hpp"
#include "net_event.hpp"
#include "net_record.hpp"
#include "net_server.hpp"
#include "net_socket.hpp"
#include "net_wait.hpp"

#include "GEngine/utils/libapi.hpp"

#include <thread>
#include <vector>

namespace Network {

/**
 * @class NET
 * @brief The NET class provides network functionalities including server and client initialization,
 *        handling network events, and managing local IP addresses. Very similar to quake3's network.
 *
 * This class encapsulates various network operations such as initializing servers and clients,
 * handling UDP and TCP sockets, managing network events, and pinging servers. It also provides
 * utility functions to manage local IP addresses and handle network timeouts.
 *
 * @note This class contains both public and private static members and methods to manage network operations.
 */
class NET {
private:
    API static NetWait mg_wait;

    static SocketUDP mg_socketUdp;
    static SocketTCPMaster mg_socketListenTcp;
    static SocketUDP mg_socketUdpV6;
    static SocketTCPMaster mg_socketListenTcpV6;

    API static NetServer mg_server;
    API static CLNetClient mg_client;

    static std::vector<IP> g_localIPs;

    API static std::atomic_bool mg_aEnable;
    static std::mutex mg_mutex;
    static std::thread mg_networkThread;

    API static uint16_t mg_currentUnusedPortTCP;
    API static uint16_t mg_currentUnusedPortUDP;

    API static Event::Manager mg_eventManager;

    /* Init everything */
public:
    static bool init(void);
    static void stop(void);
    static bool start(void);

    static bool initServer(void);
    static bool initClient(void);

    static const std::vector<IP> &getLocalAddresses(void);
    static SocketUDP createUDPSocket(const IP &ip);
    static bool destroyUDPSocket(SocketUDP &socket);
private:
    static void fetchLocalAddresses(void);
    static void addLocalAddress(char *ifname, struct sockaddr *sockaddr, struct sockaddr *netmask, bool isLoopback);
    static void sortLocalAddresses(void);
    static bool isLanAddress(const Address &addr);

    static bool mg_hasListLocalAddress;
    /*********************/

    /* Usage of "select" */
public:
    static bool sleep(uint32_t ms);
    static bool handleEvents(const NetWaitSet &set);
    static bool handleUdpEvent(SocketUDP &socket, UDPMessage &msg, const Address &addr);
    static bool handleUdpIPv4(void);
    static bool handleUdpIPv6(void);
    static void handleTimeouts(void);

private:
    static void createSets(NetWaitSet &readSet);
    /*********************/

    /* Ping servers */
public:
    static void pingServers(void);

    static CLNetClient &getClient(void) {
        return mg_client;
    }
    static NetServer &getServer(void) {
        return mg_server;
    }

    static Event::Manager &getEventManager(void) {
        return mg_eventManager;
    }

    static NetRecord &getRecord(void) {
        return mg_client.getRecord();
    }

public:
    /* todo : temp*/
    static SocketUDP &getSocketUdp(void) {
        return mg_socketUdp;
    }

private:
};
} // namespace Network
