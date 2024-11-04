/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : SoundManager.hpp
**  Create at   : 2024-10-15 05:02
**  Author      : AUTHOR
**  Description : This system dedicated to ther DriverEngine, it must with
                    the raylib play and manage sounds.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/driver/output/Sound.hpp"
#include "GEngine/libdev/components/driver/output/Sound.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/interface/components/RemoteLocal.hpp"

#include "ecs/entity/Entity.hpp"

#include "GEngine/libdev/Events.hpp"

#include "module/raylib_safe.h"

#include <filesystem>
#include <unordered_map>
#include <set>

namespace gengine::system::driver::output {

class SoundManager : public gengine::System<SoundManager, gengine::component::driver::output::Sound, gengine::interface::component::RemoteLocal> {
public:
    SoundManager(const std::string &folder);
    virtual void init(void) override {
    }

    void onStartEngine(gengine::system::event::StartEngine &e);
    void onStopEngine(gengine::system::event::StopEngine &e);

    const Sound &get(const std::string &path);

    void onSound(gengine::system::event::driver::output::Sound &e) {
        spawnEntity(gengine::component::driver::output::Sound(getIdByPath(e.getPath())), geg::component::network::NetSend());
    }

    void onMainLoop(geg::event::MainLoop &e);
    void onSoundPlayed(gengine::interface::event::SharedEvent<gengine::system::event::driver::output::SoundPlayed> &e);


    protected:
        std::string m_folder;
        std::unordered_map<std::string, std::pair<std::uint64_t, Sound>> m_soundTable;
        std::uint64_t m_baseId = 0;

        std::uint64_t getIdByPath(const std::string &path) const;

        void playSoundById(std::uint64_t id);
    };

class SoundManagerLocal : public SoundManager, public gengine::LocalSystem {
public:
    SoundManagerLocal(const std::string &folder);

    void init(void) override;
};

class SoundManagerRemote : public SoundManager, public gengine::RemoteSystem {
public:
    SoundManagerRemote(const std::string &folder);

    void init(void) override;
};

} // namespace gengine::system::driver::output
