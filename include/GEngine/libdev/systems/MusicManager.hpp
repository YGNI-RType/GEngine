/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : MusicManager.hpp
**  Create at   : 2024-10-17 14:25
**  Author      : AUTHOR
**  Description : DESCRIPTION
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/libdev/systems/events/game/Music.hpp"
#include "GEngine/libdev/components/Music.hpp"

namespace gengine::system {
class MusicManager: public gengine::System<MusicManager, gengine::component::Music> {
public:
    MusicManager(const std::string &musicFolder);
    void init(void) override;
    void onStartEngine(gengine::system::event::StartEngine &e);
    void onPlayMusic(gengine::system::event::game::PlayMusic &e);
    void onStopMusic(gengine::system::event::game::StopMusic &e);
private:
    gengine::component::Music &getMusicComponent(void);
    const std::string m_musicFolder;
    std::unordered_map<std::string, std::uint64_t> m_musicTable;
    std::uint64_t m_musicId = 1;
};
}
