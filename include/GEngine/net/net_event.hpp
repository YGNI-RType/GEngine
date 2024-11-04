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

/**
 * @class Manager
 * @brief Manages network events, callbacks, and results for the game engine.
 *
 * The Manager class is responsible for handling network events, managing callbacks,
 * and storing results. It provides methods to add events, register and invoke callbacks,
 * and retrieve results. It also manages the state of the engine and signals socket events.
 */
class Manager {
public:
    Manager() = default;
    ~Manager() = default;

    template <typename T>
    /**
     * @brief Adds an event to the event queue.
     *
     * This function creates a new event of the specified type with the given data,
     * assigns it a unique ticket number, stores the event, and signals the socket event.
     *
     * @tparam T The type of the data associated with the event.
     * @param type The type of the event.
     * @param data The data associated with the event.
     * @return size_t The unique ticket number assigned to the event.
     */
    size_t addEvent(Type type, const T &data) {
        std::unique_ptr<Info<T>> info = std::make_unique<Info<T>>();

        info->type = type;
        info->data = std::make_unique<T>(data);
        auto ticket = m_resultTicket++;
        storeEvent(std::move(info), ticket);
        m_socketEvent.signal();
        return ticket;
    }

    /**
     * @brief Creates and initializes the network wait sets.
     *
     * This function sets up the necessary network wait sets for monitoring
     * network events, setting up the fd for example. It initializes the provided NetWaitSet object with
     * the appropriate settings and configurations.
     *
     * @param set A reference to a NetWaitSet object that will be initialized.
     */
    void createSets(NetWaitSet &set);
    /**
     * @brief Handles network events based on the provided wait set.
     *
     * This function processes the network events that are ready in the given
     * NetWaitSet. It performs the necessary actions based on the type of events
     * detected.
     *
     * @param set The NetWaitSet containing the network events to be handled.
     * @return true if the event handling was successful, false otherwise.
     */
    bool handleEvent(const NetWaitSet &set);
    bool handleEvent(void);

    void sendPackets(void);

    /**
     * @brief Retrieves the last result for a given ticket.
     *
     * @param idTicket The identifier of the ticket for which to get the last result.
     * @param wait If true, the function will wait for the result to be available.
     * @return Result The last result associated with the specified ticket.
     */
    Result getLastResult(size_t idTicket, bool wait);
    /**
     * @brief Retrieves the last result of a network operation.
     *
     * This function returns an ExceptionLocation object that contains
     * information about the last network operation result. It can be used
     * to diagnose issues or errors that occurred during network communication.
     *
     * @return ExceptionLocation The last result of a network operation.
     */
    ExceptionLocation getLastResult(void);
    /**
     * @brief Pushes a result into the event queue, awaited by the GEG thread via a ticket.
     *
     * @param result The result to be pushed.
     * @param ticket The ticket associated with the result.
     */
    void pushResult(Result result, size_t ticket);
    /**
     * @brief Pushes the result of a network event, most likely and error.
     *
     * @param result The location of the exception that occurred.
     * @param ended Indicates whether the event has ended. Defaults to false.
     */
    void pushResult(ExceptionLocation result, bool ended = false);

    SocketEvent &getSocketEvent() {
        return m_socketEvent;
    }

    /** callback **/

    using CallbackMap = std::unordered_map<CallbackType, std::vector<std::pair<std::type_index, std::any>>>;

    template <typename T>
    /**
     * @brief Registers a callback function for a specific event type.
     *
     * This function allows you to register a callback function that will be called
     * when an event of the specified type occurs. The callback function is stored
     * in a thread-safe manner.
     *
     * @tparam T The type of the event data that the callback function will handle.
     * @param type The type of the event for which the callback is being registered.
     * @param callback The callback function to be registered. This function should
     * take a single parameter of type T.
     */
    void registerCallback(const CallbackType &type, std::function<void(T)> callback) {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_callbacks[type].emplace_back(std::type_index(typeid(T)), callback);
    }

    template <typename T>
    /**
     * @brief Invokes all registered callbacks of a specific type with the provided argument.
     *
     * This function locks the mutex to ensure thread safety, checks if there are any callbacks
     * registered for the given type, and if so, iterates through them. For each callback that
     * matches the type of the argument, it casts the callback to the appropriate function type
     * and invokes it in a detached thread.
     *
     * @tparam T The type of the argument to pass to the callbacks.
     * @param type The type of the callback to invoke.
     * @param arg The argument to pass to the callbacks.
     */
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
    /**
     * @brief Stores an event with the given information and ticket. Created via addEvent.
     *
     * @param info A unique pointer to an InfoHeader object containing the event information.
     * @param ticket A size_t representing the ticket associated with the event.
     */
    void storeEvent(std::unique_ptr<InfoHeader> info, size_t ticket);

    /**
     * @brief Handles a new engine request.
     *
     * This function processes a new engine request based on the provided header information
     * and ticket number. For example for connection callback, record callbacks etc.
     * Closely linked to CallbackType.
     *
     * @param header A reference to an InfoHeader object containing the request details.
     * @param ticket A size_t representing the ticket number associated with the request.
     */
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
