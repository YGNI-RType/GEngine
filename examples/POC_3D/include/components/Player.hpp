/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Player.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

#define DEFAULT_SPEED 0.06

namespace poc3d::component {
enum class PlayerState {
    WALKING,
    RUNNING,
    IDLING,
};
struct Player : public gengine::Component<Player> {
    float speed;
    float defaultSpeed;
    bool isLocked = false;

    Player(float defaultSpeed = DEFAULT_SPEED)
        : speed(defaultSpeed)
        , defaultSpeed(defaultSpeed) {
    }
};

} // namespace poc3d::component
