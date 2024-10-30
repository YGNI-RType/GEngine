/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Event
** ════════════════════════════════════════════════════════════════════════════
**  File        : CLI.hpp
**  Create at   : 2024-10-28 16:52
**  Author      : b3w
**  Description : DESCRIPTION
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace gengine::system::event {
struct CLINewInput : public gengine::Event {
    CLINewInput(std::vector<std::string> &&prompt)
        : prompt(prompt) {
    }

    std::vector<std::string> prompt;
};
} // namespace gengine::system::event