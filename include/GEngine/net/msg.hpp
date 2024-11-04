/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#pragma once

#include "GEngine/utils/libapi.hpp"
#include "net_common.hpp"
#include "net_huffman.hpp"
#include "structs/msg_all_structs.hpp"
#include "structs/msg_tcp_structs.hpp"
#include "structs/msg_udp_structs.hpp"
#include "utils/pack.hpp"

#include "msg_error.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace Network {

PACK(struct HeaderSerializedMessage {
    uint8_t type;
    uint64_t curSize;
});

PACK(struct SerializedMessage {
    uint8_t type;
    uint64_t curSize;
    uint8_t flag;
    byte_t data[MAX_TCP_MSGLEN];
});

PACK(struct UDPSerializedMessage {
    uint8_t type;
    uint64_t curSize;
    uint8_t flag;
    byte_t data[MAX_UDP_MSGLEN];
});

PACK(struct TCPSerializedMessage {
    uint8_t type;
    uint64_t curSize;
    uint8_t flag;
    byte_t data[MAX_TCP_MSGLEN];
});

/**
 * @class AMessage
 * @brief Represents a network message with various flags and data handling capabilities.
 *
 * The AMessage class provides an interface for handling network messages with support for
 * compression, fragmentation, and acknowledgment. It includes methods for reading and writing
 * data, as well as managing message flags.
 *
 * @note This class is abstract and cannot be instantiated directly.
 *
 * @enum Flag
 * @brief Enumeration of possible message flags.
 * @var Flag::COMPRESSED
 * Indicates that the message is compressed.
 * @var Flag::HEADER
 * Indicates that the message contains a header.
 * @var Flag::FRAGMENTED
 * Indicates that the message is fragmented.
 * @var Flag::FULL_ACK
 * Indicates that the message requires full acknowledgment.
 * @var Flag::ACK
 * Indicates that the message requires acknowledgment.
 * @var Flag::WAS_FRAGMENTED
 * Indicates that the message was fragmented.
 * @var Flag::END_COMPRESS
 * Indicates the end of compression for the message.
 * @var Flag::FAST_RETRANSMISSION
 * Indicates that the message should be retransmitted quickly.
 *
 **/
class AMessage {
public:
    enum Flag {
        COMPRESSED = 1,
        HEADER = 2,
        FRAGMENTED = 4,
        FULL_ACK = 8,
        ACK = 16,
        WAS_FRAGMENTED = 32,
        END_COMPRESS = 64,
        FAST_RETRANSMISSION = 128,
    };

public:
    /* there is no constructor, the data is casted as the class it was meant to
     * be delivered */
    /* commented since we don't use AMessage anyway, so don't use the vtable */

    uint64_t getSize() const {
        return m_curSize;
    }

    uint8_t getType() const {
        return m_type;
    }
    void setType(uint8_t type) {
        m_type = type;
    }

    /* THERE IS A DIFFERENCE BETWEEN isCompressed & isEndCompress */
    bool isCompressed(void) const {
        return m_flags & COMPRESSED;
    }
    bool isEndCompress(void) const {
        return m_flags & END_COMPRESS;
    }
    bool hasHeader(void) const {
        return m_flags & HEADER;
    }
    bool isFragmented(void) const {
        return m_flags & FRAGMENTED;
    }
    bool wasFragmented(void) const {
        return m_flags & WAS_FRAGMENTED;
    }
    bool isFullAck(void) const {
        return m_flags & FULL_ACK;
    }
    bool shouldAck(void) const {
        return m_flags & ACK;
    }
    /* tells if the network layer should immediately handle, without getting to the engine part */
    bool shouldFastRetransmit(void) const {
        return m_flags & FAST_RETRANSMISSION;
    }

    uint8_t getFlags(void) const {
        return m_flags;
    }

    void setFlag(uint8_t flag) {
        m_flags = flag;
    }

    virtual const byte_t *getData(void) const = 0;
    virtual byte_t *getModifyData(void) = 0;
    virtual uint64_t getMaxMsgSize(void) const = 0;
    virtual uint8_t getClassType(void) const = 0;

    /******** WRITE DATA ********/

