/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShootEvent.hpp
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct EnemyShootEvent : public gengine::Event {
    gengine::Entity from;
    Network::NetString<32> bulletName;
    int bulletSpeed;

    EnemyShootEvent(size_t from_, const std::string &bulletName_, int bulletSpeed_)
        : from(from_)
        , bulletName(bulletName_)
        , bulletSpeed(bulletSpeed_) {
    }
};
} // namespace rtype::event
