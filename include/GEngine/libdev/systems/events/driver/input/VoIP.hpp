/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RemoteLocal.hpp
*/

#pragma once

#include <cstdint>
#include <string>

#include "GEngine/libdev/System.hpp"

namespace gengine::system::event::driver::input {
struct StartVoIP : gengine::Event {
    StartVoIP() {
    }
};

struct EndVoIP : gengine::Event {
    EndVoIP() {
    }
};
} // namespace gengine::system::event::driver::input
