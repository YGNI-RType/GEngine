/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** FlappingMotion.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Flapping.hpp"

namespace rtype::system {
class FlappingMotion : public gengine::System<FlappingMotion, geg::component::Velocity2D, geg::component::Transform2D,
                                              component::Flapping, geg::component::network::NetSend>,
                       public gengine::RemoteSystem {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
};
} // namespace rtype::system
