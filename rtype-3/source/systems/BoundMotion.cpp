/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BoundingMotion.cpp
*/

#include "Constants.hpp"
#include "systems/BoundingMotion.hpp"

namespace rtype::system {
void BoundingMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&BoundingMotion::onGameLoop);
}

void BoundingMotion::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &motions = getComponents<gengine::component::Velocity2D>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &sprites = getComponents<gengine::component::driver::output::Sprite>();
    auto &boundings = getComponents<component::Bounding>();

    for (auto [entity, motion, transform, sprite, bounding] : gengine::Zip(motions, transforms, sprites, boundings)) {
        if (transform.pos.y < -5 * transform.scale.y) {
            motion.y = -motion.y;
            sprite.src.x = sprite.src.width * 6;
            sprite.src.y = 0;
        }
        if (transform.pos.y > WINDOW_HEIGHT - (sprite.src.height - 4) * transform.scale.y) {
            motion.y = -motion.y;
            sprite.src.x = sprite.src.width * 2;
            sprite.src.y = sprite.src.height;
        }
    }
}
} // namespace rtype::system
