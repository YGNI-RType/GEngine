/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_queue_heap
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
template <typename MSGTYPE, size_t MAX_MSGS>
class NetQueueHeap {
public:
    NetQueueHeap(Event::SocketEvent &socketEvent)
        : m_socketEvent(socketEvent) {};

    ~NetQueueHeap() = default;
    NetQueueHeap(const NetQueueHeap &) = delete;
    NetQueueHeap &operator=(const NetQueueHeap &) = delete;
    NetQueueHeap(NetQueueHeap &&) = delete;
    NetQueueHeap &operator=(NetQueueHeap &&) = delete;

    bool push(std::unique_ptr<MSGTYPE> msg, size_t readcount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        return pushUnsafe(std::move(msg), readcount);
    }

    /* This is called when we know it's full, removes the front, same segment */
    bool fullpush(std::unique_ptr<MSGTYPE> msg, size_t readcount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_msgs.find(msg->getType());
        if (it == m_msgs.end() || !full())
            return pushUnsafe(std::move(msg), readcount);

        auto &[_, q] = *it;
        q.pop();
        m_popped++;
        q.push(std::move(msg));

        m_socketEvent.signal();
        return true;
    }

    /* MSGTYPE(false, <type you chose here> )*/
    bool pop(MSGTYPE &msg, size_t &readCount, uint8_t type) {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_msgs.find(type);
        if (it == m_msgs.end())
            return false;

        auto &[_, q] = *it;
        if (q.empty())
            return false;

        *msg = q.front();
        q.pop();
        m_nbUsed--;
        m_popped++;

        return true;
    }

    bool pop(MSGTYPE &msg, size_t &readCount) {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto &[type, q] : m_msgs) {
            if (q.empty())
                continue;

            msg = *q.front();
            q.pop();
            m_nbUsed--;
            m_popped++;

            return true;
        }

        return false;
    }

    bool empty(void) const {
        return m_nbUsed == 0;
    }

    bool full(void) const {
        return m_nbUsed == MAX_MSGS;
    }

    size_t size(void) const {
        return m_nbUsed;
    }

    size_t getNbPopped(void) const {
        return m_popped;
    }

    size_t size(uint8_t type) const {
        std::lock_guard<std::mutex> lock(m_mutex);

        return m_msgs.find(type) == m_msgs.end() ? 0 : m_msgs.at(type).size();
    }

    void clear(void) {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_msgs.clear();
        m_popped = 0;
    }

private:
    bool pushUnsafe(std::unique_ptr<MSGTYPE> msg, size_t readcount) {
        auto type = msg->getType();
        auto it = m_msgs.find(type);
        if (it == m_msgs.end())
            m_msgs[type] = std::queue<std::unique_ptr<MSGTYPE>>();

        auto &q = m_msgs[type];
        q.push(std::move(msg));
        m_nbUsed++;

        m_socketEvent.signal();
        return true;
    }

private:
    std::unordered_map<uint8_t, std::queue<std::unique_ptr<TCPMessage>>> m_msgs;
    std::atomic_size_t m_popped = 0;
    std::atomic_size_t m_nbUsed = 0;

    mutable std::mutex m_mutex;
    Event::SocketEvent &m_socketEvent;
};

} // namespace Network
