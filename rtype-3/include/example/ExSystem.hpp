/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ExSystem.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "example/ExComponent.hpp"
#include "example/ExEvent.hpp"

namespace rtype::system {

class ExSystem : public gengine::System<ExSystem, component::ExComponent> {
public:
    void init(void) override;
    void onCustomEvent(event::ExEvent &);
    void onGameLoop(gengine::system::event::GameLoop &);
};
} // namespace rtype::system
