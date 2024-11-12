/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RecordManager
*/

#include "GEngine/interface/network/systems/RecordManager.hpp"

#include "GEngine/net/events/record.hpp"
#include "GEngine/net/net.hpp"

namespace gengine::interface::network::system {

bool RecordManager::mg_started = false;

void RecordManager::init(void) {
    subscribeToEvent<gengine::interface::network::event::ToggleRecord>(&RecordManager::toggleCapture);
    subscribeToEvent<gengine::interface::network::event::WatchReplay>(&RecordManager::watchReplay);
}

void RecordManager::toggleCapture(gengine::interface::network::event::ToggleRecord &) {
    auto &em = Network::NET::getEventManager();
    size_t ticket;
    bool result;

    if (mg_started) {
        mg_started = false;
        ticket =
            em.addEvent(Network::Event::RECORD, Network::Event::RecordInfo(Network::Event::RecordInfo::Mode::STOP));
        result = em.getLastResult(ticket, true);
    } else {
        mg_started = true;
        ticket =
            em.addEvent(Network::Event::RECORD, Network::Event::RecordInfo(Network::Event::RecordInfo::Mode::RECORD));
        result = em.getLastResult(ticket, false);
    }
}

void RecordManager::watchReplay(gengine::interface::network::event::WatchReplay &e) {
    if (mg_started)
        return;

    mg_started = true;
    Network::Event::Manager &em = Network::NET::getEventManager();
    auto recordInfo = Network::Event::RecordInfo(Network::Event::RecordInfo::WATCH);
    recordInfo.demoFile = e.demoPath;
    size_t ticket = em.addEvent(Network::Event::RECORD, recordInfo);
    auto result = em.getLastResult(ticket, true);
    if (result != Network::Event::Result::OK)
        throw std::runtime_error("Failed to start replay");
}
} // namespace gengine::interface::network::system
