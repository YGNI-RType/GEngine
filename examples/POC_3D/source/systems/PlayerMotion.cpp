/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.cpp
*/

#include "systems/PlayerMotion.hpp"

#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include <random>

namespace poc3d::system {
void PlayerMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerMotion::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Movement>>(&PlayerMotion::movePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Rotation>>(&PlayerMotion::rotatePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Jump>>(&PlayerMotion::jumpPlayer);
}

void PlayerMotion::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &velocities = getComponents<gengine::component::Velocity3D>();
    auto &players = getComponents<component::Player>();

    for (auto [entity, transform, player] : gengine::Zip(transforms, players)) {
        if (transform.pos.y <= 0) {
            transform.pos.y = 0;
            if (velocities.contains(entity))
                velocities.get(entity).y = 0;
            unsetComponent<geg::component::Acceleration3D>(entity);
        }
    }
}

void PlayerMotion::movePlayer(gengine::interface::event::SharedEvent<event::Movement> &e) {
    auto &velocities = getComponents<gengine::component::Velocity3D>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    auto &models = getComponents<gengine::component::driver::output::Model>();

    for (auto [entity, remote, player, velocity, transform] : gengine::Zip(remotes, players, velocities, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        float yawRadians = (transform.rotation.y + 90) * 3.14f / 180.f;
        gengine::Vect3 forward = {cos(yawRadians), 0, -sin(yawRadians)};
        gengine::Vect3 right = {sin(yawRadians), 0, cos(yawRadians)};

        // Handle movement input
        switch (e->state) {
        case event::Movement::LEFT: {
            velocity.x = -player.speed * right.x;
            velocity.z = -player.speed * right.z;
            break;
        }
        case event::Movement::RIGHT: {
            velocity.x = player.speed * right.x;
            velocity.z = player.speed * right.z;
            break;
        }
        case event::Movement::FWD: {
            velocity.x = player.speed * forward.x;
            velocity.z = player.speed * forward.z;
            break;
        }
        case event::Movement::BCK: {
            velocity.x = -player.speed * forward.x;
            velocity.z = -player.speed * forward.z;
            break;
        }
        case event::Movement::FWD_RIGHT: {
            velocity.x = player.speed * (forward.x + right.x) / sqrt(2.f);
            velocity.z = player.speed * (forward.z + right.z) / sqrt(2.f);
            break;
        }
        case event::Movement::FWD_LEFT: {
            velocity.x = player.speed * (forward.x - right.x) / sqrt(2.f);
            velocity.z = player.speed * (forward.z - right.z) / sqrt(2.f);
            break;
        }
        case event::Movement::BCK_RIGHT: {
            velocity.x = -player.speed * (forward.x - right.x) / sqrt(2.f);
            velocity.z = -player.speed * (forward.z - right.z) / sqrt(2.f);
            break;
        }
        case event::Movement::BCK_LEFT: {
            velocity.x = -player.speed * (forward.x + right.x) / sqrt(2.f);
            velocity.z = -player.speed * (forward.z + right.z) / sqrt(2.f);
            break;
        }
        case event::Movement::STANDING: {
            velocity.x = 0;
            velocity.z = 0;
            break;
        }
        }

        moveCamera(transform, forward);
    }
}

void PlayerMotion::rotatePlayer(gengine::interface::event::SharedEvent<event::Rotation> &e) {
    auto &players = getComponents<component::Player>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, transform] : gengine::Zip(remotes, players, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        transform.rotation.x += e->move.x;
        transform.rotation.y += e->move.y;
        transform.rotation.z += e->move.z;
    }
}

void PlayerMotion::jumpPlayer(gengine::interface::event::SharedEvent<event::Jump> &e) {
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &velocities = getComponents<gengine::component::Velocity3D>();
    auto &accelerations = getComponents<geg::component::Acceleration3D>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, velocity, transform] : gengine::Zip(remotes, players, velocities, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID ||
            accelerations.contains(entity)) // check if its the same remote (zip)
            continue;
        velocity.y = e->strength;
        setComponent(entity, geg::component::Acceleration3D(0, -0.009, 0));
    }
}

void PlayerMotion::moveCamera(geg::component::Transform3D &transform, gengine::Vect3 &forward) {
    if (hasSystem<gengine::system::driver::output::DrawModel>()) {
        auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
        draw.camera.position.x = transform.pos.x - forward.x * 2;
        draw.camera.position.z = transform.pos.z - forward.z * 2;
        draw.camera.target.x = transform.pos.x + forward.x * 3;
        draw.camera.target.z = transform.pos.z + forward.z * 3;
    }
}
} // namespace poc3d::system
