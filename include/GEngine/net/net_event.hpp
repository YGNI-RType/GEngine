/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_event
*/

#pragma once

#include "events/socket_event.hpp"
#include "net_exception.hpp"
#include "net_wait.hpp"

#include <any>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <typeindex>

namespace Network::Event {
enum Type { CONNECT, DISCONNECT, PING_LAN, RECORD, WATCH };
enum Result { OK, KO, NO_RESULT };

enum CallbackType {
    CT_OnClientConnect,
    CT_OnClientDisconnect,
    CT_OnServerConnect,
    CT_OnServerDisconnect,
    CT_OnClientReady,
    CT_OnServerReady,
    CT_OnPingResult,
};

struct InfoHeader {
    Type type;
};

template <typename T>
struct Info : public InfoHeader {
    std::unique_ptr<T> data;
};

class Manager {
public:
    Manager() = default;
    ~Manager() = default;

    /* game engine thread */
    template <typename T>
    size_t addEvent(Type type, const T &data) {
        std::unique_ptr<Info<T>> info = std::make_unique<Info<T>>();

        info->type = type;
        info->data = std::make_unique<T>(data);
        auto ticket = m_resultTicket++;
        storeEvent(std::move(info), ticket);
        m_socketEvent.signal();
        return ticket;
    }

    void createSets(NetWaitSet &set);
    bool handleEvent(void);

    void sendPackets(void);

    Result getLastResult(size_t idTicket, bool wait);
    ExceptionLocation getLastResult(void);
    void pushResult(Result result, size_t ticket);
    void pushResult(ExceptionLocation result, bool ended = false);

    SocketEvent &getSocketEvent() {
        return m_socketEvent;
    }

    /** callback **/

    using CallbackMap = std::unordered_map<CallbackType, std::vector<std::pair<std::type_index, std::any>>>;

    template <typename T>
    void registerCallback(const CallbackType &type, std::function<void(T)> callback) {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_callbacks[type].emplace_back(std::type_index(typeid(T)), callback);
    }

    // Function to invoke callbacks with any argument type
    template <typename T>
    void invokeCallbacks(const CallbackType &type, const T &arg) {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_callbacks.find(type) == m_callbacks.end())
            return;

        for (const auto &callbackPair : m_callbacks.at(type)) {
            if (callbackPair.first != std::type_index(typeid(T)))
                continue;

            auto callbackAny = std::any_cast<std::function<void(T)>>(callbackPair.second);
            std::thread([callbackAny, arg]() { callbackAny(arg); }).detach();
        }
    }

    void setEnded(bool ended) {
        m_ended = ended;
    }
    bool isEnded() const {
        return m_ended;
    }

private:
    void storeEvent(std::unique_ptr<InfoHeader> info, size_t ticket);

    void handleNewEngineReq(InfoHeader &header, size_t ticket);

    SocketEvent m_socketEvent;
    bool m_ended = false;

    mutable std::mutex m_mutex;
    std::condition_variable m_conditionVar;

    std::queue<std::pair<size_t, std::unique_ptr<InfoHeader>>> m_events;

    std::unordered_map<size_t, Result> m_results;
    std::queue<ExceptionLocation> m_resultsUnattended;
    size_t m_resultTicket = 0;

    CallbackMap m_callbacks;
};
} // namespace Network::Event
