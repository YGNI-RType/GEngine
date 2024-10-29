/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.cpp
*/

#include "systems/PlayerMotion.hpp"

#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include <random>

namespace rtype::system {
void PlayerMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerMotion::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Movement>>(&PlayerMotion::movePlayer);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::Rotation>>(&PlayerMotion::rotatePlayer);
}

void PlayerMotion::onGameLoop(gengine::system::event::GameLoop &e) {

}

void PlayerMotion::movePlayer(gengine::interface::event::SharedEvent<event::Movement> &e) {
    auto &velocities = getComponents<gengine::component::Velocity3D>();
    auto &transforms = getComponents<gengine::component::Transform3D>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, velocity, transform] : gengine::Zip(remotes, players, velocities, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        // Calculate the direction based on the player's rotation (assuming yaw affects forward/backward movement)
        float yawRadians = (transform.rotation.z - 90) * 3.14f / 180.f;  // Convert to radians
        gengine::Vect3 forward = {cos(yawRadians), 0, -sin(yawRadians)}; // Forward direction
        gengine::Vect3 right = {sin(yawRadians), 0, cos(yawRadians)};    // Right direction

        // Handle movement input
        switch (e->state) {
        case event::Movement::LEFT:
            velocity = {-player.speed * right.x, 0.f, -player.speed * right.z}; // Move left
            break;
        case event::Movement::RIGHT:
            velocity = {player.speed * right.x, 0, player.speed * right.z}; // Move right
            break;
        case event::Movement::FWD:
            velocity = {player.speed * forward.x, 0.f, player.speed * forward.z}; // Move forward
            std::cout << "Velocity: " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
            break;
        case event::Movement::BCK:
            velocity = {-player.speed * forward.x, 0, -player.speed * forward.z}; // Move backward
            break;
        case event::Movement::FWD_RIGHT:
            velocity = {player.speed * (forward.x + right.x) / sqrt(2.f), 0,
                        player.speed * (forward.z + right.z) / sqrt(2.f)}; // Move forward-right
            break;
        case event::Movement::FWD_LEFT:
            velocity = {player.speed * (forward.x - right.x) / sqrt(2.f), 0,
                        player.speed * (forward.z - right.z) / sqrt(2.f)}; // Move forward-left
            break;
        case event::Movement::BCK_RIGHT:
            velocity = {-player.speed * (forward.x - right.x) / sqrt(2.f), 0,
                        -player.speed * (forward.z - right.z) / sqrt(2.f)}; // Move backward-right
            break;
        case event::Movement::BCK_LEFT:
            velocity = {-player.speed * (forward.x + right.x) / sqrt(2.f), 0,
                        -player.speed * (forward.z + right.z) / sqrt(2.f)}; // Move backward-left
            break;
        case event::Movement::STANDING:
            velocity = {0, 0, 0}; // Stop moving
            break;
        }

        
    }
}

void PlayerMotion::rotatePlayer(gengine::interface::event::SharedEvent<event::Rotation> &e) {
    auto &players = getComponents<component::Player>();
    auto &transforms = getComponents<gengine::component::Transform3D>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, remote, player, transform] : gengine::Zip(remotes, players, transforms)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        transform.rotation.x += e->move.x;
        transform.rotation.y += e->move.y;
        transform.rotation.z += e->move.z;
    }
}
} // namespace rtype::system
