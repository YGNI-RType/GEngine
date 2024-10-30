/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Jump.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace poc3d::event {
struct Jump : public gengine::Event {
    float strength;

    Jump(float strength = 0.1)
        : strength(strength) {
    }
};
} // namespace poc3d::event
