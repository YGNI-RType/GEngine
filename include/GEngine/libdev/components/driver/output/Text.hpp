/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Text.hpp
**  Create at   : 2024-10-15 05:26
**  Auteur      : AUTHOR
**  Description : DESCRIPTION // TODO popoche
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "module/raylib_safe.h"
#include <array>
#include <cstring>
#include <string>

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/components/driver/output/RaylibTypes.hpp"
#include "GEngine/net/net_string.hpp"

namespace gengine::component::driver::output {
struct Text : public Component<Text> {
    Network::NetString<64> fontPath;
    Network::NetString<64> str;
    float fontSize;
    float spacing;
    Clr tint;

    Text(const std::string &fontPath, const std::string &str, const Clr &tint = BLACK, float spacing = 0)
        : fontSize(10)
        , spacing(spacing)
        , tint(tint) {
        this->fontPath = fontPath;
        this->str = str;
    }

    bool operator==(const Text &other) const {
        return this->fontPath == other.fontPath && this->str == other.str && this->fontSize == other.fontSize &&
               this->spacing == other.spacing && this->tint == other.tint;
    }
};
} // namespace gengine::component::driver::output
