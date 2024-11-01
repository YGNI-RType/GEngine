/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : ClientEventPublisher.hpp
**  Create at   : 2024-10-15 04:49
**  Author      : AUTHOR
**  Description : DESCRIPTION
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>

#include "GEngine/interface/network/events/Connection.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/net/events/connection.hpp"
#include "GEngine/net/events/ping_result.hpp"
#include "GEngine/net/net.hpp"

namespace gengine::interface::network::system {
class ClientServer : public System<ClientServer>, public LocalSystem {
public:
    ClientServer() = default;

    // Constructor if you want to connect at StartEngine
    ClientServer(const std::string &ip, uint16_t port);

    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &);

    void onConnect(event::ConnectToServer &);

    void onDisconnect(event::DisconnectFromServer &);

    // void onPing(event::Ping &);
    void onPingLan(event::PingLan &);

    bool isConnected(void) const;
    const std::string &getServerIp(void) const;
    uint16_t getServerPort(void) const;
    std::vector<Network::Event::PingInfo> getPingInfos(void) const;
private:
    std::string m_serverIp = "";
    uint16_t m_serverPort = 0;

    bool m_connected = false;

    std::vector<Network::Event::PingInfo> m_pingInfos;

    mutable std::mutex m_netMutex;
};
} // namespace gengine::interface::network::system
