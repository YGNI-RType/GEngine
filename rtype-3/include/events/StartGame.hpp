/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** StartGame.hpp
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct IAmReady : public gengine::Event {
    bool ready;
    IAmReady(bool isReady)
        : ready(isReady) {
    }
};

struct StartGame : public gengine::Event {
    StartGame() = default;
};
} // namespace rtype::event
