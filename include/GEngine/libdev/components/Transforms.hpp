/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Transforms.hpp
**  Create at   : 2024-10-15 05:28
**  Auteur      : AUTHOR
**  Description : These components allow you to manage positions/size/rotation
                    linked to an entity in a basic way.
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/tools/Math.hpp"

namespace gengine::component {
struct Transform2D : public Component<Transform2D> {
    Vect2 pos;
    Vect2 scale;
    float rotation; // degree

    Transform2D()
        : pos{0, 0}
        , scale{1, 1}
        , rotation(0) {
    }
    Transform2D(const Vect2 &pos, const Vect2 &scale = {1, 1}, float rotation = 0.f)
        : pos(pos)
        , scale(scale)
        , rotation(rotation) {
    }

    bool operator==(const Transform2D &) const = default;
};

struct Transform3D : public Component<Transform3D> {
    Vect3 pos;
    Vect3 scale;
    Vect3 rotation;
    Transform3D(const Vect3 &pos, const Vect3 &scale = {1, 1, 1}, Vect3 rotation = {0, 0, 0})
        : pos(pos)
        , scale(scale)
        , rotation(rotation) {
    }

    bool operator==(const Transform3D &) const = default;
};
} // namespace gengine::component
