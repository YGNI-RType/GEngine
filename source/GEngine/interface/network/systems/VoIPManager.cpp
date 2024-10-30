/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPManager
*/

#include "GEngine/interface/network/systems/VoIPManager.hpp"

#include "GEngine/net/msg.hpp"
#include "GEngine/net/net_client.hpp"

#include <iostream>

namespace gengine::interface::network::system {

void VoIPManager::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&VoIPManager::onGameLoop);
}

void VoIPManager::onGameLoop(gengine::system::event::GameLoop &) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();

    for (auto &[remote, client] : clientsSys.getClients()) {
        size_t readCount = 0;
        if (client.shouldDelete())
            continue;

        Network::UDPMessage msg(Network::UDPMessage::HEADER, Network::CL_VOIP);
        if (!client.getNet()->popIncommingData(msg, readCount))
            continue;

        std::cout << "coucou j'arrive" << std::endl;
    }
}

} // namespace gengine::interface::network::system