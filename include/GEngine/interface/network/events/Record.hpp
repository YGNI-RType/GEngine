/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Record.hpp
*/

#pragma once

#include <cstdint>
#include <string>

#include "GEngine/libdev/System.hpp"

namespace gengine::interface::network::event {
struct ToggleRecord : gengine::Event {
    ToggleRecord() {
    }
};

struct WatchReplay : gengine::Event {
    WatchReplay(const std::string &demoPath)
        : demoPath(demoPath) {
    }
    std::string demoPath;
};
} // namespace gengine::interface::network::event
