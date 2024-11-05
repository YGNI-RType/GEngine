/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Boss.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Boss : public gengine::Component<Boss> {
    Boss(const std::string &bossName_)
        : bossName(bossName_)
        , waveSpawnInSec(0) {
    }

    Network::NetString<32> bossName;
    int waveSpawnInSec;
};
} // namespace rtype::component