    template <typename T>
    /**
     * @brief Appends data to the message, with optional compression.
     *
     * This function appends the given data to the message. If compression is enabled,
     * the data will be compressed before being appended. The function also supports
     * an optional offset and a size watcher for compressed data.
     *
     * @tparam T The type of the data to append.
     * @param data The data to append to the message.
     * @param sizeCompressWatcher Optional pointer to a size watcher for compressed data.
     *                            If provided, it will be incremented by the size of the data.
     *                            If the size exceeds the maximum message size, an exception is thrown.
     * @param offset Optional offset to start appending the data. Default is 0.
     * @return The size of the appended data.
     * @throws MsgError If the message offset is too big or if the message is too big to compress.
     */
    size_t appendData(const T &data, size_t *sizeCompressWatcher = nullptr, size_t offset = 0) {
        if (m_compressNow) {
            if (m_curSize + offset > getMaxMsgSize())
                throw MsgError("Message offset is too big");
            m_huffman.compressContinuous(*this, m_curSize + offset, (const byte_t *)&data, sizeof(T));
            if (sizeCompressWatcher) {
                *sizeCompressWatcher += sizeof(T);
                if (*sizeCompressWatcher > getMaxMsgSize())
                    throw MsgError("Message is too big to compress");
            }
            return sizeof(T);
        }

        byte_t *myData = getDataMember();
        uint64_t maxSz = getMaxMsgSize();
        if (m_curSize + offset + sizeof(T) > maxSz)
            return sizeof(T);

        std::memcpy(myData + m_curSize + offset, (void *)&data, sizeof(T));
        m_curSize += sizeof(T);
        return sizeof(T);
    }

    template <typename T>
    /**
     * @brief Writes data to the message buffer.
     *
     * This function writes the provided data to the message buffer at the specified offset.
     * If compression is enabled, the function will return without writing data, as the size
     * will change. The function also ensures that the data does not exceed the maximum message size.
     *
     * @tparam T The type of the data to be written.
     * @param data The data to be written to the message buffer.
     * @param msgDataOffset The offset in the message buffer where the data should be written. Default is 0.
     * @param dataOffset The offset within the data to start writing from. Default is 0.
     * @param updateSize A flag indicating whether to update the current size of the message. Default is true.
     */
    void writeData(const T &data, size_t msgDataOffset = 0, size_t dataOffset = 0, bool updateSize = true) {
        if (m_compressNow) /* can't bceause the size will change */
            return;

        byte_t *myData = getDataMember();
        uint64_t maxSz = getMaxMsgSize();
        if (msgDataOffset + sizeof(T) >= maxSz)
            return;

        std::memcpy(myData + msgDataOffset, &data + dataOffset, sizeof(T));
        if (updateSize)
            m_curSize = sizeof(T);
    }

    template <typename T>
    /**
     * @brief Reads data of type T from the message.
     *
     * This function reads data of type T from the message. If the message is
     * currently compressed, it returns -1 since it doesn't support it. If the size of the data to be read
     * exceeds the current size of the message, it throws a MsgError.
     *
     * @tparam T The type of the data to be read.
     * @param data A reference to the variable where the read data will be stored.
     * @return The size of the data read in bytes, or -1 if the message is compressed.
     * @throws MsgError if the message is too small to read the data.
     */
    size_t readData(T &data) const {
        if (m_compressNow)
            return -1; /* todo : raise the fact that you can't */

        if (sizeof(T) > m_curSize)
            throw MsgError("Message is too small to read data");

        const byte_t *myData = getData();
        std::memcpy(&data, myData, sizeof(T));
        return sizeof(T);
    }

    template <typename T>
    /**
     * @brief Reads continuous data of type T from the message starting at the given read offset.
     *
     * This function reads data of type T from the internal message buffer starting at the specified
     * read offset. It ensures that there is enough data in the message to read the requested type.
     * If the message is too small to read the data, it throws a MsgError exception.
     *
     * @tparam T The type of data to read.
     * @param data Reference to the variable where the read data will be stored.
     * @param readOffset Reference to the offset from where to start reading the data. This offset
     *        will be incremented by the size of the data type T after reading.
     * @return The size of the data type T that was read.
     * @throws MsgError If the message is too small to read the requested data.
     */
    size_t readContinuousData(T &data, size_t &readOffset) const {
        if (sizeof(T) + readOffset > CF_NET_MIN(m_curSize, getMaxMsgSize()))
            throw MsgError("Message is too small to read data");

        const byte_t *myData = getData();
        std::memcpy(&data, myData + readOffset, sizeof(T));
        readOffset += sizeof(T);
        return sizeof(T);
    }

