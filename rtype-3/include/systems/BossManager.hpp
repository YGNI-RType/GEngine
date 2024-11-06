/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BossManager.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "Types.hpp"
#include "components/Boss.hpp"
#include "components/Monster.hpp"
#include "components/Score.hpp"

#include <unordered_map>

namespace rtype::system {
class BossManager
    : public gengine::System<BossManager, component::Boss, component::Monster, component::Score,
                             geg::component::io::Sprite, geg::component::io::Animation, geg::component::io::Drawable,
                             geg::component::HitBoxSquare2D, geg::component::Transform2D, geg::component::Velocity2D,
                             geg::component::network::NetSend>,
      public gengine::RemoteSystem {
public:
    BossManager(const std::string &);
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &);
    void onGameLoop(gengine::system::event::GameLoop &);

    void spawn(const Monster &);
    const Boss &get(const std::string &) const;

private:
    std::string m_folder;
    std::unordered_map<std::string, Boss> m_bosses;
};
} // namespace rtype::system
