/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Jump.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include <stduuid/uuid.h>

namespace poc3d::event {
struct Jump : public gengine::Event {
    float strength;
    uuids::uuid playerUuid = uuids::uuid{};

    Jump(float strength = 0.1)
        : strength(strength) {
    }

    Jump(double strength = 0.1)
        : strength(strength) {
    }

    Jump(float strength = 0.1, uuids::uuid playerUuid = uuids::uuid{})
        : strength(strength)
        , playerUuid(playerUuid) {
    }
};
} // namespace poc3d::event
