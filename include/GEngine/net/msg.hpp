/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#pragma once

#include "net_common.hpp"
#include "structs/msg_tcp_structs.hpp"
#include "structs/msg_udp_structs.hpp"
#include "utils/pack.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace Network {

#define FRAGMENT_SIZE (MAX_PACKETLEN - 100)
#define FRAGMENT_BIT (1U << 31)
#define PACKET_HEADER 10 // two ints and a short

PACK(struct HeaderSerializedMessage {
    uint8_t type;
    uint64_t curSize;
});

PACK(struct UDPSerializedMessage {
    uint8_t type;
    uint64_t curSize;
    uint8_t flag;
    uint16_t fragments;
    byte_t data[MAX_UDP_MSGLEN];
});

PACK(struct TCPSerializedMessage {
    uint8_t type;
    uint64_t curSize;
    bool isFinished = true;
    byte_t data[MAX_TCP_MSGLEN];
});

class AMessage {
public:
    /* there is no constructor, the data is casted as the class it was meant to
     * be delivered */
    /* commented since we don't use AMessage anyway, so don't use the vtable */
    // virtual void writeHeader(); // override final;
    // virtual void readHeader(); // override final;

    uint64_t getSize() const {
        return m_curSize;
    }
    uint8_t getType() const {
        return m_type;
    }
    std::size_t getMaxSize() const {
        return m_maxSize;
    }

    virtual const byte_t *getData() const = 0;

    template <typename T>
    void appendData(const T &data, size_t offset = 0) {
        byte_t *myData = getDataMember();

        std::memcpy(myData + m_curSize + offset, &data, sizeof(T));
        m_curSize += sizeof(T);
    }

    template <typename T>
    void writeData(const T &data, size_t offset = 0, bool updateSize = true) {
        byte_t *myData = getDataMember();

        std::memcpy(myData, &data, sizeof(T));
        if (updateSize)
            m_curSize = sizeof(T);
    }

    template <typename T>
    size_t readData(T &data) const {
        if (sizeof(T) > m_curSize)
            throw std::runtime_error("Message is too small to read data");

        const byte_t *myData = getData();
        std::memcpy(&data, myData, sizeof(T));
        return sizeof(T);
    }

    template <typename T>
    size_t readContinuousData(T &data, size_t &readOffset) const {
        if (sizeof(T) + readOffset > m_curSize)
            throw std::runtime_error("Message is too small to read data");

        const byte_t *myData = getData();
        std::memcpy(&data, myData + readOffset, sizeof(T));
        readOffset += sizeof(T);
        return sizeof(T);
    }

    void writeData(const void *data, std::size_t size);
    void readData(void *data, std::size_t size) const;

protected:
    AMessage(std::size_t maxSize, uint8_t type);
    virtual ~AMessage() = default;

    virtual byte_t *getDataMember() = 0;

    const std::size_t m_maxSize;
    std::uint64_t m_curSize = 0;
    uint8_t m_type;
};

class TCPMessage : public AMessage {
public:
    TCPMessage(std::size_t maxSize, uint8_t type);
    ~TCPMessage() = default;

    TCPMessage &operator=(const TCPMessage &other);

    const byte_t *getData() const override final {
        return m_data;
    }

    void getSerialize(TCPSerializedMessage &msg) const;
    void setSerialize(TCPSerializedMessage &msg);

private:
    byte_t *getDataMember() override final {
        return m_data;
    };

    bool m_isFinished = true;

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_TCP_MSGLEN];
};

class UDPMessage : public AMessage {
public:
    enum Flag {
        COMPRESSED = 1,
        HEADER = 2,
        FRAGMENTED = 4,
        ENCRYPTED = 8,
    };

public:
    /* hasHeader = tell if it will be transmitted into the channel, with the necessity to have additional headers */
    UDPMessage(bool hasHeader, uint8_t type);
    ~UDPMessage() = default;

    UDPMessage &operator=(const UDPMessage &other);

    const byte_t *getData() const override final {
        return m_data;
    }

    bool isCompressed() const {
        return m_flags & COMPRESSED;
    }
    bool hasHeader() const {
        return m_flags & HEADER;
    }
    bool isFragmented() const {
        return m_flags & FRAGMENTED;
    }
    bool isEncrypted() const {
        return m_flags & ENCRYPTED;
    }
    void setCompressed(bool compressed);
    void setHeader(bool header);
    void setFragmented(bool fragmented);
    void setEncrypted(bool encrypted);

    void writeHeader(const UDPG_NetChannelHeader &header);
    void readHeader(UDPG_NetChannelHeader &header) const;

    void getSerialize(UDPSerializedMessage &msg) const;
    std::vector<UDPSerializedMessage> getSerializeFragmented(void) const;
    void setSerialize(UDPSerializedMessage &msg);

private:
    byte_t *getDataMember() override final {
        return m_data;
    };

    uint8_t m_flags = 0;

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_UDP_MSGLEN];
};

} // namespace Network
