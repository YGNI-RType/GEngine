/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Guess.cpp
*/

#include "systems/Guess.hpp"

#include <raylib.h>
#include <raymath.h>

#include "GEngine/libdev/Component.hpp"    // gengine::Zip
#include "GEngine/libdev/tools/Raylib.hpp" // setModelTransform

#include "Constants.hpp"

#include "events/Jump.hpp"

namespace poc3d::system {
void Guess::init(void) {
    subscribeToEvent<gengine::interface::event::SharedEvent<event::GuessEvent>>(&Guess::guessWho);
}

void Guess::guessWho(gengine::interface::event::SharedEvent<event::GuessEvent> &e) {
    if (!hasSystem<gengine::system::driver::output::DrawModel>())
        return;

    auto &models = getComponents<gengine::component::driver::output::Model>();
    auto &players = getComponents<component::Player>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    RayCollision collision = {0};

    auto &modelMan = getSystem<gengine::system::driver::output::ModelManager>();

    for (auto [entity, model, player, remote, transform] : gengine::Zip(models, players, remotes, transforms)) {
        if (remote.getUUIDBytes() == e.remoteUUID) // check if its the same remote (zip)
            continue;
        collision = gengine::GetRayCollisionModel({WINDOW_WIDTH / 2, (WINDOW_TOTAL_HEIGHT) / 2},
                                                  modelMan.get(model.txtPath), transform, draw.camera);
        if (collision.hit)
            publishEvent(event::Jump(0.4, remote.getUUIDBytes()));

        // publishEvent(event::PlayerHit(entity, remote.getUUIDBytes())); //TODO Implement hit event
    }
}
} // namespace poc3d::system
