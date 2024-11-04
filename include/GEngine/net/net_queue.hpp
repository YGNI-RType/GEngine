/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** queue
*/

#pragma once

#include "GEngine/net/events/socket_event.hpp"
#include "GEngine/net/msg.hpp"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <unordered_map>

namespace Network {
/**
 * @class NetQueue
 * @brief A thread-safe network message queue for handling incoming and outgoing messages.
 *
 * The NetQueue class manages a queue of network messages, providing thread-safe
 * operations for pushing and popping messages. It ensures that messages are
 * processed in a FIFO (First In, First Out) manner and supports operations for
 * handling full queues.
 * This is used for UDP messages, where we have a predefined size of packets, so it's much faster since we don't have to
 * allocate memory.
 *
 * @tparam MSGTYPE The type of the messages to be stored in the queue.
 */
template <typename MSGTYPE, size_t NB_PACKETS, size_t MAX_PACKET_SIZE>
class NetQueue {
private:
    struct Segment {
        uint32_t id;
        uint8_t flag;
        uint64_t msgSize;

        size_t readCount = 0; /* only in incomming package */
    };

public:
    NetQueue(Event::SocketEvent &socketEvent)
        : m_socketEvent(socketEvent) {};

    ~NetQueue() = default;
    NetQueue(const NetQueue &) = delete;
    NetQueue &operator=(const NetQueue &) = delete;
    NetQueue(NetQueue &&) = delete;
    NetQueue &operator=(NetQueue &&) = delete;

    bool push(const MSGTYPE &msg, size_t readcount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        return pushUnsafe(msg, readcount);
    }

    /* This is called when we know it's full, removes the front, same segment */
    bool fullpush(const MSGTYPE &msg, size_t readcount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_msgs.find(msg.getType());
        if (!full())
            return pushUnsafe(msg, readcount);

        if (full() && it == m_msgs.end())
            return false; /* can't do shit, test it anyway */

        auto &[_, queueSegment] = *it;
        if (queueSegment.empty())
            return false;

        auto segment = queueSegment.front();
        segment.readCount = readcount;
        segment.flag = msg.getFlags();
        segment.msgSize = msg.getSize();

        queueSegment.pop_front();
        deconstructMessage(msg, segment);
        queueSegment.push_back(segment);

        m_socketEvent.signal();
        return true;
    }

    bool fullpushlast(const MSGTYPE &msg, size_t readcount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_msgs.find(msg.getType());
        if (it == m_msgs.end() || !full())
            return pushUnsafe(msg, readcount);

        auto &[_, queueSegment] = *it;

        auto segment = queueSegment.back();
        segment.readCount = readcount;
        segment.flag = msg.getFlags();
        segment.msgSize = msg.getSize();

        queueSegment.pop_back();
        deconstructMessage(msg, segment);
        queueSegment.push_back(segment);

        m_socketEvent.signal();
        return true;
    }

    /* MSGTYPE(false, <type you chose here> )*/
    bool pop(MSGTYPE &msg, size_t &readCount, uint8_t type) {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_msgs.find(type);
        if (it == m_msgs.end())
            return false;

        auto &[_, queueSegment] = *it;
        if (queueSegment.empty())
            return false;

        auto segment = queueSegment.front();
        queueSegment.pop_front();
        constructMessage(msg, segment, readCount);
        m_isUsed[segment.id] = false;
        m_nbUsed--;

        return true;
    }

    bool pop(MSGTYPE &msg, size_t &readCount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto &[type, queueSegment] : m_msgs) {
            if (queueSegment.empty())
                continue;

            auto segment = queueSegment.front();
            queueSegment.pop_front();
            constructMessage(msg, segment, readCount);
            msg.setType(type);
            m_isUsed[segment.id] = false;
            m_nbUsed--;

            return true;
        }

        return false;
    }

    bool empty(void) const {
        return m_nbUsed == 0;
    }

    bool full(void) const {
        return m_nbUsed == NB_PACKETS;
    }

    size_t size(void) const {
        return m_nbUsed;
    }

    size_t size(uint8_t type) const {
        std::lock_guard<std::mutex> lock(m_mutex);

        return m_msgs.find(type) == m_msgs.end() ? 0 : m_msgs.at(type).size();
    }

    void clear(void) {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_msgs.clear();
        m_nbUsed = 0;
        m_isUsed = {0};
    }

private:
    bool pushUnsafe(const MSGTYPE &msg, size_t readcount) {
        if (msg.getSize() > MAX_PACKET_SIZE)
            return false;

        auto it = m_msgs.find(msg.getType());
        if (it == m_msgs.end())
            m_msgs[msg.getType()] = std::deque<Segment>();

        auto &q = m_msgs[msg.getType()];
        auto idSegment = getFreeSegment();
        if (idSegment == -1)
            return false;

        Segment segment = {idSegment, msg.getFlags(), msg.getSize(), readcount};
        deconstructMessage(msg, segment);
        q.push_back(segment);
        m_nbUsed++;

        m_socketEvent.signal();
        return true;
    }

    void constructMessage(MSGTYPE &msg, const Segment &segment, size_t &readCount) const {
        auto data = static_cast<const void *>(m_data.data() + segment.id * MAX_PACKET_SIZE);

        msg.setFlag(segment.flag);
        msg.writeData(data, segment.msgSize);
        readCount = segment.readCount;
    }

    void deconstructMessage(const MSGTYPE &msg, Segment &segment) {
        auto data = m_data.data() + segment.id * MAX_PACKET_SIZE;

        segment.flag = msg.getFlags();
        size_t offset = 0;
        msg.readData(data, offset, segment.msgSize);
    }

    uint32_t getFreeSegment(void) {
        if (full())
            return -1;

        for (uint32_t i = 0; i < NB_PACKETS; i++) {
            if (m_isUsed[i])
                continue;
            m_isUsed[i] = true;
            return i;
        }
        return -1;
    }

private:
    std::array<byte_t, NB_PACKETS * MAX_PACKET_SIZE> m_data;
    std::array<bool, NB_PACKETS> m_isUsed = {0};
    std::atomic_size_t m_nbUsed = 0;
    std::unordered_map<uint8_t, std::deque<Segment>> m_msgs;

    mutable std::mutex m_mutex;
    Event::SocketEvent &m_socketEvent;
};

} // namespace Network
