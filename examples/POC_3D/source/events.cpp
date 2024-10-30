/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** driver.cpp
*/

#include "GEngine/GEngine.hpp"

#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "events/Movement.hpp"

void GEngineDeclareEvents(Registry *r) {
    // r->registerSystem<
    //     gengine::interface::network::system::ClientEventPublisher<
    //         poc3d::event::Movement,
    //         poc3d::event::Shoot
    //     >
    // >();
    // r->registerSystem<
    //     gengine::interface::network::system::ServerEventReceiver<
    //         poc3d::event::Movement,
    //         poc3d::event::Shoot
    //     >
    // >();
}
