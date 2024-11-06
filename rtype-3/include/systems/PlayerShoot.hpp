/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerShoot.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"

#include "events/Shoot.hpp"

namespace rtype::system {
class PlayerShoot : public gengine::System<PlayerShoot, gengine::interface::component::RemoteLocal,
                                           geg::component::Transform2D, geg::system::io::AudioManager> {
public:
    void init(void) override;
    void newShooter(gengine::interface::event::NewRemoteLocal &);
    void deleteShooter(gengine::interface::event::DeleteRemoteLocal &);
    void shoot(gengine::interface::event::SharedEvent<event::Shoot> &);
    void bulletSound(geg::event::io::KeySpaceEvent &);

private:
    void shootBullet(const uuids::uuid &from);
    void shootBeam(const uuids::uuid &from, int bulletScale);
    void spawnBeam(const gengine::component::Transform2D &tr, const std::string &from, int bulletScale);
    long getChargeDuration(const std::chrono::time_point<std::chrono::system_clock> &lastCharge);

    using shoot_infos_t = std::pair<event::Shoot::State, std::chrono::time_point<std::chrono::system_clock>>;
    std::unordered_map<uuids::uuid, shoot_infos_t> m_shooterMap;
};
} // namespace rtype::system
