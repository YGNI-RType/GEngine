/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BossManager.cpp
*/

#include "systems/BossManager.hpp"
#include "Constants.hpp"
#include "events/BossSpawnWave.hpp"
#include "events/EnemyShootEvent.hpp"

#include <filesystem>
#include <fstream>
#include <random>

namespace rtype::system {
BossManager::BossManager(const std::string &folder)
    : m_folder(folder) {
}

void BossManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&BossManager::onStartEngine);
    subscribeToEvent<gengine::system::event::GameLoop>(&BossManager::onGameLoop);
}

void BossManager::onGameLoop(gengine::system::event::GameLoop &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto &bosses = getComponents<component::Boss>();
    auto &motions = getComponents<geg::component::Velocity2D>();
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &netsends = getComponents<geg::component::network::NetSend>();

    for (auto [entity, motion, transform, sprite, bossComp, netsend] :
         gengine::Zip(motions, transforms, sprites, bosses, netsends)) {
        const auto &boss = get(bossComp.bossName.c_str());

        std::uniform_int_distribution<> randomIndexMob(0, boss.wavesName.size() - 1);
        bossComp.waveSpawnInSec -= e.deltaTime;
        if (bossComp.waveSpawnInSec < 0) {
            bossComp.waveSpawnInSec = boss.waveCooldown;
            publishEvent(event::BossSpawnWave(boss.wavesName[randomIndexMob(gen)]));
        }

        std::uniform_int_distribution<> speed(boss.minVelocity, boss.maxVelocity);
        std::uniform_int_distribution<> randomIndexBullet(0, boss.ammoName.size() - 1);
        if (transform.pos.y < boss.borderMargin * WINDOW_HEIGHT - (sprite.src.height * transform.scale.y) / 2.0f) {
            motion.y = speed(gen);
            netsend.update();
            publishEvent(event::EnemyShootEvent(entity, boss.ammoName[randomIndexBullet(gen)], boss.bulletSpeed));
        }
        if (transform.pos.y >
            WINDOW_HEIGHT - boss.borderMargin * WINDOW_HEIGHT - (sprite.src.height * transform.scale.y) / 2.0f) {
            motion.y = -speed(gen);
            netsend.update();
            publishEvent(event::EnemyShootEvent(entity, boss.ammoName[randomIndexBullet(gen)], boss.bulletSpeed));
        }
    }
}

void BossManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (!entry.is_regular_file())
            continue;

        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if (!file.is_open())
            continue;

        nlohmann::json jsonData = nlohmann::json::parse(file);

        std::string bossName = entry.path().filename().string();
        m_bosses[bossName] = jsonData.get<Boss>();
    }
}

void BossManager::spawn(const Monster &monster) {
    auto boss = get(monster.mobName);

    boss.transform.scale.x *= monster.scaleFactor;
    boss.transform.scale.y *= monster.scaleFactor;
    boss.transform.pos.x = WINDOW_WIDTH * monster.spawnFactor.x -
                           (boss.sprite.src.width * boss.transform.scale.x) / 2.0f +
                           WINDOW_WIDTH * boss.transform.pos.x;
    boss.transform.pos.y = WINDOW_HEIGHT * monster.spawnFactor.y -
                           (boss.sprite.src.height * boss.transform.scale.y) / 2.0f +
                           WINDOW_HEIGHT * boss.transform.pos.y;
    boss.velocity.x *= monster.speedFactor;
    boss.velocity.y *= monster.speedFactor;

    gengine::Entity entity =
        spawnEntity(component::Boss(monster.mobName), component::Monster(monster.numberOfLifes), boss.transform,
                    boss.velocity, boss.sprite, geg::component::io::Drawable(1), boss.hitbox,
                    geg::component::network::NetSend(), component::Score(monster.scoreGain));

    if (!boss.animation.trackName.empty())
        setComponent(entity, boss.animation);
}

const Boss &BossManager::get(const std::string &bossName) const {
    return m_bosses.at(bossName);
}
} // namespace rtype::system
