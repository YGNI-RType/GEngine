/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MobManager.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "Types.hpp"
#include "components/Bounding.hpp"
#include "components/Flapping.hpp"
#include "components/Monster.hpp"
#include "components/Score.hpp"
#include "events/BossSpawnWave.hpp"

#include <unordered_map>

namespace rtype::system {
class MobManager
    : public gengine::System<MobManager, component::Monster, component::Score, component::Bounding, component::Flapping,
                             geg::component::io::Sprite, geg::component::io::Animation, geg::component::io::Drawable,
                             geg::component::HitBoxSquare2D, geg::component::Transform2D, geg::component::Velocity2D,
                             geg::component::network::NetSend>,
      public gengine::RemoteSystem {
public:
    MobManager(const std::string &);
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &);

    void setMotionComponent(gengine::Entity entity, TypeOfMotion type);
    void spawn(const Monster &, std::vector<Ammo> &);
    void spawn(event::BossSpawnWave &);
    const std::vector<Mob> &get(const std::string &) const;

private:
    std::string m_folder;
    std::unordered_map<std::string, std::vector<Mob>> m_mobs;
};
} // namespace rtype::system
