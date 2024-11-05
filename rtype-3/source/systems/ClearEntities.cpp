/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ClearEntities.cpp
*/

#include "systems/ClearEntities.hpp"
#include "Constants.hpp"
#include "ecs/system/Base.hpp"

namespace rtype::system {
void ClearEntities::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&ClearEntities::onGameLoop);
}

void ClearEntities::onGameLoop(gengine::system::event::GameLoop &e) {
    clearBullets();
    clearBulletsEnemy();
    clearMonsters();
}

void ClearEntities::clearBullets(void) {
    auto &bullets = getComponents<component::Bullet>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, bullet, transform] : gengine::Zip(bullets, transforms))
        if (transform.pos.x > WINDOW_WIDTH)
            killEntity(entity);
}

void ClearEntities::clearBulletsEnemy(void) {
    auto &bulletsEnemy = getComponents<component::BulletEnemy>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto [entity, bullet, sprite, transform] : gengine::Zip(bulletsEnemy, sprites, transforms))
        if (transform.pos.x < -(sprite.src.width * transform.scale.x) ||
            transform.pos.x > WINDOW_WIDTH + sprite.src.width * transform.scale.x ||
            transform.pos.y < -(sprite.src.height * transform.scale.y) ||
            transform.pos.y > WINDOW_HEIGHT + sprite.src.height * transform.scale.y)
            killEntity(entity);
}

void ClearEntities::clearMonsters(void) {
    auto &monsters = getComponents<component::Monster>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto [entity, monster, transform, sprite] : gengine::Zip(monsters, transforms, sprites))
        if (transform.pos.x < -(sprite.src.width * transform.scale.x))
            killEntity(entity);
}
} // namespace rtype::system
