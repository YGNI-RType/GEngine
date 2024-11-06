/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BossSpawnWave.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/net/net_string.hpp"

namespace rtype::event {
struct BossSpawnWave : public gengine::Event {
    Network::NetString<32> waveName;

    BossSpawnWave(const std::string &waveName_)
        : waveName(waveName_) {
    }
};
} // namespace rtype::event
