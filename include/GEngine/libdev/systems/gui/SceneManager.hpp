/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** SceneManager.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/gui/Scenes.hpp"

namespace gengine::system::gui {
class SceneManager : public System<SceneManager>, public LocalSystem {
public:
    SceneManager(short firstSceneId);

    void init(void) override;

    void onStartEngine(event::StartEngine &e);

    void onSwitchScene(event::gui::SwitchScene &e);

private:
    short m_currentScene;
};
} // namespace gengine::system::gui