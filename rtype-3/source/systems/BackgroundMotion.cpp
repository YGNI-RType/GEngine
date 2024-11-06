/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BackgroundAutoMotion.cpp
*/

#include "systems/BackgroundMotion.hpp"

namespace rtype::system {
void BackgroundMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&BackgroundMotion::onGameLoop);
}

void BackgroundMotion::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &backgrounds = getComponents<component::Background>();

    for (auto [entity, transform, background] : gengine::Zip(transforms, backgrounds))
        if (transform.pos.x < -1119 * 3.48 - 1)
            transform.pos.x = 1119 * 3.48;
}
} // namespace rtype::system
