/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Player.hpp"
#include "events/Movement.hpp"

namespace rtype::system {
class PlayerMotion
    : public gengine::System<PlayerMotion, gengine::interface::component::RemoteLocal, geg::component::Velocity2D,
                             geg::component::Transform2D, component::Player> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void movePlayer(gengine::interface::event::SharedEvent<event::Movement> &e);
};
} // namespace rtype::system
