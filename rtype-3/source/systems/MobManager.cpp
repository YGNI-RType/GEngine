/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MobManager.cpp
*/

#include "systems/MobManager.hpp"
#include "Constants.hpp"

#include <filesystem>
#include <fstream>
#include <random>

namespace rtype::system {
MobManager::MobManager(const std::string &folder)
    : m_folder(folder) {
}

void MobManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&MobManager::onStartEngine);
    subscribeToEvent<event::BossSpawnWave>(&MobManager::spawn);
}

void MobManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (!entry.is_regular_file())
            continue;

        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if (!file.is_open())
            continue;

        nlohmann::json jsonData = nlohmann::json::parse(file);

        std::string mobName = entry.path().filename().string();
        m_mobs[mobName] = jsonData.get<std::vector<Mob>>();
    }
}

void MobManager::setMotionComponent(gengine::Entity entity, TypeOfMotion type) {
    switch (type) {
    case TypeOfMotion::FLAPPING:
        setComponent(entity, component::Flapping());
        break;
    case TypeOfMotion::BOUNDING:
        setComponent(entity, component::Bounding());
        break;
    default:
        break;
    }
}

void MobManager::spawn(const Monster &monster, std::vector<Ammo> &ammo) {
    auto mobs = get(monster.mobName);

    for (auto &mob : mobs) {
        mob.transform.scale.x *= monster.scaleFactor;
        mob.transform.scale.y *= monster.scaleFactor;
        mob.transform.pos.x = WINDOW_WIDTH * monster.spawnFactor.x -
                              (mob.sprite.src.width * mob.transform.scale.x) / 2.0f +
                              WINDOW_WIDTH * mob.transform.pos.x;
        mob.transform.pos.y = WINDOW_HEIGHT * monster.spawnFactor.y -
                              (mob.sprite.src.height * mob.transform.scale.y) / 2.0f +
                              WINDOW_HEIGHT * mob.transform.pos.y;
        mob.velocity.x *= monster.speedFactor;
        mob.velocity.y *= monster.speedFactor;

        gengine::Entity entity = spawnEntity(component::Monster(monster.numberOfLifes), mob.transform, mob.velocity,
                                             mob.sprite, geg::component::io::Drawable(1), mob.hitbox,
                                             geg::component::network::NetSend(), component::Score(monster.scoreGain));

        if (!mob.animation.trackName.empty())
            setComponent(entity, mob.animation);

        setMotionComponent(entity, mob.typeOfMotion);

        for (auto &a : mob.ammo) {
            a.spawnDelay += monster.spawnDelay;
            a.mobId = entity;
            ammo.push_back(a);
        }
    }
}

void MobManager::spawn(event::BossSpawnWave &e) {
    auto mobs = get(e.waveName.c_str());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomY(30, 70);
    float y = randomY(gen) / 100.0f;

    for (auto &mob : mobs) {
        mob.transform.pos.x = WINDOW_WIDTH * 1.05f - (mob.sprite.src.width * mob.transform.scale.x) / 2.0f +
                              WINDOW_WIDTH * mob.transform.pos.x;
        mob.transform.pos.y = WINDOW_HEIGHT * y - (mob.sprite.src.height * mob.transform.scale.y) / 2.0f +
                              WINDOW_HEIGHT * mob.transform.pos.y;

        gengine::Entity entity =
            spawnEntity(component::Monster(), mob.transform, mob.velocity, mob.sprite, geg::component::io::Drawable(1),
                        mob.hitbox, geg::component::network::NetSend());

        if (!mob.animation.trackName.empty())
            setComponent(entity, mob.animation);

        setMotionComponent(entity, mob.typeOfMotion);
    }
}

const std::vector<Mob> &MobManager::get(const std::string &mobName) const {
    return m_mobs.at(mobName);
}
} // namespace rtype::system
