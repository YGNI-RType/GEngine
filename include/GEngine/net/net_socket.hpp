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
/**
 * @typedef SOCKET
 * @brief A typedef for an integer representing a network socket. Since windows uses this type, we use it for
 * cross-platform compatibility.
 */
typedef int SOCKET;
#endif

namespace Network {

////////////////////////////////////////

class ASocket {

public:
#ifdef _WIN32
    /**
     * @brief Static variable to hold the data for Windows Sockets initialization.
     *
     * This variable is used to store information about the Windows Sockets implementation.
     * It is initialized by the WSAStartup function and contains details such as the version
     * of the Windows Sockets specification that the Windows Sockets DLL supports.
     */
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
    /**
     * @brief Initializes the necessary libraries for network operations.
     *
     * This function sets up any required libraries or resources needed for
     * network communication. It should be called before any network operations
     * are performed to ensure that all dependencies are properly initialized.
     * For now, it's only winsock2 for Windows.
     */
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

/**
 * @class ANetSocket
 * @brief Abstract base class for network sockets.
 *
 * This class provides a base interface for network socket operations,
 * including getting and setting the port, blocking mode, and address translation.
 * It is designed to be inherited by specific socket implementations.
 *
 * @note This class cannot be copied, but it can be moved.
 */
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

/**
 * @class SocketUDP
 * @brief A class representing a UDP socket.
 *
 * The SocketUDP class provides functionalities for creating and managing UDP sockets.
 * It supports both IPv4 and IPv6 addresses and allows sending and receiving UDP messages.
 *
 * @note This class is non-copyable but movable.
 */
class SocketUDP : public ANetSocket {
public:
    SocketUDP() = default;
    /**
     * @brief Constructs a UDP socket.
     *
     * @param ip The IP address to bind the socket to.
     * @param port The port number to bind the socket to.
     * @param block If true, the socket will operate in blocking mode. Defaults to false (non-blocking mode).
     */
    SocketUDP(const IP &ip, uint16_t port, bool block = false);
    /**
     * @brief Constructs a new SocketUDP object.
     *
     * @param port The port number to bind the socket to.
     * @param ipv6 A boolean indicating whether to use IPv6 (true) or IPv4 (false).
     * @param block A boolean indicating whether the socket should be blocking (true) or non-blocking (false). Default
     * is false.
     */
    SocketUDP(uint16_t port, bool ipv6, bool block = false);

    SocketUDP(const SocketUDP &other) = delete;
    SocketUDP &operator=(const SocketUDP &) = delete;
    SocketUDP(SocketUDP &&other);
    SocketUDP &operator=(SocketUDP &&other);

    ~SocketUDP() = default;

    /**
     * @brief Initializes the network socket. Used by both constructors
     *
     * @param block A boolean indicating whether the socket should be blocking or non-blocking.
     * @param port The port number to bind the socket to.
     */
    void init(bool block, uint16_t port);

    /* return the nb bytes sent */
    size_t send(const UDPMessage &msg, const Address &addr) const;

    /* this seems redundant, but to avoid any heap calls, this is necessary */
    bool receiveV4(UDPMessage &msg, AddressV4 &ip) const;
    bool receiveV6(UDPMessage &msg, AddressV6 &ip) const;

private:
    /**
     * @brief Receives a UDP message.
     *
     * This function receives a UDP message and stores the sender's address and the message in the provided parameters.
     * IPv6 and IPv4 compatible.
     *
     * @param addr A pointer to a sockaddr structure that will hold the address of the sender.
     * @param sMsg A reference to a UDPSerializedMessage object where the received message will be stored.
     * @param len A pointer to a socklen_t variable that specifies the size of the address structure.
     * @return true if the message was successfully received, false otherwise.
     */
    bool receive(struct sockaddr *addr, UDPSerializedMessage &sMsg, socklen_t *len) const;
};

////////////////////////////////////////

class SocketTCP;

/**
 * @class SocketTCPMaster
 * @brief A class representing a master TCP socket.
 *
 * The SocketTCPMaster class is responsible for managing a master TCP socket.
 * It provides constructors for initializing the socket with an IP address and port,
 * or just a port with an option for IPv6. The class also supports move semantics
 * but explicitly deletes copy semantics to prevent sharing of static fields.
 * It's used for creating a "accept" socket.
 */
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

/**
 * @class SocketTCP
 * @brief A class representing a TCP socket.
 *
 * The SocketTCP class provides functionalities for creating and managing TCP sockets.
 * It supports operations such as sending and receiving messages, both fully and partially.
 * The class also handles different event types and socket states.
 *
 * @note This class cannot be copied, only moved.
 */
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

