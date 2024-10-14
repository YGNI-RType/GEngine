/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BackgroundAutoMotion.cpp
*/

#include "systems/BackgroundMotion.hpp"

namespace rtype::system {
void BackgroundMotion::init(void) {
    // TODO replace by an other event
    // This is for update speed for simulate an hyper speed for example
    // subscribeToEvent<gengine::system::event::MainLoop>(&BackgroundAutoMotion::updateBackgroundSpeed);

    // TODO doesn't work because background is create after
    // subscribeToEvent<gengine::system::event::StartEngine>(&BackgroundMotion::updateBackgroundSpeed);

    subscribeToEvent<gengine::system::event::MainLoop>(&BackgroundMotion::onMainLoop);
}

void BackgroundMotion::onMainLoop(gengine::system::event::MainLoop &e) {
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &backgrounds = getComponents<component::Background>();

    for (auto [entity, transform, background] : gengine::Zip(transforms, backgrounds))
        if (transform.pos.x < -1226 * 3.48 + 1280)
            transform.pos.x = 0;
}

void BackgroundMotion::updateBackgroundSpeed(void) {
    auto &motions = getComponents<gengine::component::Velocity2D>();
    auto &backgrounds = getComponents<component::Background>();

    for (auto [entity, motion, background] : gengine::Zip(motions, backgrounds))
        motion.x = background.speed;
}
} // namespace rtype::system
