/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPManager
*/

#pragma once

#include "GEngine/BaseEngine.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/network/systems/ServerClient.hpp"

namespace Network {
class NetClient;
} // namespace Network

namespace gengine::interface::network::system {

class VoIPManager : public System<VoIPManager, interface::component::RemoteLocal,
                               gengine::interface::network::system::ServerClientsHandler>,
                 public RemoteSystem {

public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
};

} // namespace gengine::interface::network::system