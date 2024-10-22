/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {

Compression::AHC AMessage::m_huffman;

AMessage::AMessage(uint8_t type, uint8_t flags)
    : m_type(type), m_flags(flags) {
}

void AMessage::appendData(const void *data, std::size_t size) {
    byte_t *myData = getDataMember();
    std::memcpy(myData + m_curSize, data, size);
    m_curSize += size;
}

void AMessage::writeData(const void *data, std::size_t size, bool updateSize) {
    byte_t *myData = getDataMember();
    std::memcpy(myData, data, size);
    if (updateSize)
        m_curSize = size;
}

void AMessage::readData(void *data, size_t offset, std::size_t size) const {
    if (offset + size > m_curSize)
        throw std::runtime_error("Not enough data to read");

    const byte_t *myData = getData();
    std::memcpy(data, myData + offset, size);
}

void AMessage::startCompressingSegment(void) {
    if (isCompressed())
        return;

    auto data = getDataMember();
    ALL_MessageCompressionHeader *header = reinterpret_cast<ALL_MessageCompressionHeader *>(data + hasHeader() ? sizeof(UDPG_NetChannelHeader) : 0);
    if (header->size != 0)
        return;

    header->offset = m_curSize;
}

void AMessage::stopCompressingSegment(void) {
    if (!isCompressed())
        return;

    auto data = getDataMember();
    /* todo: using UDP structs in amessage */
    ALL_MessageCompressionHeader *header = reinterpret_cast<ALL_MessageCompressionHeader *>(data + hasHeader() ? sizeof(UDPG_NetChannelHeader) : 0);
    if (header->size == 0)
        return;

    header->size = m_curSize - header->offset;
}

bool AMessage::getCompressingBuffer(void *&data, size_t &bufferSize)
{
    if (!isCompressed())
        return false;

    auto m_data = getDataMember();
    ALL_MessageCompressionHeader *header = reinterpret_cast<ALL_MessageCompressionHeader *>(m_data + hasHeader() ? sizeof(UDPG_NetChannelHeader) : 0);
    if (header->size == 0)
        return false;
    data = m_data + header->offset;
    bufferSize = header->size;
    return true;
}


} // namespace Network
