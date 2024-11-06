/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** FlappingMotion.cpp
*/

#include "systems/FlappingMotion.hpp"
#include "events/EnemyShootEvent.hpp"

#include <random>

namespace rtype::system {
void FlappingMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&FlappingMotion::onGameLoop);
}

void FlappingMotion::onGameLoop(gengine::system::event::GameLoop &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> change_chance_fork(0, 10);
    std::uniform_int_distribution<> motion_fork(-1, 1);

    auto &motions = getComponents<geg::component::Velocity2D>();
    auto &flappings = getComponents<component::Flapping>();
    auto &netsends = getComponents<geg::component::network::NetSend>();
    for (auto [entity, motion, flapping, netsend] : gengine::Zip(motions, flappings, netsends))
        if (change_chance_fork(gen) == 0) {
            motion.y = motion_fork(gen);
            netsend.update();
        }
}
} // namespace rtype::system
