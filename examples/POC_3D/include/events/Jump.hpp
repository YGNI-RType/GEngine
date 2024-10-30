/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Jump.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct Jump : public gengine::Event {
    float strength;

    Jump(float strength = 0.1)
        : strength(strength) {
    }
};
} // namespace rtype::event
