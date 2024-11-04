/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** NetworkWatcher
*/

#pragma once

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Base.hpp"

#include "GEngine/libdev/systems/events/GameLoop.hpp"

namespace gengine::interface::network::system {
class NetworkWatcher : public System<NetworkWatcher> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
};

} // namespace gengine::interface::network
