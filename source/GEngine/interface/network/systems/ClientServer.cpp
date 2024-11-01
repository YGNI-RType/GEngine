/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** ClientServer.cpp
*/

#include "GEngine/interface/network/systems/ClientServer.hpp"

namespace gengine::interface::network::system {
void ClientServer::init(void) {
    subscribeToEvent<event::ConnectToServer>(&ClientServer::onConnect);
    subscribeToEvent<event::DisconnectFromServer>(&ClientServer::onDisconnect);
}

void ClientServer::onConnect(event::ConnectToServer &e) {
    if (m_connected)
        return;
    auto &eventManager = Network::NET::getEventManager();

    eventManager.addEvent(Network::Event::CONNECT, Network::Event::ConnectInfo(e.ip, e.port));
    m_connected = true;
}

void ClientServer::onDisconnect(event::DisconnectFromServer &e) {
    auto &eventManager = Network::NET::getEventManager();
    Network::NET::getClient().disconnectFromServer(Network::Event::DT_WANTED);
}
} // namespace gengine::interface::network::system