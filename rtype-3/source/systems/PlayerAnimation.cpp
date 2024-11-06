
/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerAnimation.cpp
*/

#include "systems/PlayerAnimation.hpp"

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"

namespace rtype::system {
void PlayerAnimation::init(void) {
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Movement>>(&PlayerAnimation::animatePlayer);
}

void PlayerAnimation::animatePlayer(gengine::interface::event::SharedEvent<event::Movement> &e) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    auto &animations = getComponents<gengine::component::driver::output::Animation>();

    for (auto [entity, remote, anim] : gengine::Zip(remotes, animations)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        if (e->state == event::Movement::DOWN || e->state == event::Movement::DOWN_LEFT ||
            e->state == event::Movement::DOWN_RIGHT) {
            anim.setPlaybackMode(gengine::component::driver::output::AnimationTrack::Reverse);
        } else if (e->state == event::Movement::UP || e->state == event::Movement::UP_LEFT ||
                   e->state == event::Movement::UP_RIGHT) {
            anim.setPlaybackMode(gengine::component::driver::output::AnimationTrack::Forward);
        } else {
            anim.setPlaybackMode(gengine::component::driver::output::AnimationTrack::Idling);
        }
    }
}
} // namespace rtype::system
