/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : SceneMember.hpp
**  Create at   : 2024-10-15 05:27
**  Auteur      : AUTHOR
**  Description :
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace gengine::component::gui {
struct SceneMember : public Component<SceneMember> {
    short sceneId;
    SceneMember(short sceneId)
        : sceneId(sceneId) {
    }

    bool operator==(const SceneMember &) const = default;
};
} // namespace gengine::component::gui
