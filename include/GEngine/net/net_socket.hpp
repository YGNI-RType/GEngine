/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socket
*/

#pragma once

#include "msg.hpp"
#include "net_address.hpp"
#include "net_common.hpp"

#include <cstdint>
#include <string>
#include <utility>

#ifndef _WIN32
#include <netinet/in.h> // Add this line to include the header file that defines sockaddr_in
#include <sys/socket.h>
#include <sys/types.h>
#endif

#ifndef _WIN32
typedef int SOCKET;
#endif

namespace Network {

////////////////////////////////////////

class ASocket {

public:
#ifdef _WIN32
    static WSADATA winsockdata;
#endif

    SOCKET getSocket(void) const {
        return m_sock;
    }

#ifdef NET_USE_HANDLE
    HANDLE getHandle(void) const {
        return m_handle;
    }

    void createHandle(void);
#endif

public:
    static void initLibs(void);

    ASocket(const ASocket &other) = delete;
    ASocket &operator=(const ASocket &other) = delete;
    ASocket(ASocket &&other);
    ASocket &operator=(ASocket &&other);

protected:
    ASocket() = default;
    virtual ~ASocket();

public:
    int socketClose(void);

protected:
    SOCKET m_sock = -1;
#ifdef NET_USE_HANDLE
    HANDLE m_handle = INVALID_HANDLE_VALUE;
#endif
};

class ANetSocket : public ASocket {
public:
    uint16_t getPort(void) const {
        return m_port;
    }

    void setBlocking(bool blocking);
    bool isBlocking(void) const;

    static void translateAutomaticAddressing(struct sockaddr_storage &addr, uint16_t port, bool ipv6);

    ANetSocket(const ANetSocket &other) = delete;
    ANetSocket &operator=(const ANetSocket &other) = delete;
    ANetSocket(ANetSocket &&other);
    ANetSocket &operator=(ANetSocket &&other);

protected:
    ANetSocket() = default;
    virtual ~ANetSocket() = default;

    uint16_t m_port = -1;
};

////////////////////////////////////////

class SocketUDP : public ANetSocket {
public:
    SocketUDP() = default;
    SocketUDP(const IP &ip, uint16_t port, bool block = false);
    SocketUDP(uint16_t port, bool ipv6, bool block = false);

    SocketUDP(const SocketUDP &other) = delete;
    SocketUDP &operator=(const SocketUDP &) = delete;
    SocketUDP(SocketUDP &&other);
    SocketUDP &operator=(SocketUDP &&other);

    ~SocketUDP() = default;

    void init(bool block, uint16_t port);

    /* return the nb bytes sent */
    size_t send(const UDPMessage &msg, const Address &addr) const;

    /* this seems redundant, but to avoid any heap calls, this is necessary */
    bool receiveV4(UDPMessage &msg, AddressV4 &ip) const;
    bool receiveV6(UDPMessage &msg, AddressV6 &ip) const;

private:
    bool receive(struct sockaddr *addr, UDPSerializedMessage &sMsg, socklen_t *len) const;
};

////////////////////////////////////////

class SocketTCP;

class SocketTCPMaster : public ANetSocket {
public:
    SocketTCPMaster() = default;
    SocketTCPMaster(const IP &ip, uint16_t port);
    SocketTCPMaster(uint16_t port, bool ipv6);

    // don't share anything, all the fields are static
    SocketTCPMaster(const SocketTCPMaster &other) = delete;
    SocketTCPMaster &operator=(const SocketTCPMaster &) = delete;
    SocketTCPMaster(SocketTCPMaster &&other);
    SocketTCPMaster &operator=(SocketTCPMaster &&other);

    ~SocketTCPMaster() = default;

    SocketTCP accept(UnknownAddress &unkwAddr) const;
};

class SocketTCP : public ANetSocket {
public:
    enum EventType {
        NONE = 0,
        READ,
        WRITE,
    };

public:
    SocketTCP() = default;
    SocketTCP(const SocketTCPMaster &socketMaster, UnknownAddress &unkwAddr,
              bool block = false);                                          // accepts it from the socket master
    SocketTCP(const AddressV4 &addr, uint16_t tcpPort, bool block = false); // connect to the address (only for client)
    SocketTCP(const AddressV6 &addr, uint16_t tcpPort, bool block = false); // connect to the address (only for client)
    SocketTCP(const SocketTCP &other) = delete;
    SocketTCP &operator=(const SocketTCP &) = delete;
    SocketTCP(SocketTCP &&other);
    SocketTCP &operator=(SocketTCP &&other);
    ~SocketTCP() = default;

    bool send(const TCPMessage &msg) const;
    void receive(TCPMessage &msg) const;
    bool sendPartial(const TCPMessage &msg, size_t sizeToSend, size_t &offset) const;
    bool receivePartial(TCPSerializedMessage &msg, size_t size, size_t &offset) const;

    const EventType getEventType(void) const {
        return m_eventType;
    }
    const bool isNotReady(void) const {
        return m_notReady;
    }

private:
    std::size_t receiveReliant(TCPSerializedMessage *buffer, std::size_t size, size_t offset) const;
    std::size_t sendReliant(const TCPSerializedMessage *msg, std::size_t msgDataSize, size_t offset) const;

    EventType m_eventType = READ;
    bool m_notReady = true;
};

SocketTCPMaster openSocketTcp(const IP &ip, uint16_t wantedPort);
SocketUDP openSocketUdp(const IP &ip, uint16_t wantedPort);
SocketTCPMaster openSocketTcp(uint16_t &wantedPort, bool ipv6);
SocketUDP openSocketUdp(uint16_t &wantedPort, bool ipv6);

} // namespace Network
