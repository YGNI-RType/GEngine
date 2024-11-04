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
    std::uint64_t _ack = 0;

    Sound(std::uint64_t soundId)
        : soundId(soundId) {
    }
};
} // namespace gengine::component::driver::output
