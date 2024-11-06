/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BecomeInvincible.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct BecomeInvincible : public gengine::Event {
    BecomeInvincible(bool state)
        : state(state) {
    }

    bool state;
};
} // namespace rtype::event
