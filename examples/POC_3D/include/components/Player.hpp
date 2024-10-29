/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Player.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include <string>

#define DEFAULT_SPEED 0.06

namespace rtype::component {
struct Player : public gengine::Component<Player> {
    float speed;

    Player(float newSpeed = DEFAULT_SPEED)
        : speed(newSpeed) {
    }
};

} // namespace rtype::component