    template <typename T>
    /**
     * @brief Reads and decompresses data continuously from the message.
     *
     * This function reads data of type T from the message starting at the given offset,
     * decompressing it using Huffman coding. It ensures that the message is compressed
     * and checks for potential overflow before performing the decompression.
     *
     * @tparam T The type of data to be read and decompressed.
     * @param data A reference to the variable where the decompressed data will be stored.
     * @param offset The offset in the message from where to start reading.
     * @return The number of bytes read and decompressed.
     * @throws MsgError if the message is not compressed or if there is a message overflow.
     */
    size_t readContinuousCompressed(T &data, size_t &offset) {
        if (!isCompressed())
            throw MsgError("Message is not compressed");

        if (m_curSize + offset + getBitBuffer() / 8 > getMaxMsgSize())
            throw MsgError("Message overflow when reading");
        return m_huffman.decompressContinuous(*this, offset, (byte_t *)&data, sizeof(T));
    }

    /**
     * @brief Appends data to the internal buffer.
     *
     * @param data Pointer to the data to be appended.
     * @param size Size of the data to be appended in bytes.
     * @param sizeCompressWatcher Optional pointer to a size_t that will be updated with the compressed size.
     * @return size_t The new size of the internal buffer after appending the data.
     */
    size_t appendData(const void *data, std::size_t size, size_t *sizeCompressWatcher = nullptr);
    /**
     * @brief Writes data to the buffer.
     *
     * @param data Pointer to the data to be written.
     * @param size Size of the data to be written in bytes.
     * @param updateSize If true, updates the size of the buffer after writing. Default is true.
     */
    void writeData(const void *data, std::size_t size, bool updateSize = true);
    /**
     * @brief Reads data from a source into a buffer.
     *
     * This function reads a specified amount of data from a source and stores it
     * into the provided buffer. The read operation starts at the given offset.
     *
     * @param data Pointer to the buffer where the read data will be stored.
     * @param offset Reference to the offset from where the read operation will start.
     * @param size The number of bytes to read from the source.
     */
    void readData(void *data, std::size_t &offset, std::size_t size) const;
    /**
     * @brief Reads compressed data from a source.
     *
     * This function reads compressed data from a given source and updates the offset in bits.
     *
     * @param data Pointer to the source data to be read.
     * @param offsetBits Reference to the offset in bits, which will be updated after reading.
     * @param size Size of the data to be read in bytes.
     */
    void readDataCompressed(void *data, std::size_t &offsetBits, std::size_t size);

    /****************************/

    /* When you call, all the appendeed data from here will be compressed, this is for END_COMPRESS */
    void startCompressingSegment(bool reading);
    /* When you call, everything that was in that previous segment will stop being compressed. THIS WILL APPLY THE SIZE
     * ONCE DONE */
    void stopCompressingSegment(bool reading);

    /*
        There is no:
            void startDecompressingSegment(void);
            void stopDecompressingSegment(void);

        since as soon we get the data, we decompress it, and append inside the message in the queue
    */

    bool getCompressingBuffer(void *&data, size_t &bufferSize);

    size_t &getBitBuffer(void) {
        return bitBuffer;
    }
    size_t &getBitRemains(void) {
        return bitBuffer;
    }

protected:
    AMessage(uint8_t type, uint8_t flags);
    virtual ~AMessage() = default;

    virtual byte_t *getDataMember() = 0;

    std::uint64_t m_curSize = 0;
    uint8_t m_type;
    uint8_t m_flags = 0;
    bool m_compressNow = false;

private:
    API static Compression::AHC m_huffman;
    size_t bitBuffer = 0;
};

/****************************************************************************************************/

class TCPMessage : public AMessage {
public:
    TCPMessage(uint8_t type);
    ~TCPMessage() = default;

    TCPMessage &operator=(const TCPMessage &other);

    const byte_t *getData(void) const override final {
        return m_data;
    }
    byte_t *getModifyData(void) override final {
        return m_data;
    }
    uint64_t getMaxMsgSize(void) const override final {
        return MAX_TCP_MSGLEN;
    }
    uint8_t getClassType(void) const override final {
        return 'T';
    }

