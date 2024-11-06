/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Monster.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

#define DEFAULT_MONSTER_LIVES 1

namespace rtype::component {
struct Monster : public gengine::Component<Monster> {
    Monster(int lives = DEFAULT_MONSTER_LIVES)
        : lives(lives) {
    }

    int lives;
};
} // namespace rtype::component
