/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** SceneManager.cpp
*/

#include "GEngine/libdev/systems/gui/SceneManager.hpp"

namespace gengine::system::gui {
SceneManager::SceneManager(short firstSceneId)
    : m_currentScene(firstSceneId) {
}

void SceneManager::init(void) {
    subscribeToEvent<event::StartEngine>(&SceneManager::onStartEngine);
    subscribeToEvent<event::gui::SwitchScene>(&SceneManager::onSwitchScene);
}

void SceneManager::onStartEngine(event::StartEngine &e) {
    publishEvent<event::gui::SpawnScene>(m_currentScene);
}

void SceneManager::onSwitchScene(event::gui::SwitchScene &e) {
    publishEvent<event::gui::ClearScene>(m_currentScene);
    m_currentScene = e.sceneId;
    publishEvent<event::gui::SpawnScene>(m_currentScene);
}
} // namespace gengine::system::gui