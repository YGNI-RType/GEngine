/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Background.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

#define DEFAULT_BACKGROUND_SPEED 3

namespace poc3d::component {
struct Background : public gengine::Component<Background> {
    float speed;

    Background(float newSpeed = DEFAULT_BACKGROUND_SPEED)
        : speed(newSpeed) {
    }
};
} // namespace poc3d::component
