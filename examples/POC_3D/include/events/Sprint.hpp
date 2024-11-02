/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Sprint.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace poc3d::event {
struct Sprint : public gengine::Event {
    bool sprinting;

    Sprint(bool sprinting = false)
        : sprinting(sprinting) {
    }
};
} // namespace poc3d::event
