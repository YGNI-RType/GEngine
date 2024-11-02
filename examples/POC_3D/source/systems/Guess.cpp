/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Guess.cpp
*/

#include "systems/Guess.hpp"

#include <raylib.h>
#include <raymath.h>

#include "GEngine/libdev/Component.hpp" // gengine::Zip

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

    Ray ray = {0};
    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    RayCollision collision = {0};
    BoundingBox box = {0};

    auto &modelMan = getSystem<gengine::system::driver::output::ModelManager>();

    std::cout << "Guessing" << std::endl;

    for (auto [entity, model, player, remote, transform] : gengine::Zip(models, players, remotes, transforms)) {
        if (remote.getUUIDBytes() == e.remoteUUID) // check if its the same remote (zip)
            continue;
        Model modelRaylib = modelMan.get(model.txtPath);

        box = GetMeshBoundingBox(modelRaylib.meshes[0]);
        box.min.x += transform.pos.x;
        box.min.y += transform.pos.y;
        box.min.z += transform.pos.z;
        box.max.x += transform.pos.x;
        box.max.y += transform.pos.y;
        box.max.z += transform.pos.z;

        ray = GetMouseRay(GetMousePosition(), draw.camera);
        collision = GetRayCollisionBox(ray, box);
        if (collision.hit) {
            // publishEvent(event::PlayerHit(entity, remote.getUUIDBytes())); //TODO Implement hit event
            // player.speed = 0;
            // player.defaultSpeed = 0; //TODO Why is it not working?
            publishEvent(event::Jump());
            std::cout << "HIT" << std::endl;
        } else
            std::cout << "MISS" << std::endl;
    }
}
} // namespace poc3d::system
