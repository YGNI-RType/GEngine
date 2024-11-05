/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Invincibility.cpp
*/

#include "GEngine/libdev/Component.hpp"

#include "systems/Invincibility.hpp"

namespace rtype::system {
void Invincibility::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&Invincibility::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::BecomeInvincible>>(&Invincibility::becomeInvincible);
}

void Invincibility::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &invincibles = getComponents<component::Invincible>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto &[entity, invincible] : invincibles) {
        if (invincible.duration > 0) {
            invincible.duration -= static_cast<int>(e.deltaTime);
        } else {
            unsetComponent<component::Invincible>(entity);
            if (sprites.contains(entity))
                sprites.get(entity).tint.a = 255;
        }
    }
}

void Invincibility::becomeInvincible(gengine::interface::event::SharedEvent<event::BecomeInvincible> &e) {
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    // TODO tester avec plusieurs joueurs
    for (auto &[entity, player] : players) {
        if (e.remoteUUID != player.getUUIDBytes())
            continue;
        setComponent(entity, component::Invincible(e->state ? INT_MAX : 0));
        if (sprites.contains(entity))
            sprites.get(entity).tint.a = 128;
    }
}
} // namespace rtype::system
