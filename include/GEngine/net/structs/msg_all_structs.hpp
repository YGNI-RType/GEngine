/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** msg_all_structs
*/

#pragma once

#include "../utils/pack.hpp"

#include <cstdint>

namespace Network {
    PACK(struct ALL_MessageCompressionHeader {
        uint16_t offset;
        uint16_t size;
    });
}