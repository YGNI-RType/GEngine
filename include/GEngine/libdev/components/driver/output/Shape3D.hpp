/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Shape3D.hpp
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
struct Cube : public Component<Cube> {
    float width;
    float height;
    float length;
    Clr tint;

    Cube(float width, float height, float length, const Clr &tint = RAYWHITE)
        : width(width)
        , height(height)
        , length(length)
        , tint(tint) {
    }

    bool operator==(const Cube &) const = default;
};
} // namespace gengine::component::driver::output
