/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** BaseScene.cpp
*/

#pragma once

namespace gengine::system::gui {
// template <class... DependTypes>
BaseScene::BaseScene(short sceneId)
    : m_sceneId(sceneId) {
}

// template <class... DependTypes>
void BaseScene::init(void) {
    subscribeToEvent<event::gui::ClearScene>(&BaseScene::onClear);
    subscribeToEvent<event::gui::SpawnScene>(&BaseScene::onSpawn);
}

// template <class... DependTypes>
void BaseScene::onClear(event::gui::ClearScene &e) {
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
} // namespace gengine::system::gui