/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ExSystem.cpp
*/

#include "example/ExSystem.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

namespace rtype::system {
void ExSystem::init(void) {
    subscribeToEvent<event::ExEvent>(&ExSystem::onCustomEvent);
    subscribeToEvent<gengine::system::event::GameLoop>(&ExSystem::onGameLoop);
}

void ExSystem::onCustomEvent(event::ExEvent &e) {
    auto &components = getComponents<component::ExComponent>();
    for (auto &[entity, component] : components) {
        // Do something with component
    }
}

void ExSystem::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &components = getComponents<component::ExComponent>();

    for (auto &[entity, component] : components) {
        // Do something with component
    }
}

} // namespace rtype::system
