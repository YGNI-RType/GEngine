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
struct ToogleRecord : gengine::Event {
    ToogleRecord() {
    }
};
} // namespace gengine::interface::network::event
