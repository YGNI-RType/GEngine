/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BulletManager.cpp
*/

#include "systems/BulletManager.hpp"
#include "Constants.hpp"

#include <filesystem>
#include <fstream>
#include <random>

namespace rtype::system {
BulletManager::BulletManager(const std::string &folder)
    : m_folder(folder) {
}

void BulletManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&BulletManager::onStartEngine);
    subscribeToEvent<event::EnemyShootEvent>(&BulletManager::spawn);
}

void BulletManager::determineVelocity(const geg::component::Transform2D &mobTransform,
                                      const geg::component::io::Sprite &mobSprite,
                                      geg::component::Velocity2D &bulletVelocity) {
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, players.size());
    int randomPlayer = dist6(rng);

    for (auto [entity, player, playerTransform, playerSprite] : gengine::Zip(players, transforms, sprites)) {
        if (randomPlayer > 1) {
            randomPlayer--;
            continue;
        }

        geg::component::Velocity2D velocity(
            playerTransform.pos.x + (playerSprite.src.width * playerTransform.scale.x) / 2.0f - mobTransform.pos.x +
                (mobSprite.src.width * mobTransform.scale.x) / 2.0f,
            playerTransform.pos.y + (playerSprite.src.height * playerTransform.scale.y) / 2.0f - mobTransform.pos.y +
                (mobSprite.src.height * mobTransform.scale.y) / 2.0f);

        float norm = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
        if (norm == 0)
            break;

        bulletVelocity.x = velocity.x / norm * bulletVelocity.x;
        bulletVelocity.y = velocity.y / norm * bulletVelocity.y;

        break;
    }
}

void BulletManager::spawn(const Ammo &ammo) {
    auto bullets = get(ammo.bulletName);
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    if (!transforms.contains(ammo.mobId) || !sprites.contains(ammo.mobId))
        return;
    auto &mobTransform = transforms.get(ammo.mobId);
    auto &mobSprite = sprites.get(ammo.mobId);

    for (auto &bullet : bullets) {
        bullet.transform.scale.x *= ammo.scaleFactor;
        bullet.transform.scale.y *= ammo.scaleFactor;

        bullet.transform.pos.x = mobTransform.pos.x + (mobSprite.src.width * mobTransform.scale.x) / 2.0f -
                                 (bullet.sprite.src.width * bullet.transform.scale.x) / 2.0f +
                                 WINDOW_WIDTH * bullet.transform.pos.x;
        bullet.transform.pos.y = mobTransform.pos.y + (mobSprite.src.height * mobTransform.scale.y) / 2.0f -
                                 (bullet.sprite.src.height * bullet.transform.scale.y) / 2.0f +
                                 WINDOW_HEIGHT * bullet.transform.pos.y;

        if (bullet.followPlayer)
            determineVelocity(mobTransform, mobSprite, bullet.velocity);
        bullet.velocity.x *= ammo.speedFactor;
        bullet.velocity.y *= ammo.speedFactor;

        gengine::Entity entity =
            spawnEntity(component::BulletEnemy(), bullet.transform, bullet.velocity, bullet.sprite,
                        geg::component::io::Drawable(1), bullet.hitbox, geg::component::network::NetSend());

        if (!bullet.animation.trackName.empty())
            setComponent(entity, bullet.animation);

        if (bullet.isDestroyable)
            setComponent(entity, component::Monster());
    }
}

void BulletManager::spawn(event::EnemyShootEvent &e) {
    auto bullets = get(e.bulletName.c_str());
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    if (!transforms.contains(e.from) || !sprites.contains(e.from))
        return;
    auto &mobTransform = transforms.get(e.from);
    auto &mobSprite = sprites.get(e.from);

    for (auto &bullet : bullets) {
        bullet.transform.pos.x = mobTransform.pos.x + (mobSprite.src.width * mobTransform.scale.x) / 2.0f -
                                 (bullet.sprite.src.width * bullet.transform.scale.x) / 2.0f +
                                 WINDOW_WIDTH * bullet.transform.pos.x;
        bullet.transform.pos.y = mobTransform.pos.y + (mobSprite.src.height * mobTransform.scale.y) / 2.0f -
                                 (bullet.sprite.src.height * bullet.transform.scale.y) / 2.0f +
                                 WINDOW_HEIGHT * bullet.transform.pos.y;

        if (bullet.followPlayer)
            determineVelocity(mobTransform, mobSprite, bullet.velocity);
        bullet.velocity.x *= e.bulletSpeed;
        bullet.velocity.y *= e.bulletSpeed;

        gengine::Entity entity =
            spawnEntity(component::BulletEnemy(), bullet.transform, bullet.velocity, bullet.sprite,
                        geg::component::io::Drawable(1), bullet.hitbox, geg::component::network::NetSend());

        if (!bullet.animation.trackName.empty())
            setComponent(entity, bullet.animation);

        if (bullet.isDestroyable)
            setComponent(entity, component::Monster());
    }
}

void BulletManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (!entry.is_regular_file())
            continue;

        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if (!file.is_open())
            continue;

        nlohmann::json jsonData = nlohmann::json::parse(file);

        std::string mobName = entry.path().filename().string();
        m_bullets[mobName] = jsonData.get<std::vector<Bullet>>();
    }
}

const std::vector<Bullet> &BulletManager::get(const std::string &bulletName) const {
    return m_bullets.at(bulletName);
}
} // namespace rtype::system
