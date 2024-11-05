
/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ResetPlayerRotation.cpp
*/

#include "systems/ResetPlayerRotation.hpp"

#include <raymath.h>

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include "Constants.hpp"
#include <random>

#include "GEngine/libdev/tools/Raylib.hpp"

namespace poc3d::system {
void ResetPlayerRotation::init(void) {
    subscribeToEvent<gengine::interface::event::SharedEvent<event::ResetPlayerRotationEvent>>(
        &ResetPlayerRotation::resetPlayerRotation);
}

void ResetPlayerRotation::resetPlayerRotation(
    gengine::interface::event::SharedEvent<event::ResetPlayerRotationEvent> &e) {
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, transform] : gengine::Zip(remotes, players, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        transform.rotation.y = e->rotation;
    }
}
} // namespace poc3d::system
