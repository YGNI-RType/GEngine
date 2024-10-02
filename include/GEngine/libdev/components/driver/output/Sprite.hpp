/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Sprite.hpp
*/

/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Shape.hpp
*/

#pragma once

#include <limits>
#include <raylib.h>
#include <string>

#include "GEngine/libdev/Component.hpp"

namespace gengine::component::driver::output {
struct Sprite : public gengine::Component {
    std::string txtPath; // The texture of the sprite
    ::Rectangle src;     // The portion of the texture to render (for sprite sheets/atlases)
    Color tint;

    Sprite(const std::string &txtPath, const ::Rectangle &src, const Color &tint = RAYWHITE)
        : txtPath(txtPath), src(src), tint(tint) {};
    Sprite(const std::string &txtPath, const Color &tint = RAYWHITE)
        : txtPath(txtPath), src({0.f, 0.f, std::numeric_limits<float>().max(), std::numeric_limits<float>().max()}),
          tint(tint) {};
};
} // namespace gengine::component::driver::output
