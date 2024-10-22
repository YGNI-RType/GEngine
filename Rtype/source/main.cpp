/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

// TODO reorder include

#include "GEngine/GEngine.hpp"
#include "GEngine/interface/network/Networked.hpp"
#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "events/Movement.hpp"
#include "events/Shoot.hpp"

int main(int argc, char **argv) {
    std::string ip;
    if (argc > 1)
        ip = std::string(argv[1]);
    else
        ip = "127.0.0.1";

    GEngine::init();

    GEngine::getDriver().registerSystem<
        gengine::interface::network::system::ClientEventPublisher<
            rtype::event::Movement,
            rtype::event::Shoot
        >
    >();

    GEngine::getGame().registerSystem<
        gengine::interface::network::system::ServerEventReceiver<
            rtype::event::Movement,
            rtype::event::Shoot
        >
    >();

    gengine::interface::network::Networked interface(GEngine::getDriver(), GEngine::getGame(), ip, 4243, true);

    interface.run();
}
