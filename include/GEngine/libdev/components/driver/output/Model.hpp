/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Model.hpp
**  Create at   : 2024-10-15 05:26
**  Auteur      : AUTHOR
**  Description : DESCRIPTION // TODO popoche
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "module/raylib_safe.h"
#include <cstring>
#include <limits>
#include <string>

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/components/driver/output/RaylibTypes.hpp"
#include "GEngine/net/net_string.hpp"

namespace gengine::component::driver::output {
struct Model : public Component<Model> {
    Network::NetString<32> txtPath; // The texture of the model
    Clr tint;
    bool isAnimated;

    Model(const std::string &txtPathstr, const Clr &tint = WHITE, bool isAnimated = false)
        : txtPath(txtPathstr)
        , tint(tint)
        , isAnimated(isAnimated) {
    }

    bool operator==(const Model &) const = default;
};
} // namespace gengine::component::driver::output
