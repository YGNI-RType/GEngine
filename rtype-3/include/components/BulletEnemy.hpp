/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BulletEnemy.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct BulletEnemy : public gengine::Component<BulletEnemy> {
    BulletEnemy() {
    }
};
} // namespace rtype::component
