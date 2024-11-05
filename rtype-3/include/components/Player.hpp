/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Player.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

#define DEFAULT_SPEED 5
#define DEFAULT_LIFES 3

namespace rtype::component {
struct Player : public gengine::Component<Player> {
    float speed;
    int lifes;
    bool ready = false;

    Player(float newSpeed = DEFAULT_SPEED, int lifes = DEFAULT_LIFES)
        : speed(newSpeed)
        , lifes(lifes) {
    }

    bool operator==(const Player &) const = default;
};
} // namespace rtype::component
