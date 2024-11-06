/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ChangePlayerModelEvent.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

#include "GEngine/net/net_string.hpp"

#include "GEngine/libdev/components/Transforms.hpp"

namespace poc3d::event {
struct ChangePlayerModelEvent : public gengine::Event {
    Network::NetString<32> modelPath;
    bool isAnimated;
    Network::NetString<32> animPath;
    float frameDuration;
    gengine::component::Transform3D transform = {gengine::Vect3{0, 0, 0}};

    ChangePlayerModelEvent(const std::string &modelPath)
        : modelPath(modelPath)
        , isAnimated(false) {
    }

    ChangePlayerModelEvent(const std::string &modelPath, gengine::component::Transform3D transform)
        : modelPath(modelPath)
        , isAnimated(false)
        , transform(transform) {
    }

    ChangePlayerModelEvent(const std::string &modelPath, gengine::Vect3 scale)
        : modelPath(modelPath)
        , isAnimated(false) {
        transform = gengine::component::Transform3D({0, 0, 0}, scale);
    }

    ChangePlayerModelEvent(const std::string &modelPath, const std::string &animPath, float frameDuration,
                           gengine::component::Transform3D transform)
        : modelPath(modelPath)
        , isAnimated(true)
        , animPath(animPath)
        , frameDuration(frameDuration)
        , transform(transform) {
    }

    ChangePlayerModelEvent(const std::string &modelPath, const std::string &animPath, float frameDuration,
                           gengine::Vect3 scale)
        : modelPath(modelPath)
        , isAnimated(true)
        , animPath(animPath)
        , frameDuration(frameDuration) {
        transform = gengine::component::Transform3D({0, 0, 0}, scale);
    }
};
} // namespace poc3d::event
