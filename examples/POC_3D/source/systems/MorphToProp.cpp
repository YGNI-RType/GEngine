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
#include "events/ResetPlayerRotationCameraEvent.hpp"

namespace poc3d::system {
void MorphToProp::init(void) {
    subscribeToEvent<event::MorphToPropEvent>(&MorphToProp::morphPlayer);
}

void MorphToProp::morphPlayer(event::MorphToPropEvent &e) {
    auto &modelMan = getSystem<gengine::system::driver::output::ModelManager>();
    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    auto &wd = getSystem<gengine::system::driver::output::RenderWindow>();

    auto &models = getComponents<gengine::component::driver::output::Model>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &props = getComponents<component::Prop>();

    Vector2 center = {wd.getWidth() / 2.f, wd.getHeight() / 2.f};
    Ray ray = GetMouseRay(center, draw.camera);
    RayCollision collision = {0};
    for (auto [entity, prop, model, transform] : gengine::Zip(props, models, transforms)) {
        collision = gengine::GetRayCollisionModel(ray, modelMan.get(model.txtPath), transform);
        if (collision.hit) {
            publishEvent(event::ChangePlayerModelEvent(model.txtPath, transform));
            publishEvent(event::ResetPlayerRotationCameraEvent());
        }
    }
}
} // namespace poc3d::system
