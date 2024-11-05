/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Invincible.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Invincible : public gengine::Component<Invincible> {
    Invincible(unsigned int duration_ = 2000)
        : duration(duration_) {
    }

    int duration;
};
} // namespace rtype::component
