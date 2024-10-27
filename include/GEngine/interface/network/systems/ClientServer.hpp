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
#include "GEngine/libdev/System.hpp"
#include "GEngine/net/events/connection.hpp"
#include "GEngine/net/net.hpp"

namespace gengine::interface::network::system {
class ClientServer : public System<ClientServer>, public LocalSystem {
public:
    void init(void) override;

    void onConnect(gengine::interface::network::event::ConnectToServer &);

    void onDisconnect(gengine::interface::network::event::DisconnectFromServer &);

    // void onPing(gengine::interface::network::event::Ping &);
    // void onBroadcast(gengine::interface::network::event::Ping &);
    private:
        bool m_connected = false;
};
} // namespace gengine::interface::network::system
