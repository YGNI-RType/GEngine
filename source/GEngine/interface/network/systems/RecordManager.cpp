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
void RecordManager::init(void) {
    subscribeToEvent<gengine::system::driver::input::KeyPEvent>(&RecordManager::toggleCapture);
}

void RecordManager::toggleCapture(gengine::system::driver::input::KeyPEvent &e) {
    auto &em = Network::NET::getEventManager();
    size_t ticket;
    bool result;

    switch (e.state) {
    case gengine::system::driver::input::InputState::PRESSED:
        if (m_started) {
            m_started = false;
            ticket =
                em.addEvent(Network::Event::RECORD, Network::Event::RecordInfo(Network::Event::RecordInfo::Mode::STOP));
            result = em.getLastResult(ticket, true);
        } else {
            m_started = true;
            ticket = em.addEvent(Network::Event::RECORD,
                                 Network::Event::RecordInfo(Network::Event::RecordInfo::Mode::RECORD));
            result = em.getLastResult(ticket, false);
        }
        break;
    case gengine::system::driver::input::InputState::RELEASE:
        break;
    default:
        break;
    }
}
} // namespace gengine::interface::network::system
