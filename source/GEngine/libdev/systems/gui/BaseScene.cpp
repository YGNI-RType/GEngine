/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** BaseScene.cpp
*/

#include "GEngine/libdev/systems/gui/BaseScene.hpp"

namespace gengine::system::gui {
BaseScene::BaseScene(short sceneId)
    : m_sceneId(sceneId) {
}

void BaseScene::init(void) {
    subscribeToEvent<event::gui::ClearScene>(&BaseScene::onClear);
    subscribeToEvent<event::gui::SpawnScene>(&BaseScene::onSpawn);
}

void BaseScene::onClear(event::gui::ClearScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    auto &members = getComponents<component::gui::SceneMember>();
    std::queue<ecs::entity::Entity> toKill;

    for (auto &[entity, member] : members) {
        if (m_sceneId == member.sceneId)
            toKill.push(entity);
    }
    while (!toKill.empty()) {
        killEntity(toKill.front());
        toKill.pop();
    }
}
}
