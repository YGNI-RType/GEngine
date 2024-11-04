/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** NetworkWatcher
*/

#include "GEngine/interface/network/systems/NetworkWatcher.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"

#include "GEngine/net/net.hpp"

namespace gengine::interface::network::system {

void NetworkWatcher::init(void) {
    subscribeToEvent(&NetworkWatcher::onGameLoop);
}

void NetworkWatcher::onGameLoop(gengine::system::event::GameLoop &) {
    auto &em = Network::NET::getEventManager();

    auto unintendedRes = em.getLastResult();
    if (unintendedRes == Network::EL_NONE)
        return;
    switch (unintendedRes) {
    case Network::EL_RECORDING_END:
        publishEvent(gengine::system::event::StopMainLoop());
        break;
    default:
        return;
    }
}
} // namespace gengine::interface::network
