/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** BaseScene.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/components/gui/SceneMember.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/gui/Scenes.hpp"

namespace gengine::system::gui {
// template <class... DependTypes>
class BaseScene : public System<BaseScene, component::gui::SceneMember>, public LocalSystem {
public:
    BaseScene(short sceneId);

    void init(void) override;

    virtual void onSpawn(event::gui::SpawnScene &e) = 0;

    virtual void onClear(event::gui::ClearScene &e);

    // virtual void onUpdate(gengine::system::event::GameLoop &e);

protected:
    short m_sceneId;
};
} // namespace gengine::system::gui
