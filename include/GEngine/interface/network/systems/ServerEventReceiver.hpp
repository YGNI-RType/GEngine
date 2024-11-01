/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : ServerEventReceiver.hpp
**  Create at   : 2024-10-15 04:51
**  Author      : AUTHOR
**  Description : This system catch all SharedEvent send by ClientEventPublisher
                    and publis them to GameEngine event Bus.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/interface/network/systems/ServerClients.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/net/net.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace gengine::interface::network::system {

template <class... Events>
class ServerEventReceiver
    : public System<ServerEventReceiver<Events...>, gengine::interface::network::system::ServerClientsHandler>,
      public RemoteSystem {
public:
    ServerEventReceiver();

    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &e);

private:
    template <typename T>
    void dynamicPublish(void);

    std::uint64_t m_id = 0;
    const Network::NetServer &m_server;
    std::unordered_map<std::uint64_t, std::pair<std::function<void(void *, uuids::uuid &)>, size_t>> m_eventsCallbacks;
    mutable std::mutex m_netMutex;
};

#include "ServerEventReceiver.inl"

} // namespace gengine::interface::network::system
