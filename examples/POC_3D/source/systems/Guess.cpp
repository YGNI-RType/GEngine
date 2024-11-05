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
    subscribeToEvent<event::GuessEvent>(&Guess::guessWho);
    subscribeToEvent<gengine::interface::event::ItsMe>(&Guess::setMe);
}

void Guess::guessWho(event::GuessEvent &e) {
    if (m_me.is_nil())
        return;

    auto &draw = getSystem<gengine::system::driver::output::DrawModel>();
    auto &wd = getSystem<gengine::system::driver::output::RenderWindow>();

    auto &models = getComponents<gengine::component::driver::output::Model>();
    auto &players = getComponents<component::Player>();
    auto &transforms = getComponents<geg::component::Transform3D>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    auto &anims = getComponents<geg::component::io::Animation>();

    RayCollision collision = {0};
    Vector2 center = {wd.getWidth() / 2.f, wd.getHeight() / 2.f};
    Ray ray = GetMouseRay(center, draw.camera);
    auto &modelMan = getSystem<gengine::system::driver::output::ModelManager>();
    for (auto [entity, model, player, remote, transform] : gengine::Zip(models, players, remotes, transforms)) {
        if (remote.getUUIDBytes() == m_me) // check if its the same remote (zip)
            continue;
        std::string fullPath = model.txtPath;
        if (model.isAnimated && anims.contains(entity)) {
            auto &anim = anims.get(entity);
            std::string animName = anim.trackName;
            size_t frameIndex = anim.currentFrameIndex;

            fullPath += animName.substr(animName.find_last_of("/") + 1) + "/";
            if (frameIndex + 1 < 10)
                fullPath += "0";
            fullPath += std::to_string(frameIndex + 1) + ".obj";
        }
        collision = gengine::GetRayCollisionModel(ray, modelMan.get(fullPath), transform);
        if (collision.hit)
            publishEvent(event::Jump(0.4, remote.getUUIDBytes()));

        // publishEvent(event::PlayerHit(entity, remote.getUUIDBytes())); //TODO Implement hit event
    }
}
void Guess::setMe(gengine::interface::event::ItsMe &e) {
    m_me = e.myUUID;
}

} // namespace poc3d::system
