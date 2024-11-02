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

// TODO try to get Raylib code with rcamera.h
static void CameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
    // Up direction
    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);
    Vector3 up = Vector3Normalize(camera->up);
    Vector3 forward = Vector3Normalize(targetPosition);
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, up));

    if (lockView) {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp)
            angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f;  // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown)
            angle = maxAngleDown;
    }

    // Rotate view vector around right axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget) {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    } else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp) {
        // Rotate up direction around right axis
        camera->up = Vector3RotateByAxisAngle(camera->up, right, angle);
    }
}

namespace poc3d::system {
void PlayerMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerMotion::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Movement>>(&PlayerMotion::movePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Rotation>>(&PlayerMotion::rotatePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Jump>>(&PlayerMotion::jumpPlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::ChangeCameraMode>>(
        &PlayerMotion::changeCameraModePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Sprint>>(&PlayerMotion::sprintPlayer);
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

        transform.rotation.y += e->move.y;
        if (hasSystem<gengine::system::driver::output::DrawModel>()) {
            auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
            // UpdateCameraPro(&draw.camera, {},
            //                 {0, draw.cameraMode == CAMERA_FREE ? 0 : -e->move.z * 3 * MOUSE_SENSITIVITY, 0}, 0);
            if (draw.cameraMode != CAMERA_FREE)
                CameraPitch(&draw.camera, e->move.z / 50.f * MOUSE_SENSITIVITY, true, false,
                            // draw.cameraMode == CAMERA_FIRST_PERSON ? false : true, // TODO Fun to play with
                            false);
        }
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
    static float lastY = 0;
    if (!hasSystem<gengine::system::driver::output::DrawModel>())
        return;
    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    switch (draw.cameraMode) {
    case CAMERA_FIRST_PERSON: {
        draw.camera.position.y = 1.4; // TODO: make this a variable (height of the model)
        draw.camera.position.x = transform.pos.x + forward.x / 4;
        draw.camera.position.z = transform.pos.z + forward.z / 4;
        draw.camera.target.x = transform.pos.x + forward.x;
        draw.camera.target.z = transform.pos.z + forward.z;
        break;
    }
    case CAMERA_THIRD_PERSON: {
        draw.camera.position.y = 1.7;
        draw.camera.position.x = transform.pos.x - forward.x * 1;
        draw.camera.position.z = transform.pos.z - forward.z * 1;
        draw.camera.target.x = transform.pos.x + forward.x * 2;
        draw.camera.target.z = transform.pos.z + forward.z * 2;
        break;
    }
    case CAMERA_CUSTOM: {
        draw.camera.position.y = 1.7;
        draw.camera.position.x = transform.pos.x + forward.x * 2;
        draw.camera.position.z = transform.pos.z + forward.z * 2;
        draw.camera.target.x = transform.pos.x;
        draw.camera.target.z = transform.pos.z;
        break;
    }
    default:
        break;
    }
    // Jump
    if (draw.cameraMode != CAMERA_FREE) {
        draw.camera.position.y += transform.pos.y;
        draw.camera.target.y += transform.pos.y;
        draw.camera.target.y -= lastY;
        lastY = transform.pos.y;
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
        if (e->sprinting)
            player.speed = player.defaultSpeed * 2;
        else
            player.speed = player.defaultSpeed;
    }
}

} // namespace poc3d::system
