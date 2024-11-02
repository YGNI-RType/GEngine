/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

//? ## Engine ##
#include "GEngine/GEngine.hpp"
#include "GEngine/interface/network/Networked.hpp"

int main(int argc, const char **argv) {

    GEngine::init(argc, argv);
    gengine::interface::network::Networked interface(GEngine::getLocal(), GEngine::getRemote(), true);

    interface.run();
}
