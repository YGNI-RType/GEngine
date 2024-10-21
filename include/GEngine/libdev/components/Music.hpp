/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Music.hpp
**  Create at   : 2024-10-17 15:03
**  Auteur      : AUTHOR
**  Description : DESCRIPTION
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace gengine::component {
struct Music: public gengine::Component<Music> {
    std::uint64_t musicId;
    Music(std::uint64_t musicId): musicId(musicId) {
    }
};
}
