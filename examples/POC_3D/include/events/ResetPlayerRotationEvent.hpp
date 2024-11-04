/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ResetPlayerRotationEvent.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace poc3d::event {
struct ResetPlayerRotationEvent : public gengine::Event {
    float rotation;

    ResetPlayerRotationEvent(float rotation)
        : rotation(rotation) {
    }
};
} // namespace poc3d::event
