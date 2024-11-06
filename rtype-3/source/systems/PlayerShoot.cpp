/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerShoot.cpp
*/

#include "systems/PlayerShoot.hpp"
#include "components/Bullet.hpp"

// Spawn entity
#include "GEngine/libdev/Component.hpp" // gengine::Zip
#include "GEngine/libdev/Components.hpp"

#define BULLET_SPEED 10

namespace rtype::system {
void PlayerShoot::init(void) {
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Shoot>>(&PlayerShoot::shoot);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&PlayerShoot::newShooter);
    subscribeToEvent<gengine::interface::event::DeleteRemoteLocal>(&PlayerShoot::deleteShooter);
    subscribeToEvent<geg::event::io::KeySpaceEvent>(&PlayerShoot::bulletSound);
}

void PlayerShoot::newShooter(gengine::interface::event::NewRemoteLocal &e) {
    m_shooterMap.insert({e.uuid, shoot_infos_t()});
}
void PlayerShoot::deleteShooter(gengine::interface::event::DeleteRemoteLocal &e) {
    m_shooterMap.erase(e.uuid);
}

void PlayerShoot::shoot(gengine::interface::event::SharedEvent<event::Shoot> &e) {
    auto it = m_shooterMap.find(e.remoteUUID);
    if (it == m_shooterMap.end())
        return;
    auto &[state, lastCharge] = it->second;
    if (e->state == event::Shoot::REST && state == event::Shoot::CHARGING) {
        state = event::Shoot::REST;
        if (getChargeDuration(lastCharge) > 500)
            shootBeam(e.remoteUUID, (getChargeDuration(lastCharge) - 500) / 100);
        return;
    }
    if (e->state == event::Shoot::CHARGING && state == event::Shoot::REST) {
        lastCharge = std::chrono::system_clock::now();
        state = event::Shoot::CHARGING;
        // spawnEntity(
        //     gengine::component::Transform2D({0, 0}, {2, 2}), gengine::component::Velocity2D(0, 0),
        //     gengine::component::io::Drawable(2),
        //     gengine::component::io::Sprite("r-typesheet1.gif", Rectangle{0, 51, 32, 32}, WHITE),
        //     gengine::component::io::Animation(
        //         "r-typesheet1.json/charging", 0.1f, gengine::component::io::AnimationTrack::Forward, 2));
        publishEvent(gengine::system::event::driver::output::Sound("shoot.mp3"));
        publishEvent(gengine::system::event::driver::output::Music("stage2.mp3"));
        shootBullet(e.remoteUUID);
    }
}

void PlayerShoot::bulletSound(geg::event::io::KeySpaceEvent &e) {
    // auto &soundMan = getSystem<geg::system::io::AudioManager>();
    // if (e.state == geg::event::io::InputState::PRESSED)
    //     PlaySound(soundMan.get("shoot.mp3"));
}

void PlayerShoot::shootBullet(const uuids::uuid &from) {
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();
    auto &transforms = getComponents<geg::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        if (player.getUUIDBytes() != from)
            continue;
        spawnEntity(component::Bullet(player.getUUIDString()),
                    geg::component::Transform2D({transform.pos.x + 93, transform.pos.y + 22}, {2, 2}, 0),
                    geg::component::Velocity2D(BULLET_SPEED, 0),
                    geg::component::io::Sprite("effects.gif", Rectangle{248, 85, 17, 12}, WHITE),
                    geg::component::io::Drawable(1), gengine::component::HitBoxSquare2D(17, 12),
                    geg::component::network::NetSend());
        return;
    }
}

void PlayerShoot::shootBeam(const uuids::uuid &from, int bulletScale) {
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();
    auto &transforms = getComponents<geg::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        if (player.getUUIDBytes() != from)
            continue;
        spawnBeam(transform.pos, player.getUUIDString(), bulletScale);
        return;
    }
}

void PlayerShoot::spawnBeam(const geg::component::Transform2D &tr, const std::string &from, int bulletScale) {
    Rectangle rect;
    gengine::Vect2 pos({tr.pos.x + 50, tr.pos.y + 22});

    switch (bulletScale) {
    case 0:
        rect = {231, 103, 17, 12};
        break;
    case 1:

        rect = {199, 120, 33, 12};
        break;
    case 2:
        rect = {167, 136, 49, 14};
        break;
    case 3:
        rect = {135, 154, 65, 14};
        break;
    default:
        rect = {104, 170, 81, 16};
        break;
    }

    spawnEntity(
        component::Bullet(from, true), geg::component::Transform2D(pos, {2, 2}),
        geg::component::Velocity2D(BULLET_SPEED * 2, 0), geg::component::HitBoxSquare2D(rect.width, rect.height),
        geg::component::io::Sprite("effects.gif", rect), geg::component::io::Animation("effects.json/beam", 0.06f),
        geg::component::io::Drawable(1), geg::component::network::NetSend());
}

long PlayerShoot::getChargeDuration(const std::chrono::time_point<std::chrono::system_clock> &lastCharge) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastCharge).count();
}

} // namespace rtype::system