    /**
     * @brief Serializes the current object into a TCPSerializedMessage.
     *
     * This function takes a TCPSerializedMessage object as a parameter and
     * serializes the current object's state into it. The serialized message
     * can then be used for network transmission or storage.
     *
     * @param msg A reference to a TCPSerializedMessage object where the serialized
     *            data will be stored.
     */
    void getSerialize(TCPSerializedMessage &msg) const;
    /**
     * @brief Sets the serialized message.
     *
     * This function assigns the given TCPSerializedMessage to the internal
     * representation used by the class. It is used to update or initialize
     * the serialized message that will be processed or transmitted over
     * the network.
     *
     * @param msg The TCPSerializedMessage object containing the serialized data.
     */
    void setSerialize(const TCPSerializedMessage &msg);

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
    /* hasHeader = tell if it will be transmitted into the channel, with the necessity to have additional headers */
    UDPMessage(uint8_t flags, uint8_t type);
    ~UDPMessage() = default;

    UDPMessage &operator=(const UDPMessage &other);

    const byte_t *getData(void) const override final {
        return m_data;
    }
    byte_t *getModifyData(void) override final {
        return m_data;
    }
    uint64_t getMaxMsgSize(void) const override final {
        return MAX_UDP_MSGLEN;
    }
    uint8_t getClassType(void) const override final {
        return 'U';
    }

    uint64_t getHash(void) const;

    void clear(void);

    void setCompressed(bool compressed);
    void setHeader(bool header);
    void setFragmented(bool fragmented);
    void setWasFragmented(bool fragmented);
    void setFullAck(bool fullack);
    void setAck(bool ack);
    void setFastRetransmission(bool fastRetransmission);

    /**
     * @brief Writes the given network channel header to the output stream.
     *
     * This function takes a UDPG_NetChannelHeader object and writes its contents
     * to the appropriate output stream, ensuring that the header information is
     * correctly serialized for network transmission.
     * This is used only by the net_channel
     *
     * @param header The UDPG_NetChannelHeader object containing the header information
     *               to be written.
     */
    void writeHeader(const UDPG_NetChannelHeader &header);
    /**
     * @brief Reads the header from the network channel.
     *
     * This function reads the header information from the network channel and updates the read offset accordingly.
     *
     * @param header A reference to the UDPG_NetChannelHeader structure where the header information will be stored.
     * @param readOffset A reference to the size_t variable that keeps track of the current read offset. This will be
     * updated after reading the header.
     */
    void readHeader(UDPG_NetChannelHeader &header, size_t &readOffset) const;

    /**
     * @brief Serializes the current object into a UDPSerializedMessage.
     *
     * This function takes a reference to a UDPSerializedMessage object and
     * serializes the current object's state into it. The serialized message
     * can then be used for network transmission.
     *
     * @param msg A reference to a UDPSerializedMessage object where the
     *            serialized data will be stored.
     */
    void getSerialize(UDPSerializedMessage &msg) const;
    /**
     * @brief Retrieves a vector of UDPSerializedMessage objects that represent
     *        the serialized and fragmented messages.
     *
     * This function returns a collection of messages that have been serialized
     * and fragmented for UDP transmission. Each UDPSerializedMessage in the
     * vector corresponds to a fragment of the original message.
     *
     * @return std::vector<UDPSerializedMessage> A vector containing the serialized
     *         and fragmented messages.
     */
    std::vector<UDPSerializedMessage> getSerializeFragmented(void) const;
    /**
     * @brief Sets the serialized message.
     *
     * This function assigns the given UDPSerializedMessage to the internal
     * representation used by the class. It is used to update or initialize
     * the serialized message that will be processed or transmitted over
     * the network.
     *
     * @param msg The UDPSerializedMessage object containing the serialized data.
     */
    void setSerialize(const UDPSerializedMessage &msg);

    /**
     * @brief Retrieves the acknowledgment number.
     *
     * @return uint64_t The acknowledgment number.
     */
    uint64_t getAckNumber(void) const;

private:
    byte_t *getDataMember() override final {
        return m_data;
    };

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_UDP_MSGLEN];
};

} // namespace Network
