
/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Init.hpp
*/

#pragma once

#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"

#include "GEngine/interface/network/events/Connection.hpp"

namespace poc3d::system {
class Init : public gengine::OnEventSystem<Init, gengine::system::event::StartEngine>, public gengine::LocalSystem {
public:
    void onEvent(gengine::system::event::StartEngine &e) {
        publishEvent(gengine::interface::network::event::ConnectToServer("127.0.0.1", 4243));
    }
};
} // namespace poc3d::system
