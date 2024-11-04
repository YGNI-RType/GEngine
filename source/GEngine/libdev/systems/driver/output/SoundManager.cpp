/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** SoundManager.cpp
*/

#include "GEngine/libdev/systems/driver/output/SoundManager.hpp"

namespace gengine::system::driver::output {

SoundManager::SoundManager(const std::string &folder)
    : m_folder(folder) {
}

void SoundManager::onStartEngine(gengine::system::event::StartEngine &e) {
    SetTraceLogLevel(LOG_WARNING);
    InitAudioDevice();
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();

            Sound sound = LoadSound(filePath.c_str());
            std::string path = std::filesystem::relative(entry.path(), m_folder).string();
            m_soundTable.insert({path, {m_baseId++, sound}});
        }
    }
}

void SoundManager::onStopEngine(gengine::system::event::StopEngine &e) {
    for (auto &[path, sound] : m_soundTable) {
        UnloadSound(sound.second);
    }
}

const Sound &SoundManager::get(const std::string &path) {
    const auto &sound = m_soundTable.find(path);
    if (sound == m_soundTable.cend())
        THROW_ERROR("Out of range: This sound does not exist. PATH: " + path);

    return sound->second.second;
}

void SoundManager::onMainLoop(geg::event::MainLoop &e) {
    auto &sounds = getComponents<gengine::component::driver::output::Sound>();

    static std::set<gengine::Entity> m_soundsPlayed;

    for (auto &[e, sound]: sounds) {
        if (m_soundsPlayed.find(e) == m_soundsPlayed.end()) {
            m_soundsPlayed.insert(e);
            playSoundById(sound.soundId);
        }
        publishEvent(gengine::system::event::driver::output::SoundPlayed(sound.soundId));
    }
    for (auto it = m_soundsPlayed.begin(); it != m_soundsPlayed.end(); ) {
        if (!sounds.contains(*it))
            it = m_soundsPlayed.erase(it);
        else
            ++it;
    }
}

void SoundManager::onSoundPlayed(gengine::interface::event::SharedEvent<gengine::system::event::driver::output::SoundPlayed> &e) {
    static std::unordered_map<std::uint64_t, std::set<uuids::uuid>> m_soundsAck;
    auto &remoteLocals = getComponents<gengine::interface::component::RemoteLocal>();
    auto &sounds = getComponents<gengine::component::driver::output::Sound>();

    if (m_soundsAck.find(e->soundId) == m_soundsAck.end())
        m_soundsAck.insert({e->soundId, std::set<uuids::uuid>()});
    m_soundsAck[e->soundId].insert(e.remoteUUID);

    for (auto &[entity, s]: sounds) {
        if (s.soundId == e->soundId && m_soundsAck[e->soundId].size() == remoteLocals.size())
            killEntity(entity);
        m_soundsAck.erase(e->soundId);
    }
}

std::uint64_t SoundManager::getIdByPath(const std::string &path) const {
    auto it = m_soundTable.find(path);

    if (it == m_soundTable.end())
        throw ""; // TODO

    return it->second.first;
}

void SoundManager::playSoundById(std::uint64_t id) {
    for (auto &[_, pair]: m_soundTable) {
        if (pair.first == id) {
            PlaySound(pair.second);
            return;
        }
    }
}

SoundManagerLocal::SoundManagerLocal(const std::string &folder)
: SoundManager(folder) {
}

void SoundManagerLocal::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&SoundManager::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&SoundManager::onStopEngine);
    subscribeToEvent<geg::event::MainLoop>(&SoundManager::onMainLoop);
}

SoundManagerRemote::SoundManagerRemote(const std::string &folder): SoundManager(folder) {
}

void SoundManagerRemote::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&SoundManager::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&SoundManager::onStopEngine);
    subscribeToEvent<gengine::system::event::driver::output::Sound>(&SoundManager::onSound);
    subscribeToEvent<gengine::interface::event::SharedEvent<gengine::system::event::driver::output::SoundPlayed>>(&SoundManager::onSoundPlayed);
}
}