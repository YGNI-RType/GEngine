/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** CameraRotation.cpp
*/

#include "systems/CameraRotation.hpp"

#include <raymath.h>

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include "events/ResetPlayerRotationEvent.hpp"

#include "GEngine/libdev/tools/Raylib.hpp"

namespace poc3d::system {
void CameraRotation::init(void) {
    subscribeToEvent<event::Rotation>(&CameraRotation::rotateCamera);
    subscribeToEvent<gengine::system::event::StartEngine>(&CameraRotation::onStart);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Movement>>(&CameraRotation::onMovement);
    subscribeToEvent<event::ResetPlayerRotationCameraEvent>(&CameraRotation::resetPlayerRotation);
}

void CameraRotation::onStart(gengine::system::event::StartEngine &e) {
    m_cameraId = spawnEntity(geg::component::Transform3D({0.0f, 0.0f, 0.0f}, {1, 1, 1}, {-90.f, 0.f, 90.f}));
}

void CameraRotation::rotateCamera(event::Rotation &e) {
    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    auto &transforms = getComponents<geg::component::Transform3D>();

    if (transforms.contains(m_cameraId)) {
        auto &camera = transforms.get(m_cameraId);
        camera.rotation.y += e.move.y;
    }
    if (draw.cameraMode != CAMERA_FREE)
        gengine::CameraPitch(
            &draw.camera, e.move.z / 50.f * MOUSE_SENSITIVITY, true, false,
            // draw.cameraMode == CAMERA_FIRST_PERSON ? false : true, // TODO rotateAroundTarget: Fun to play with
            false);
}

void CameraRotation::onMovement(gengine::interface::event::SharedEvent<event::Movement> &e) {
    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, transform] : gengine::Zip(remotes, players, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        if (transforms.contains(m_cameraId)) {
            auto &camera = transforms.get(m_cameraId);

            float yawRadians = (camera.rotation.y + 90) * 3.14f / 180.f;
            gengine::Vect3 forward = {cos(yawRadians), 0, -sin(yawRadians)};
            moveCamera(transform, forward);
        }
    }
}

void CameraRotation::moveCamera(geg::component::Transform3D &transform, gengine::Vect3 &forward) {
    static float lastY = 0;
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

void CameraRotation::resetPlayerRotation(event::ResetPlayerRotationCameraEvent &e) {
    auto &transforms = getComponents<geg::component::Transform3D>();
    if (transforms.contains(m_cameraId)) {
        auto &camera = transforms.get(m_cameraId);
        publishEvent(event::ResetPlayerRotationEvent(camera.rotation.y));
    }
}
} // namespace poc3d::system
