/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerAnimation.hpp
*/

#pragma once

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Player.hpp"
#include "events/Movement.hpp"

namespace rtype::system {
class PlayerAnimation : public gengine::System<PlayerAnimation, gengine::interface::component::RemoteLocal,
                                               geg::component::io::Animation>,
                        public gengine::LocalSystem {
public:
    void init(void) override;
    void animatePlayer(gengine::interface::event::SharedEvent<event::Movement> &e);
};
} // namespace rtype::system
