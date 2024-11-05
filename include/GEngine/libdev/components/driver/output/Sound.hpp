/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Sound.hpp
**  Create at   : 2024-11-03 23:55
**  Auteur      : b3w
**  Description : DESCRIPTION
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace gengine::component::driver::output {
struct Sound : public gengine::Component<Sound> {
    std::uint64_t soundId;

    Sound(std::uint64_t soundId)
        : soundId(soundId) {
    }
};

struct Music : public gengine::Component<Music> {
    bool pause = false;
    std::uint64_t musicId;

    Music(std::uint64_t musicId)
        : musicId(musicId) {
    }

    bool operator==(const Music &) const = default;
};
} // namespace gengine::component::driver::output
