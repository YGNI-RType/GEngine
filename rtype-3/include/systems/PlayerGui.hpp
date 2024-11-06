/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerGui.hpp
*/

#pragma once

#include "components/Life.hpp"
#include "components/Player.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/events/RemoteLocal.hpp"

namespace rtype::system {
class PlayerGui
    : public gengine::System<PlayerGui, gengine::interface::component::RemoteLocal, component::Player, component::Life>,
      public gengine::LocalSystem {
public:
    void init(void) override;
    void displayLife(gengine::system::event::GameLoop &);
    void setMe(gengine::interface::event::ItsMe &);

private:
    uuids::uuid m_me;

    size_t m_life = 1;
};
} // namespace rtype::system
