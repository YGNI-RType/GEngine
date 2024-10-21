/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Event
** ════════════════════════════════════════════════════════════════════════════
**  File        : Music.hpp
**  Create at   : 2024-10-17 14:31
**  Author      : AUTHOR
**  Description : DESCRIPTION
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace gengine::system::event::game {
struct PlayMusic: public gengine::Event {
    std::string path;
    PlayMusic(const std::string &path): path(path) {
    }
};

struct StopMusic: public gengine::Event {
};
}
