/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MorphToProp.cpp
*/

#include "systems/MorphToProp.hpp"

#include <raylib.h>
#include <raymath.h>

#include "GEngine/libdev/Component.hpp"    // gengine::Zip
#include "GEngine/libdev/tools/Raylib.hpp" // setModelTransform

#include "Constants.hpp"

#include "events/ChangePlayerModelEvent.hpp"

namespace poc3d::system {
void MorphToProp::init(void) {
    subscribeToEvent<gengine::interface::event::SharedEvent<event::MorphToPropEvent>>(&MorphToProp::morphPlayer);
}

void MorphToProp::morphPlayer(gengine::interface::event::SharedEvent<event::MorphToPropEvent> &e) {
    if (!hasSystem<gengine::system::driver::output::DrawModel>())
        return;

    auto &models = getComponents<gengine::component::driver::output::Model>();
    auto &players = getComponents<component::Player>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();

    auto &modelMan = getSystem<gengine::system::driver::output::ModelManager>();

    gengine::Entity playerEntity = 0;

    for (auto [entity, player, remote, transform] : gengine::Zip(players, remotes, transforms)) {
        if (remote.getUUIDBytes() == e.remoteUUID) { // check if its the same remote (zip)
            playerEntity = entity;
            break;
        }
    }

    auto &props = getComponents<component::Prop>();
    RayCollision collision = {0};

    for (auto [entity, prop, model, transform] : gengine::Zip(props, models, transforms)) {
        collision = gengine::GetMouseRayCollisionModel(modelMan.get(model.txtPath), transform, draw.camera);
        if (collision.hit)
            publishEvent(event::ChangePlayerModelEvent(model.txtPath));
    }
}
} // namespace poc3d::system