    /**
     * @brief Sends a TCP message.
     *
     * This function sends the provided TCPMessage over the network.
     *
     * @param msg The TCPMessage object to be sent.
     * @return true if the message was sent successfully.
     * @return false if there was an error sending the message.
     */
    bool send(const TCPMessage &msg) const;
    /**
     * @brief Receives a TCP message.
     *
     * This function receives a TCP message and stores it in the provided TCPMessage object.
     *
     * @param msg A reference to a TCPMessage object where the received message will be stored.
     */
    void receive(TCPMessage &msg, TCPSerializedMessage &tmpBuffer, size_t &recvSz) const;
    /**
     * @brief Sends a partial TCP message.
     *
     * This function sends a specified portion of a TCP message starting from a given offset.
     *
     * @param msg The TCPMessage object containing the data to be sent.
     * @param sizeToSend The number of bytes to send from the message.
     * @param offset The offset from which to start sending the data. This parameter will be updated to reflect the new
     * offset after sending.
     * @return true if the data was successfully sent, false otherwise.
     */
    bool sendPartial(const TCPMessage &msg, size_t sizeToSend, size_t &offset) const;
    /**
     * @brief Receives a partial TCP serialized message.
     *
     * This function attempts to receive a portion of a TCP serialized message.
     * It reads up to the specified size and updates the offset accordingly.
     *
     * @param msg The TCP serialized message to receive data into.
     * @param size The maximum number of bytes to receive.
     * @param offset The current offset in the message buffer, which will be updated.
     * @return true if the partial message was successfully received, false otherwise.
     */
    bool receivePartial(TCPSerializedMessage &msg, size_t size, size_t &offset) const;

    const EventType getEventType(void) const {
        return m_eventType;
    }
    const bool isNotReady(void) const {
        return m_notReady;
    }

private:
    /**
     * @brief Receives a reliable TCP message into the provided buffer.
     *
     * This function attempts to receive a TCP message and store it in the provided buffer.
     * It ensures that the message is received reliably, handling any necessary retries or
     * error checking to ensure the message is complete and accurate.
     *
     * @param buffer A pointer to a TCPSerializedMessage where the received data will be stored.
     * @param size The size of the buffer in bytes.
     * @param offset The offset in the buffer where the received data should be stored.
     * @return The number of bytes successfully received and stored in the buffer.
     */
    std::size_t receiveReliant(TCPSerializedMessage *buffer, std::size_t size, size_t offset, size_t &receivedTotal) const;
    /**
     * @brief Sends a reliable TCP message.
     *
     * This function sends a reliable TCP message starting from a given offset.
     *
     * @param msg Pointer to the TCPSerializedMessage to be sent.
     * @param msgDataSize The size of the message data to be sent.
     * @param offset The offset from which to start sending the message.
     * @return The number of bytes successfully sent.
     */
    std::size_t sendReliant(const TCPSerializedMessage *msg, std::size_t msgDataSize, size_t offset) const;

    EventType m_eventType = READ;
    bool m_notReady = true;
};

/**
 * @brief Opens a TCP socket on the specified IP address and port.
 *
 * This function creates and opens a TCP socket using the provided IP address
 * and port number. It returns a SocketTCPMaster object that can be used to
 * manage the socket.
 *
 * @param ip The IP address to bind the socket to.
 * @param wantedPort The port number to bind the socket to.
 * @return SocketTCPMaster The created and opened TCP socket.
 */
SocketTCPMaster openSocketTcp(const IP &ip, uint16_t wantedPort);
/**
 * @brief Opens a UDP socket with the specified IP address and port.
 *
 * @param ip The IP address to bind the socket to.
 * @param wantedPort The port number to bind the socket to.
 * @return SocketUDP The opened UDP socket.
 */
SocketUDP openSocketUdp(const IP &ip, uint16_t wantedPort);
/**
 * @brief Opens a TCP socket on the specified port.
 *
 * This function attempts to open a TCP socket on the given port. If the port is already in use,
 * it will attempt to find an available port. The function supports both IPv4 and IPv6 based on the
 * ipv6 parameter.
 *
 * @param wantedPort Reference to the port number to open. If the port is unavailable, this value
 *                   may be modified to reflect the actual port number used.
 * @param ipv6 Boolean flag indicating whether to use IPv6 (true) or IPv4 (false).
 * @return SocketTCPMaster An object representing the opened TCP socket.
 */
SocketTCPMaster openSocketTcp(uint16_t &wantedPort, bool ipv6);
/**
 * @brief Opens a UDP socket.
 *
 * This function creates and opens a UDP socket on the specified port.
 *
 * @param wantedPort The port number on which the socket should be opened.
 *                   This parameter is passed by reference and may be modified
 *                   to reflect the actual port number used.
 * @param ipv6 A boolean flag indicating whether to use IPv6 (true) or IPv4 (false).
 * @return SocketUDP The created UDP socket.
 */
SocketUDP openSocketUdp(uint16_t &wantedPort, bool ipv6);

} // namespace Network
