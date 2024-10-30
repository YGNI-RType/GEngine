/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Event
** ════════════════════════════════════════════════════════════════════════════
**  File        : Native.hpp
**  Create at   : 2024-10-15 05:22
**  Author      : AUTHOR
**  Description : These events are published natively by the engine itself.
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"

#include <vector>
#include <string>

namespace gengine::system::event {
struct StartEngine : public Event {
    StartEngine(std::vector<std::string> &params)
    : params(params) {
    }
    std::vector<std::string> params;
};
struct StopEngine : public Event {};
} // namespace gengine::system::event
