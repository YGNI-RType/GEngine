/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Scenes.hpp
*/

#pragma once

#include <cstdint>

namespace gengine::system::event::gui {
struct SwitchScene : public gengine::Event {
    short sceneId;
    SwitchScene(short sceneId)
        : sceneId(sceneId) {
    }
};

struct SpawnScene : public gengine::Event {
    short sceneId;
    SpawnScene(short sceneId)
        : sceneId(sceneId) {
    }
};

struct ClearScene : public gengine::Event {
    short sceneId;
    ClearScene(short sceneId)
        : sceneId(sceneId) {
    }
};

} // namespace gengine::system::event::gui