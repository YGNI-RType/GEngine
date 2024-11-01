/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Event
** ════════════════════════════════════════════════════════════════════════════
**  File        : Draw.hpp
**  Create at   : 2024-10-15 05:14
**  Author      : AUTHOR
**  Description : DESCRIPTION // TODO popoche
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/tools/Math.hpp"

namespace gengine::system::event {
class WindowResized : public Event {
public:
    Vect2 ratio;
    WindowResized(const Vect2 &ratio)
        : ratio(ratio) {
    }
};
} // namespace gengine::system::event
