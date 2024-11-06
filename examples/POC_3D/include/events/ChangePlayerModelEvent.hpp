/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ChangePlayerModelEvent.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

#include "GEngine/net/net_string.hpp"

namespace poc3d::event {
struct ChangePlayerModelEvent : public gengine::Event {
    Network::NetString<32> modelPath;
    bool isAnimated;
    Network::NetString<32> animPath;
    float frameDuration;

    ChangePlayerModelEvent(const std::string &modelPath)
        : modelPath(modelPath)
        , isAnimated(false) {
    }

    ChangePlayerModelEvent(const std::string &modelPath, const std::string &animPath, float frameDuration)
        : modelPath(modelPath)
        , isAnimated(true)
        , animPath(animPath)
        , frameDuration(frameDuration) {
    }
};
} // namespace poc3d::event
