/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : AudioDriver.hpp
**  Create at   : 2024-10-15 05:02
**  Author      : AUTHOR
**  Description : This system dedicated to ther DriverEngine, it must with
                    the raylib play and manage sounds.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/driver/output/Sound.hpp"
#include "GEngine/libdev/components/Music.hpp"

#include "module/raylib_safe.h"

#include <filesystem>

namespace gengine::system::driver::output {
class AudioDriver : public gengine::System<AudioDriver, gengine::component::Music> {
public:
    AudioDriver(const std::string &, const std::string &);
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &e);
    void onStopEngine(gengine::system::event::StopEngine &e);
    void onSound(gengine::system::event::driver::output::Sound &e);
    void onGameLoop(gengine::system::event::GameLoop &e);

    const Sound &getSound(const std::string &path);
    const Music &getMusic(const std::uint64_t musicId);

private:
    gengine::component::Music &getMusicComponent(void);
    std::string m_soundFolder;
    std::string m_musicFolder;
    std::unordered_map<std::string, Sound> m_soundTable;
    std::unordered_map<std::uint64_t, Music> m_musicTable;
    std::uint64_t m_musicId = 1;
    std::uint64_t m_currentMusic = 0;
};
} // namespace gengine::system::driver::output