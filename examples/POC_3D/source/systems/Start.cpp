/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "systems/Start.hpp"

#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/Velocities.hpp"

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"
#include "GEngine/libdev/components/driver/output/Text.hpp"

#include "components/Background.hpp"
#include "components/Player.hpp"

#include <random>

namespace rtype {
void system::Start::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&Start::onNewRemoteLocal);
    subscribeToEvent<gengine::interface::event::DeleteRemoteLocal>(&Start::onDeleteRemoteLocal);
}

void system::Start::onStartEngine(gengine::system::event::StartEngine &e) {
    spawnEntity(component::Background(), gengine::component::Transform2D({0, 0}, {3.48, 3.48}),
                gengine::component::Velocity2D(-DEFAULT_BACKGROUND_SPEED, 0),
                gengine::component::driver::output::Drawable(0),
                gengine::component::driver::output::Sprite("background.png", Rectangle{0, 0, 1120, 207}, WHITE), geg::component::network::NetSend());

    spawnEntity(component::Background(), gengine::component::Transform2D({1119 * 3.48, 0}, {3.48, 3.48}),
                gengine::component::Velocity2D(-DEFAULT_BACKGROUND_SPEED, 0),
                gengine::component::driver::output::Drawable(0),
                gengine::component::driver::output::Sprite("background.png", Rectangle{0, 0, 1120, 207}, WHITE), geg::component::network::NetSend());

    spawnEntity(gengine::component::driver::output::Model("gmApart.glb"),
                gengine::component::driver::output::Drawable(1), gengine::component::Transform3D({0.0f, 2.0f, 4.0f}), geg::component::network::NetSend());
}

void system::Start::onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &e) {
    spawnEntity(component::Player(), gengine::component::Transform3D({0.0f, 2.0f, 4.0f}),
                gengine::component::Velocity3D(0, 0, 0), gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Model("coraline.glb"),
                gengine::interface::component::RemoteLocal(e.uuid), geg::component::network::NetSend());
    m_nbPlayer++;
}

void system::Start::onDeleteRemoteLocal(gengine::interface::event::DeleteRemoteLocal &e) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    for (auto &[entity, remote] : remotes) {
        if (remote.getUUIDBytes() == e.uuid) {
            killEntity(entity);
            m_nbPlayer--;
            return;
        }
    }
}
} // namespace rtype
