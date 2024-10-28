/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** ping_result
*/

#pragma once

#include <cstdint>
#include <string>

namespace Network::Event {

struct PingInfo {
    std::string ip;
    uint16_t port;

    uint32_t currentPlayers;
    uint32_t maxPlayers;

    uint8_t os;
    uint64_t pingMs;
};
} // namespace Network::Event
