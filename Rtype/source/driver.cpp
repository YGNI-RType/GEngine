/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** driver.cpp
*/

#include "GEngine/GEngine.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Systems.hpp"

#include "systems/InputsToGameEvents.hpp"
#include "systems/PlayerMotion.hpp"

void GEngineDeclareDriver(Registry *r) {
    r->registerSystem<geg::system::io::RenderWindow>(1280, 750, "R-Type");
    r->registerSystem<geg::system::io::Draw2D>(BLACK);
    r->registerSystem<geg::system::io::DrawSprite>();
    r->registerSystem<geg::system::io::DrawText>();

    r->registerSystem<geg::system::io::TextureManager>("../assets/sprites");
    r->registerSystem<geg::system::io::FontManager>("../assets/fonts");
    r->registerSystem<geg::system::io::KeyboardCatcher>();
    r->registerSystem<rtype::system::InputsToGameEvents>();
    r->registerSystem<geg::system::io::SoundManager>("../assets/sounds");
}
