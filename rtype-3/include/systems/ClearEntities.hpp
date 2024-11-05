/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ClearEntities.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "components/Bullet.hpp"
#include "components/BulletEnemy.hpp"
#include "components/Monster.hpp"

namespace rtype::system {
class ClearEntities : public gengine::System<ClearEntities, geg::component::Transform2D, geg::component::io::Sprite,
                                             component::Bullet, component::BulletEnemy, component::Monster> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);

private:
    void clearBullets(void);
    void clearBulletsEnemy(void);
    void clearMonsters(void);
};
} // namespace rtype::system
