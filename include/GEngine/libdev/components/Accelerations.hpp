/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Accelerations.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace gengine::component {
struct Acceleration2D : public Component<Acceleration2D> {
    float x, y;
    Acceleration2D(float x, float y)
        : x(x)
        , y(y) {
    }

    bool operator==(const Acceleration2D &) const = default;
};

struct Acceleration3D : public Component<Acceleration3D> {
    float x, y, z;
    Acceleration3D(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z) {
    }

    bool operator==(const Acceleration3D &) const = default;
};
} // namespace gengine::component
