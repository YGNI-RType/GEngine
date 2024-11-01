/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {

TCPMessage::TCPMessage(uint8_t type)
    : AMessage(type, 0) {
}

TCPMessage &TCPMessage::operator=(const TCPMessage &other) {
    m_curSize = other.m_curSize;
    m_type = other.m_type;
    m_flags = other.m_flags;
    std::memcpy(m_data, other.m_data, m_curSize);
    return *this;
}

void TCPMessage::setSerialize(const TCPSerializedMessage &msg) {
    m_type = msg.type;
    m_curSize = msg.curSize;
    m_flags = msg.flag;
    std::memcpy(m_data, &msg.data, m_curSize);
}

void TCPMessage::getSerialize(TCPSerializedMessage &msg) const {
    msg.type = m_type;
    msg.curSize = m_curSize;
    msg.flag = m_flags;
    std::memcpy(&msg.data, m_data, m_curSize);
}

} // namespace Network
