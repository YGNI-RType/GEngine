/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BoundingMotion.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "components/Bounding.hpp"

namespace rtype::system {
class BoundingMotion : public gengine::System<BoundingMotion, geg::component::Velocity2D, geg::component::Transform2D,
                                              geg::component::io::Sprite, component::Bounding> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
};
} // namespace rtype::system
