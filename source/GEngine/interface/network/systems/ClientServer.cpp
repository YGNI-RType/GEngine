/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** ClientServer.cpp
*/

#include "GEngine/interface/network/systems/ClientServer.hpp"

namespace gengine::interface::network::system {
void ClientServer::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&ClientServer::onStartEngine);
    subscribeToEvent<event::ConnectToServer>(&ClientServer::onConnect);
    subscribeToEvent<event::DisconnectFromServer>(&ClientServer::onDisconnect);
    subscribeToEvent<event::PingLan>(&ClientServer::onPingLan);
}

void ClientServer::onStartEngine(gengine::system::event::StartEngine &e) {
    auto &eventManager = Network::NET::getEventManager();
    eventManager.registerCallback<Network::Event::PingInfo>(Network::Event::CT_OnPingResult,
                                                            [this](Network::Event::PingInfo info) -> void {
                                                                std::lock_guard<std::mutex> lock(m_netMutex);
                                                                m_pingInfos.push_back(info);
                                                            });
}

void ClientServer::onConnect(event::ConnectToServer &e) {
    if (m_connected)
        return;
    auto &eventManager = Network::NET::getEventManager();

    eventManager.addEvent(Network::Event::CONNECT, Network::Event::ConnectInfo(e.ip, e.port));
    m_serverIp = e.ip;
    m_serverPort = e.port;
    m_connected = true;
}

void ClientServer::onDisconnect(event::DisconnectFromServer &e) {
    auto &eventManager = Network::NET::getEventManager();

    std::lock_guard<std::mutex> lock(m_netMutex);
    Network::NET::getClient().disconnectFromServer(Network::Event::DT_WANTED);
    m_connected = false;
}

void ClientServer::onPingLan(event::PingLan &e) {
    Network::Event::Manager &em = Network::NET::getEventManager();
    std::lock_guard<std::mutex> lock(m_netMutex);
    em.addEvent<int>(Network::Event::PING_LAN, 0);
    m_pingInfos.clear();
}

bool ClientServer::isConnected(void) const {
    return m_connected;
}
const std::string &ClientServer::getServerIp(void) const {
    return m_serverIp;
}
uint16_t ClientServer::getServerPort(void) const {
    return m_serverPort;
}
std::vector<Network::Event::PingInfo> ClientServer::getPingInfos(void) const {
    std::lock_guard<std::mutex> lock(m_netMutex);

    return m_pingInfos;
}

ConnectAtStart::ConnectAtStart(const std::string &ip, uint16_t port)
    : m_serverIp(ip)
    , m_serverPort(port) {
}

void ConnectAtStart::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&ConnectAtStart::onStartEngine);
}

void ConnectAtStart::onStartEngine(gengine::system::event::StartEngine &e) {
    publishEvent(event::ConnectToServer(m_serverIp, m_serverPort));
}
} // namespace gengine::interface::network::system