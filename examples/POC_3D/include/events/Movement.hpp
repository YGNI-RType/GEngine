/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Movement.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct Movement : public gengine::Event {
    enum State { STANDING = 0, FWD, RIGHT, BCK, LEFT, FWD_RIGHT, FWD_LEFT, BCK_RIGHT, BCK_LEFT };

    State state;

    Movement(State state)
        : state(state) {
    }
};
} // namespace rtype::event
