/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"
#include "GEngine/net/msg_error.hpp"

namespace Network {

Compression::AHC AMessage::m_huffman;

AMessage::AMessage(uint8_t type, uint8_t flags)
    : m_type(type)
    , m_flags(flags) {
}

size_t AMessage::appendData(const void *data, std::size_t size, size_t *sizeCompressWatcher) {
    if (m_compressNow) {
        if (m_curSize + size > getMaxMsgSize())
            return size;
        m_huffman.compressContinuous(*this, m_curSize, (const byte_t *)data, size);
        if (sizeCompressWatcher) {
            *sizeCompressWatcher += size;
            if (*sizeCompressWatcher > getMaxMsgSize())
                throw MsgError("Message is too big to compress");
        }
        return size;
    }

    if (m_curSize + size > getMaxMsgSize())
        return size;

    byte_t *myData = getDataMember();
    std::memcpy(myData + m_curSize, data, size);
    m_curSize += size;
    return size;
}

void AMessage::writeData(const void *data, std::size_t size, bool updateSize) {
    if (m_compressNow) /* can't bceause the size will change */
        return;

    if (m_curSize + size > getMaxMsgSize())
        return;

    byte_t *myData = getDataMember();
    std::memcpy(myData, data, size);
    if (updateSize)
        m_curSize = size;
}

void AMessage::readData(void *data, size_t &offset, std::size_t size) const {
    if (offset + size > m_curSize)
        throw MsgError("Reading Overflow");

    const byte_t *myData = getData();
    std::memcpy(data, myData + offset, size);
    offset += size;
}

void AMessage::readDataCompressed(void *data, size_t &offset, std::size_t size) {
    if (!isCompressed())
        throw MsgError("Message should be compressed");

    if (offset + getBitBuffer() / 8 > getMaxMsgSize())
        throw MsgError("Message overflow when reading: SIZE=" +
                       std::to_string(m_curSize + offset + getBitBuffer() / 8));
    m_huffman.decompressContinuous(*this, offset, (byte_t *)data, size);
}

void AMessage::startCompressingSegment(bool reading) {
    if (isCompressed()) {
        m_compressNow = true;
        return;
    }

    if (!isEndCompress() || reading)
        return;

    auto data = getDataMember();
    ALL_MessageCompressionHeader *header =
        reinterpret_cast<ALL_MessageCompressionHeader *>((data + hasHeader()) ? sizeof(UDPG_NetChannelHeader) : 0);
    if (header->size != 0)
        return;

    header->offset = m_curSize;
}

void AMessage::stopCompressingSegment(bool reading) {
    if (isCompressed()) {
        m_compressNow = false;
        if (!reading) {
            size_t wroteData = getBitBuffer() / 8;
            m_curSize += wroteData + 1;
        }
        return;
    }

    if (!isEndCompress() || reading)
        return;

    auto data = getDataMember();
    /* todo: using UDP structs in amessage */
    ALL_MessageCompressionHeader *header =
        reinterpret_cast<ALL_MessageCompressionHeader *>((data + hasHeader()) ? sizeof(UDPG_NetChannelHeader) : 0);
    if (header->size == 0)
        return;

    header->size = m_curSize - header->offset;
}

bool AMessage::getCompressingBuffer(void *&data, size_t &bufferSize) {
    if (!isEndCompress())
        return false;

    auto m_data = getDataMember();
    ALL_MessageCompressionHeader *header =
        reinterpret_cast<ALL_MessageCompressionHeader *>((m_data + hasHeader()) ? sizeof(UDPG_NetChannelHeader) : 0);
    if (header->size == 0)
        return false;
    data = m_data + header->offset;
    bufferSize = header->size;
    return true;
}

} // namespace Network
