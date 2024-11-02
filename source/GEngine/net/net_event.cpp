/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_event
*/

#include "GEngine/net/net_event.hpp"
#include "GEngine/net/events/connection.hpp"
#include "GEngine/net/events/record.hpp"
#include "GEngine/net/net.hpp"

namespace Network::Event {
void Manager::createSets(NetWaitSet &set) {
    set.setAlert(m_socketEvent);
}

bool Manager::handleEvent(const NetWaitSet &set) {
    if (!set.isSignaled(m_socketEvent))
        return false;

    size_t size;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        size = m_events.size();
    }
    m_socketEvent.wait();
    if (size == 0) {
        sendPackets();
        return true;
    }
    for (size_t i = 0; i < size; i++) {
        std::unique_ptr<InfoHeader> info;
        size_t ticket;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_events.empty())
                return true;
            auto &[_ticket, _info] = m_events.front();
            ticket = _ticket;
            info = std::move(_info);
            m_events.pop();
        }

        handleNewEngineReq(*info, ticket);
    }
    return true;
}

/* game engine thread */
void Manager::storeEvent(std::unique_ptr<InfoHeader> info, size_t ticket) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_events.push(std::make_pair(ticket, std::move(info)));
}

void Manager::handleNewEngineReq(InfoHeader &header, size_t ticket) {
    switch (header.type) {
    case CONNECT: {
        auto &client = NET::getClient();
        auto &dataPtr = reinterpret_cast<Info<ConnectInfo> &>(header).data;
        bool result;

        if (dataPtr->pingIndex != -1) /* this means they found it via pinging LAN */
            result = client.connectToServer(dataPtr->pingIndex);
        else
            result = client.connectToServer(dataPtr->ip, dataPtr->port);
        if (!result)
            break;
        return pushResult(Result::OK, ticket);
    } break;
    case DISCONNECT: {
        auto &client = NET::getClient();

        client.disconnectFromServer(Event::DT_WANTED);
        return pushResult(Result::OK, ticket);
    } break;
    case PING_LAN:
        NET::pingServers();
        return pushResult(Result::OK, ticket);
        break;
    case RECORD: {
        auto &record = NET::getRecord();

        auto &dataPtr = reinterpret_cast<Info<RecordInfo> &>(header).data;
        switch (dataPtr->mode) {
        case RecordInfo::STOP:
            if (record.isRecording()) {
                if (!record.endRecord())
                    break;
                return pushResult(Result::OK, ticket);
            } else if (record.isWatching()) {
                if (record.endWatch())
                    break;
                return pushResult(Result::OK, ticket);
            }

            break;
        case RecordInfo::RECORD: {
            if (record.isRecording())
                break;

            if (!record.isEnabled())
                record.init();

            if (!record.startRecord())
                break;

            auto &client = NET::getClient();
            client.refreshSnapshots();
            return pushResult(Result::OK, ticket);
        } break;
        case RecordInfo::WATCH:
            if (record.isWatching())
                break;

            if (!record.isEnabled())
                record.init();

            if (!record.startWatch(dataPtr->demoFile))
                break;

            record.startWatchFakeNet();
            return pushResult(Result::OK, ticket);
        default:
            break;
        }
    }
    default:
        break;
    }
    return pushResult(Result::KO, ticket);
}

void Manager::sendPackets(void) {
    NetServer &server = NET::getServer();
    if (server.sendPackets())
        return;

    CLNetClient &client = NET::getClient();
    if (client.sendPackets())
        return;
}

void Manager::pushResult(Result result, size_t ticket) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_results[ticket] = result;
    }
    m_conditionVar.notify_all();
}

void Manager::pushResult(ExceptionLocation result, bool ended) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_resultsUnattended.push(result);
    }
    if (ended)
        m_conditionVar.notify_all();
}

Result Manager::getLastResult(size_t ticket, bool wait) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (wait)
        m_conditionVar.wait(lock, [&] { return m_results.find(ticket) != m_results.end() || isEnded(); });
    else if (m_results.find(ticket) == m_results.end())
        return Result::NO_RESULT;

    Result result = m_results[ticket];
    m_results.erase(ticket);
    return result;
}

ExceptionLocation Manager::getLastResult(void) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_resultsUnattended.empty())
        return ExceptionLocation::EL_NONE;

    auto result = m_resultsUnattended.front();
    m_resultsUnattended.pop();
    return result;
}

} // namespace Network::Event