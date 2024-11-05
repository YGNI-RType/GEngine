/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BulletManager.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "Types.hpp"
#include "components/BulletEnemy.hpp"
#include "components/Monster.hpp"
#include "events/EnemyShootEvent.hpp"

#include <unordered_map>

namespace rtype::system {
class BulletManager
    : public gengine::System<BulletManager, component::BulletEnemy, component::Monster, geg::component::io::Sprite,
                             geg::component::io::Animation, geg::component::io::Drawable,
                             geg::component::HitBoxSquare2D, geg::component::Transform2D, geg::component::Velocity2D,
                             gengine::interface::component::RemoteLocal, geg::component::network::NetSend>,
      public gengine::RemoteSystem {
public:
    BulletManager(const std::string &);
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &);

    void determineVelocity(const geg::component::Transform2D &, const geg::component::io::Sprite &,
                           geg::component::Velocity2D &);
    void spawn(const Ammo &);
    void spawn(event::EnemyShootEvent &);
    const std::vector<Bullet> &get(const std::string &) const;

private:
    std::string m_folder;
    std::unordered_map<std::string, std::vector<Bullet>> m_bullets;
};
} // namespace rtype::system
