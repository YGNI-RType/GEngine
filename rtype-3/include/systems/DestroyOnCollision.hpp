/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** DestroyOnCollision.hpp
*/

#pragma once

#include "ecs/entity/Entity.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"

#include "components/Bullet.hpp"
#include "components/BulletEnemy.hpp"
#include "components/Invincible.hpp"
#include "components/Life.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/Score.hpp"

#include "ecs/entity/Entity.hpp"

namespace rtype::system {
class DestroyOnCollision
    : public gengine::System<DestroyOnCollision, component::Bullet, component::BulletEnemy, component::Monster,
                             component::Life, component::Player, component::Invincible, geg::component::io::Sprite,
                             gengine::interface::component::RemoteLocal, gengine::component::Transform2D,
                             component::Score, gengine::component::driver::output::Drawable,
                             geg::component::HitBoxSquare2D, gengine::component::driver::output::Text> {
public:
    void init(void) override;
    void destroyMonster(geg::event::Collision &);
    void destroyPlayer(geg::event::Collision &);

private:
    void spawnExplosion(gengine::Entity);
    void claimScore(gengine::Entity, const char *);
    void playerHit(gengine::Entity, component::Player &, gengine::component::Transform2D &);
};
} // namespace rtype::system
