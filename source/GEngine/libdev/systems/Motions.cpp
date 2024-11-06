/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Motions.cpp
*/

#include "GEngine/libdev/systems/Motions.hpp"

#include "GEngine/libdev/Constants.hpp"

namespace gengine::system {
void Motion2D::init(void) {
    subscribeToEvent<event::GameLoop>(&Motion2D::onGameLoop);
}

void Motion2D::onGameLoop(event::GameLoop &e) {
    auto &transforms = getComponents<component::Transform2D>();
    auto &velocities = getComponents<component::Velocity2D>();

    for (auto [entity, tr, vel] : Zip(transforms, velocities)) {
        tr.pos.x += vel.x * TPS_MAGIC_NUMBER;
        tr.pos.y += vel.y * TPS_MAGIC_NUMBER;
    }
}

void Motion3D::init(void) {
    subscribeToEvent<event::GameLoop>(&Motion3D::onGameLoop);
}
void Motion3D::onGameLoop(event::GameLoop &e) {
    auto &transforms = getComponents<component::Transform3D>();
    auto &velocities = getComponents<component::Velocity3D>();

    for (auto [entity, tr, vel] : Zip(transforms, velocities)) {
        tr.pos.x += vel.x * TPS_MAGIC_NUMBER;
        tr.pos.y += vel.y * TPS_MAGIC_NUMBER;
        tr.pos.z += vel.z * TPS_MAGIC_NUMBER;
    }
}

void MotionAcceleration2D::init(void) {
    subscribeToEvent<event::GameLoop>(&MotionAcceleration2D::onGameLoop);
}

void MotionAcceleration2D::onGameLoop(event::GameLoop &e) {
    auto &velocities = getComponents<component::Velocity2D>();
    auto &accelerations = getComponents<component::Acceleration2D>();

    for (auto [entity, velocity, acceleration] : Zip(velocities, accelerations)) {
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
    }
}

void MotionAcceleration3D::init(void) {
    subscribeToEvent<event::GameLoop>(&MotionAcceleration3D::onGameLoop);
}

void MotionAcceleration3D::onGameLoop(event::GameLoop &e) {
    auto &velocities = getComponents<component::Velocity3D>();
    auto &accelerations = getComponents<component::Acceleration3D>();

    for (auto [entity, velocity, acceleration] : Zip(velocities, accelerations)) {
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        velocity.z += acceleration.z;
    }
}
} // namespace gengine::system
