
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

#include <raylib.h>

namespace poc3d::system {
class Init : public gengine::OnEventSystem<Init, gengine::system::event::StartEngine>, public gengine::LocalSystem {
public:
    void onEvent(gengine::system::event::StartEngine &e) {
        publishEvent(gengine::interface::network::event::ConnectToServer("127.0.0.1", 4243));

        // Crosshair
        float crosshairSize = 3;
        float crosshairBorder = 0.7;
        Color whiteTransp = {255, 255, 255, 100};
        spawnEntity(
            geg::component::Transform2D({WINDOW_WIDTH / 2 - crosshairSize / 2, WINDOW_HEIGHT / 2 - crosshairSize / 2},
                                        {crosshairSize, crosshairSize}),
            gengine::component::driver::output::Drawable(5),
            gengine::component::driver::output::Circle(crosshairSize / 2 + crosshairBorder, BLACK));
        spawnEntity(
            geg::component::Transform2D({WINDOW_WIDTH / 2 - crosshairSize / 2, WINDOW_HEIGHT / 2 - crosshairSize / 2},
                                        {crosshairSize, crosshairSize}),
            gengine::component::driver::output::Drawable(5),
            gengine::component::driver::output::Circle(crosshairSize / 2, whiteTransp));
    }
};
} // namespace poc3d::system
