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

namespace gengine::interface::network::event {
struct ConnectToServer : gengine::Event {
    std::string ip;
    uint16_t port;

    ConnectToServer(const std::string &ip, uint16_t port)
        : ip(ip)
        , port(port) {
    }
};

struct DisconnectFromServer : gengine::Event {
    DisconnectFromServer() {
    }
};

struct PingLan : gengine::Event {
    PingLan() {
    }
};
} // namespace gengine::interface::network::event
