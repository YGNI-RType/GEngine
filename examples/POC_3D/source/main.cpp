/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

//? ## Engine ##
#include "GEngine/GEngine.hpp"
#include "GEngine/interface/network/Networked.hpp"

int main(int argc, char **argv) {
    std::string ip;
    if (argc > 1)
        ip = std::string(argv[1]);
    else
        ip = "127.0.0.1";

    GEngine::init();
    gengine::interface::network::Networked interface(GEngine::getLocal(), GEngine::getRemote(), true);

    interface.run();
}
