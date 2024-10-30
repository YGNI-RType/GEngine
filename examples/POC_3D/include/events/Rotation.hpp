/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Rotation.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace poc3d::event {
struct Rotation : public gengine::Event {
    gengine::Vect3 move = {0, 0, 0};

    Rotation(gengine::Vect3 move)
        : move(move) {
    }
};
} // namespace poc3d::event
