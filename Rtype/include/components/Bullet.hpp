/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Bullet.hpp
*/

#pragma once

#include <cstring>

#include "GEngine/libdev/Component.hpp"
#include "GEngine/net/net_string.hpp"

namespace rtype::component {
struct Bullet : public gengine::Component<Bullet> {
    Bullet(std::string fromString, bool isBeam = false)
        : from(fromString)
        , isBeam(isBeam) {
    }

    Network::NetString<37> from;
    bool isBeam = false;
};
} // namespace rtype::component
