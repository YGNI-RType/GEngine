/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : AudioManager.hpp
**  Create at   : 2024-10-15 05:02
**  Author      : AUTHOR
**  Description : This system dedicated to ther DriverEngine, it must with
                    the raylib play and manage sounds.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/components/driver/output/Sound.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/driver/output/Sound.hpp"

#include "ecs/entity/Entity.hpp"

#include "GEngine/libdev/Events.hpp"

#include "module/raylib_safe.h"

#include <filesystem>
#include <set>
#include <unordered_map>

namespace gengine::system::driver::output {

class AudioManager
    : public gengine::System<AudioManager, gengine::component::driver::output::Sound,
                             gengine::component::driver::output::Music, gengine::interface::component::RemoteLocal,
                             geg::component::network::NetSend> {
public:
    AudioManager(const std::string &soundFolder, const std::string &musicFolder);
    virtual void init(void) override {
    }

    void onStartEngine(gengine::system::event::StartEngine &e);
    void onStopEngine(gengine::system::event::StopEngine &e);

    const Sound &get(const std::string &path);

    void onSound(gengine::system::event::driver::output::Sound &e) {
        spawnEntity(gengine::component::driver::output::Sound(getIdByPath(e.getPath())),
                    geg::component::network::NetSend());
    }

    void onMainLoop(geg::event::MainLoop &e);
    void onSoundPlayed(gengine::interface::event::SharedEvent<gengine::system::event::driver::output::SoundPlayed> &e);
    void onMusic(gengine::system::event::driver::output::Music &e) {
        auto &musics = getComponents<gengine::component::driver::output::Music>();
        auto &netSends = getComponents<geg::component::network::NetSend>();

        if (!musics.size())
            spawnEntity(gengine::component::driver::output::Music(getMusicIdByPath(e.path)),
                        geg::component::network::NetSend());
        else {
            getMusicComponent().musicId = getMusicIdByPath(e.path);
            for (auto [e, _unused, netSend] : gengine::Zip(musics, netSends)) {
                netSend.update();
                break;
            }
        }
        std::cout << "music: " << getMusicComponent().musicId << std::endl;
    }

protected:
    const std::string m_soundFolder;
    const std::string m_musicFolder;
    std::string m_folder;
    std::unordered_map<std::string, std::pair<std::uint64_t, Sound>> m_soundTable;
    std::unordered_map<std::string, std::pair<std::uint64_t, Music>> m_musicTable;
    std::uint64_t m_soundBaseId = 0;
    std::uint64_t m_musicBaseId = 1;
    std::uint64_t m_currentMusicId = 0;

    std::uint64_t getIdByPath(const std::string &path) const;
    Music getMusicById(std::uint64_t id);
    std::uint64_t getMusicIdByPath(const std::string &path) {
        auto it = m_musicTable.find(path);

        if (it == m_musicTable.end())
            throw THROW_WARNING("Music not found");

        return it->second.first;
    }
    void playSoundById(std::uint64_t id);

    gengine::component::driver::output::Music &getMusicComponent(void);
};

class AudioManagerLocal : public AudioManager, public gengine::LocalSystem {
public:
    AudioManagerLocal(const std::string &soundFolder, const std::string &musicFolder);

    void init(void) override;
};

class AudioManagerRemote : public AudioManager, public gengine::RemoteSystem {
public:
    AudioManagerRemote(const std::string &soundFolder, const std::string &musicFolder);

    void init(void) override;
};

} // namespace gengine::system::driver::output
