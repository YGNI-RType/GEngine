/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Sprite.hpp
**  Create at   : 2024-10-15 05:26
**  Auteur      : AUTHOR
**  Description : DESCRIPTION // TODO popoche
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "module/raylib_safe.h"
#include <cstring>
#include <limits>
#include <string>

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/components/driver/output/RaylibTypes.hpp"
#include "GEngine/net/net_string.hpp"

namespace gengine::component::driver::output {
struct Sprite : public Component<Sprite> {
    Network::NetString<32> txtPath; // The texture of the sprite
    Rect src;                       // The portion of the texture to render (for sprite sheets/atlases)
    Clr tint;

    Sprite(const std::string &txtPathstr, const Rect &src, const Clr &tint = RAYWHITE)
        : txtPath(txtPathstr)
        , src(src)
        , tint(tint) {
    }
    Sprite(const std::string &txtPathstr, const Clr &tint = RAYWHITE)
        : txtPath(txtPathstr)
        , src(0.f, 0.f, std::numeric_limits<float>().max(), std::numeric_limits<float>().max())
        , tint(tint) {
    }

    bool operator==(const Sprite &) const = default;
};
} // namespace gengine::component::driver::output
