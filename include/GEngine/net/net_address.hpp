/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_address
*/

#pragma once

#include "net_common.hpp"

#include <array>

#ifdef _WIN32

typedef uint32_t in_addr_t;

#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace Network {
/**
 * @class Address
 * @brief Abstract base class representing a network address.
 *
 * This class provides an interface for handling different types of network addresses.
 * It includes methods for converting the address to a sockaddr structure, checking if
 * the address is a local area network (LAN) address, and converting the address to a string.
 *
 * @protected
 * @typedef ipv4_t
 * @brief Type alias for an IPv4 address represented as an array of 4 bytes.
 *
 * @typedef ipv6_t
 * @brief Type alias for an IPv6 address represented as an array of 16 bytes.
 *
 * @protected
 * @var AddressType m_type
 * @brief The type of the address.
 *
 * @var uint16_t m_port
 * @brief The port number of the address.
 *
 * @var uint32_t m_mask
 * @brief The mask of the address.
 */
class Address {

protected:
    typedef std::array<byte_t, 4> ipv4_t;
    typedef std::array<byte_t, 16> ipv6_t;

public:
    virtual ~Address() = default;
    /**
     * @brief Converts the network address to a sockaddr structure.
     *
     * This pure virtual function must be implemented by derived classes
     * to provide the appropriate conversion of the network address to
     * a sockaddr structure.
     *
     * @param addr Pointer to a sockaddr structure where the converted
     *             address will be stored.
     */
    virtual void toSockAddr(sockaddr *addr) const = 0;
    /**
     * @brief Checks if the network address is a local area network (LAN) address.
     *
     * This function determines whether the network address is within the range
     * of addresses designated for local area networks (LANs).
     *
     * @return true if the address is a LAN address, false otherwise.
     */
    virtual bool isLanAddr(void) const = 0;
    virtual std::string toString(void) const = 0;

    AddressType getType() const {
        return m_type;
    }
    uint16_t getPort() const {
        return m_port;
    }
    uint32_t getMask() const {
        return m_mask;
    }

    bool operator==(const Address &other) const;

    void setMask(uint32_t mask) {
        m_mask = mask;
    };

protected:
    virtual const byte_t *getData() const = 0;
    Address(AddressType type, uint16_t port, uint32_t mask = 0)
        : m_type(type)
        , m_port(port)
        , m_mask(mask) {
    }

    /**
     * @brief Compares two network addresses for equality based on a given mask.
     *
     * @param addr1 Pointer to the first network address.
     * @param addr2 Pointer to the second network address.
     * @param mask The mask to apply when comparing the addresses.
     * @return true if the addresses are equal based on the mask, false otherwise.
     */
    bool isEqual(const byte_t *addr1, const byte_t *addr2, uint32_t mask) const;

    AddressType m_type;
    uint16_t m_port;
    uint32_t m_mask;
};

/**
 * @class AddressV4
 * @brief Represents an IPv4 network address.
 *
 * This class provides functionalities to handle IPv4 addresses, including
 * conversion to socket address structures, checking if the address is a LAN
 * address, and converting the address to a string representation.
 *
 * @note This class inherits from the Address class.
 */

/**
 * @var AddressV4::m_address
 * @brief Stores the IPv4 address.
 *
 * This member variable holds the IPv4 address in a custom ipv4_t type.
 */
class AddressV4 : public Address {

public:
    AddressV4(AddressType type, uint16_t port, const ipv4_t &address);
    AddressV4(AddressType type, uint16_t port);
    AddressV4(AddressType type, uint16_t port, in_addr_t ip);
    AddressV4(AddressType type, const std::string &ip, uint16_t port);
    ~AddressV4() = default;

    const ipv4_t &getAddress() const {
        return m_address;
    }

    void toSockAddr(sockaddr *addr) const override final;
    bool isLanAddr(void) const override final;
    std::string toString(void) const override final;

private:
    const byte_t *getData() const override final {
        return m_address.data();
    }

    ipv4_t m_address;
};

/**
 * @class AddressV6
 * @brief Represents an IPv6 network address.
 *
 * The AddressV6 class provides functionalities to handle IPv6 addresses,
 * including conversion to socket address structures, checking if the address
 * is a LAN address, and converting the address to a string representation.
 *
 * @note This class inherits from the Address class.
 *
 * @var AddressV6::m_address
 * The IPv6 address stored as an ipv6_t type.
 *
 * @var AddressV6::m_scopeId
 * The scope ID associated with the IPv6 address.
 */
class AddressV6 : public Address {

public:
    AddressV6(AddressType type, uint16_t port, const ipv6_t &address, uint64_t scopeId);
    AddressV6(AddressType type, uint16_t port);
    AddressV6(AddressType type, uint16_t port, in6_addr ip, uint32_t scopeId);
    AddressV6(AddressType type, const std::string &ip, uint16_t port);
    ~AddressV6() = default;

    const ipv6_t &getAddress() const {
        return m_address;
    }

    uint64_t getScopeId() const {
        return m_scopeId;
    };

    void toSockAddr(sockaddr *addr) const override final;
    bool isLanAddr(void) const override final;
    std::string toString(void) const override final;

private:
    const byte_t *getData() const override final {
        return m_address.data();
    }

    ipv6_t m_address;
    uint64_t m_scopeId;
};

/**
 * @class UnknownAddress
 * @brief Represents an unknown network address.
 *
 * This class encapsulates an unknown network address, providing methods to
 * retrieve and manipulate the address type and its underlying sockaddr structure.
 *
 * @var UnknownAddress::m_type
 * The type of the address (e.g., IPv4, IPv6, or none).
 *
 * @var UnknownAddress::m_addr
 * The storage for the address, capable of holding either IPv4 or IPv6 addresses.
 *
 * @var UnknownAddress::m_len
 * The length of the address stored in m_addr.
 */
class UnknownAddress {

public:
    UnknownAddress() = default;
    ~UnknownAddress() = default;

    AddressType getType(void) const {
        return m_type;
    }
    void updateType(void);
    AddressV6 getV6() const;
    AddressV4 getV4() const;

    sockaddr *getAddr(void) {
        return reinterpret_cast<sockaddr *>(&m_addr);
    }
    socklen_t &getLen(void) {
        return m_len;
    }

private:
    AddressType m_type = AT_NONE;
    sockaddr_storage m_addr = {0};
    socklen_t m_len = sizeof(m_addr);
};

} // namespace Network
