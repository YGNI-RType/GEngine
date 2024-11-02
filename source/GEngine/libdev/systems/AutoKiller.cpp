/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** AutoKiller.cpp
*/

#include "GEngine/libdev/systems/AutoKiller.hpp"
#include <queue>

namespace gengine::system {

void AutoKiller::init(void) {
    subscribeToEvent<event::GameLoop>(&AutoKiller::onGameLoop);
}

void AutoKiller::onGameLoop(event::GameLoop &e [[maybe_unused]]) {
    auto &spanlifes = getComponents<component::SpanLife>();
    std::queue<ecs::entity::Entity> toKill;

    for (auto &[entity, spanlife] : spanlifes) {
        spanlife.time -= e.deltaTime / 1000.f;
        if (spanlife.time < 0)
            toKill.push(entity);
    }
    while (!toKill.empty()) {
        killEntity(toKill.front());
        toKill.pop();
    }
}
} // namespace gengine::system
