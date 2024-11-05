/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Invincibility.hpp
*/

#pragma once

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Invincible.hpp"
#include "events/BecomeInvincible.hpp"

namespace rtype::system {

class Invincibility : public gengine::System<Invincibility, component::Invincible, geg::component::io::Sprite,
                                             gengine::interface::component::RemoteLocal>,
                      public gengine::RemoteSystem {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void becomeInvincible(gengine::interface::event::SharedEvent<event::BecomeInvincible> &e);
};
} // namespace rtype::system
