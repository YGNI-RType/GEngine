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
#include "components/Prop.hpp"

#include "Constants.hpp"

#include <random>

namespace poc3d {
void system::Start::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&Start::onNewRemoteLocal);
    subscribeToEvent<gengine::interface::event::DeleteRemoteLocal>(&Start::onDeleteRemoteLocal);
}

void system::Start::onStartEngine(gengine::system::event::StartEngine &e) {
    // Background
    spawnEntity(component::Background(), gengine::component::Transform2D({0, 0}, {3.48, 3.48}),
                gengine::component::Velocity2D(-DEFAULT_BACKGROUND_SPEED, 0),
                gengine::component::driver::output::Drawable(0),
                gengine::component::driver::output::Sprite("background.png", Rectangle{0, 0, 1120, 207}, WHITE),
                geg::component::network::NetSend());
    spawnEntity(component::Background(), gengine::component::Transform2D({1119 * 3.48, 0}, {3.48, 3.48}),
                gengine::component::Velocity2D(-DEFAULT_BACKGROUND_SPEED, 0),
                gengine::component::driver::output::Drawable(0),
                gengine::component::driver::output::Sprite("background.png", Rectangle{0, 0, 1120, 207}, WHITE),
                geg::component::network::NetSend());

    // 3D Scene
    spawnEntity(gengine::component::driver::output::Model("gmApart.glb"),
                gengine::component::driver::output::Drawable(1),
                gengine::component::Transform3D({0.0f, -0.1f, 0.0f}, {1, 1, 1}, {-90.f, 0.f, 90.f}),
                geg::component::network::NetSend());

    // Props
    float scale = 1;
    spawnEntity(component::Prop(), geg::component::Transform3D({-2.0f, 0.0f, 0.0f}, {scale, scale, scale}, {0, 0, 0}),
                gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Model("props/antique_cabinet.glb"),
                geg::component::network::NetSend());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomIndex(0, propsList.size() - 1);
    std::uniform_real_distribution<float> randomX(-1.0f, 11.0f);
    std::uniform_real_distribution<float> randomZ(-5.0f, 7.0f);
    std::uniform_real_distribution<float> randomRotation(0.f, 360.f);

    for (int i = 0; i < PROPS_COUNT; i++) {
        size_t newIndex = randomIndex(gen);

        scale = propsList[newIndex].second;
        spawnEntity(component::Prop(),
                    geg::component::Transform3D({randomX(gen), 0.0f, randomZ(gen)}, {scale, scale, scale},
                                                {0, randomRotation(gen), 0}),
                    gengine::component::driver::output::Drawable(2),
                    gengine::component::driver::output::Model("props/" + propsList[newIndex].first),
                    geg::component::network::NetSend());
    }

    // Car
    scale = 0.03;
    spawnEntity(component::Prop(), geg::component::Transform3D({-2.5f, 0.0f, 2.0f}, {scale, scale, scale}, {90, 0, 56}),
                gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Model("props/voiture_cassee.glb"),
                geg::component::network::NetSend());

    // Surprise
    scale = 0.6;
    spawnEntity(
        component::Prop(), geg::component::Transform3D({1.38f, 0.0f, -5.5f}, {scale, scale, scale}, {-90, 0, 0}),
        gengine::component::driver::output::Drawable(2),
        gengine::component::driver::output::Model("props/skibidi_toilet.glb"), geg::component::network::NetSend());

    // Chair
    scale = 0.02;
    spawnEntity(component::Prop(), geg::component::Transform3D({1.38f, 0.0f, -4.0f}, {scale, scale, scale}, {90, 0, 0}),
                gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Model("props/chaise.glb"), geg::component::network::NetSend());

    publishEvent(gengine::system::event::driver::output::Music("ambiance.mp3"));
}

void system::Start::onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &e) {
    float scale = 0.5;
    spawnEntity(component::Player(),
                gengine::component::Transform3D({2.0f, 0.0f, 0.0f}, {scale, scale, scale},
                                                {0, 0, 0}), // coraline_old {-90, 0, 180}
                gengine::component::Velocity3D(0, 0, 0), gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Model("player/", WHITE, true),
                geg::component::io::Animation("player.json/walk", 0.03f),
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
} // namespace poc3d
