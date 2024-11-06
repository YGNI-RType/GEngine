/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.cpp
*/

#include "systems/PlayerMotion.hpp"

#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"

#include <raymath.h>

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include "Constants.hpp"
#include <random>

namespace poc3d::system {
void PlayerMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerMotion::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Movement>>(&PlayerMotion::movePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Rotation>>(&PlayerMotion::rotatePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Jump>>(&PlayerMotion::jumpPlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::ChangeCameraMode>>(
        &PlayerMotion::changeCameraModePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Sprint>>(&PlayerMotion::sprintPlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::LockPlayerEvent>>(&PlayerMotion::lockPlayer);
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
    auto &anims = getComponents<geg::component::io::Animation>();

    for (auto [entity, remote, player, velocity, transform] : gengine::Zip(remotes, players, velocities, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        float yawRadians = (transform.rotation.y + 90) * 3.14f / 180.f;
        gengine::Vect3 forward = {cos(yawRadians), 0, -sin(yawRadians)};
        gengine::Vect3 right = {sin(yawRadians), 0, cos(yawRadians)};

        if (e->state != event::Movement::STANDING && anims.contains(entity)) {
            if (player.sprinting && anims.get(entity).trackName != "player.json/run")
                setComponent(entity, geg::component::io::Animation("player.json/run", 0.03f));
            if (!player.sprinting && anims.get(entity).trackName != "player.json/walk")
                setComponent(entity, geg::component::io::Animation("player.json/walk", 0.03f));
        }

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
            if (anims.contains(entity) && anims.get(entity).trackName != "player.json/idle")
                setComponent(entity, geg::component::io::Animation("player.json/idle", 0.03f));
            break;
        }
        }
    }
}

void PlayerMotion::rotatePlayer(gengine::interface::event::SharedEvent<event::Rotation> &e) {
    auto &players = getComponents<component::Player>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, transform] : gengine::Zip(remotes, players, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID || player.isLocked) // check if its the same remote (zip)
            continue;

        transform.rotation.y += e->move.y;
    }
}

void PlayerMotion::jumpPlayer(gengine::interface::event::SharedEvent<event::Jump> &e) {
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &velocities = getComponents<gengine::component::Velocity3D>();
    auto &accelerations = getComponents<geg::component::Acceleration3D>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, velocity, transform] : gengine::Zip(remotes, players, velocities, transforms)) {
        if (accelerations.contains(entity) ||
            (remote.getUUIDBytes() != e.remoteUUID && remote.getUUIDBytes() != e->playerUuid) ||
            (!e->playerUuid.is_nil() && remote.getUUIDBytes() == e.remoteUUID)) // check if its the same remote (zip)
            continue;
        velocity.y = e->strength;
        setComponent(entity, geg::component::Acceleration3D(0, -0.009, 0));
    }
}

void PlayerMotion::changeCameraModePlayer(gengine::interface::event::SharedEvent<event::ChangeCameraMode> &e) {
    if (!hasSystem<gengine::system::driver::output::DrawModel>())
        return;
    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    if (draw.cameraMode == CAMERA_FIRST_PERSON)
        draw.cameraMode = CAMERA_THIRD_PERSON;
    else if (draw.cameraMode == CAMERA_THIRD_PERSON)
        draw.cameraMode = CAMERA_CUSTOM;
    else if (draw.cameraMode == CAMERA_CUSTOM)
        draw.cameraMode = CAMERA_FREE;
    else
        draw.cameraMode = CAMERA_FIRST_PERSON;
}

void PlayerMotion::sprintPlayer(gengine::interface::event::SharedEvent<event::Sprint> &e) {
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player] : gengine::Zip(remotes, players)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        player.sprinting = e->sprinting;
        if (e->sprinting)
            player.speed = player.defaultSpeed * 2;
        else
            player.speed = player.defaultSpeed;
    }
}

void PlayerMotion::lockPlayer(gengine::interface::event::SharedEvent<event::LockPlayerEvent> &e) {
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    for (auto [entity, remote, player] : gengine::Zip(remotes, players)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        player.isLocked = e->locked;
    }
}
} // namespace poc3d::system
