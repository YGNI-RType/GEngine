/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** LockPlayerEvent.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace poc3d::event {
struct LockPlayerEvent : public gengine::Event {
    bool locked;

    LockPlayerEvent(bool locked)
        : locked(locked) {
    }
};
} // namespace poc3d::event
