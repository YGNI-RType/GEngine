/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** AudioManager.cpp
*/

#include "GEngine/libdev/systems/driver/output/AudioManager.hpp"

namespace gengine::system::driver::output {

AudioManager::AudioManager(const std::string &soundFolder, const std::string &musicFolder)
    : m_soundFolder(soundFolder)
    , m_musicFolder(musicFolder) {
}

void AudioManager::onStartEngine(gengine::system::event::StartEngine &e) {
    SetTraceLogLevel(LOG_WARNING);
    InitAudioDevice();
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_soundFolder)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();

            Sound sound = LoadSound(filePath.c_str());
            std::string path = std::filesystem::relative(entry.path(), m_soundFolder).string();
            m_soundTable.insert({path, {m_soundBaseId++, sound}});
        }
    }
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_musicFolder)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();

            Music music = LoadMusicStream(filePath.c_str());
            std::string path = std::filesystem::relative(entry.path(), m_musicFolder).string();
            m_musicTable.insert({path, {m_musicBaseId++, music}});
        }
    }
}

Music AudioManager::getMusicById(std::uint64_t id) {
    for (auto &[_, pair] : m_musicTable)
        if (pair.first == id)
            return pair.second;
    THROW_WARNING("Music component not initilized");
}

void AudioManager::onStopEngine(gengine::system::event::StopEngine &e) {
    for (auto &[path, sound] : m_soundTable)
        UnloadSound(sound.second);
}

const Sound &AudioManager::get(const std::string &path) {
    const auto &sound = m_soundTable.find(path);
    if (sound == m_soundTable.cend())
        THROW_ERROR("Out of range: This sound does not exist. PATH: " + path);

    return sound->second.second;
}

void AudioManager::onMainLoop(geg::event::RenderLoop &e) {
    auto &sounds = getComponents<gengine::component::driver::output::Sound>();
    auto &musics = getComponents<gengine::component::driver::output::Music>();

    if (musics.size()) {
        auto &music = getMusicComponent();
        if (music.musicId != m_currentMusicId) {
            if (m_currentMusicId)
                StopMusicStream(getMusicById(m_currentMusicId));
            m_currentMusicId = music.musicId;
            PlayMusicStream(getMusicById(m_currentMusicId));
            SetMusicVolume(getMusicById(m_currentMusicId), music.volume);
        }
    }

    if (m_currentMusicId)
        UpdateMusicStream(getMusicById(m_currentMusicId));

    static std::set<gengine::Entity> m_soundsPlayed;

    for (auto &[e, sound] : sounds) {
        if (m_soundsPlayed.find(e) == m_soundsPlayed.end()) {
            m_soundsPlayed.insert(e);
            playSoundById(sound.soundId);
        }
        publishEvent(gengine::system::event::driver::output::SoundPlayed(sound.soundId));
        killEntity(e);
    }
    for (auto it = m_soundsPlayed.begin(); it != m_soundsPlayed.end();)
        if (!sounds.contains(*it))
            it = m_soundsPlayed.erase(it);
        else
            ++it;
}

gengine::component::driver::output::Music &AudioManager::getMusicComponent(void) {
    auto &musics = getComponents<gengine::component::driver::output::Music>();

    if (!musics.size())
        THROW_WARNING("Music component not initilazed");
    for (auto &[_, m] : musics)
        return m;
}

void AudioManager::onSoundPlayed(
    gengine::interface::event::SharedEvent<gengine::system::event::driver::output::SoundPlayed> &e) {
    static std::unordered_map<std::uint64_t, std::set<uuids::uuid>> m_soundsAck;
    auto &remoteLocals = getComponents<gengine::interface::component::RemoteLocal>();
    auto &sounds = getComponents<gengine::component::driver::output::Sound>();

    if (m_soundsAck.find(e->soundId) == m_soundsAck.end())
        m_soundsAck.insert({e->soundId, std::set<uuids::uuid>()});
    m_soundsAck[e->soundId].insert(e.remoteUUID);

    for (auto &[entity, s] : sounds) {
        if (s.soundId == e->soundId && m_soundsAck[e->soundId].size() == remoteLocals.size())
            killEntity(entity);
        m_soundsAck.erase(e->soundId);
    }
}

std::uint64_t AudioManager::getIdByPath(const std::string &path) const {
    auto it = m_soundTable.find(path);

    if (it == m_soundTable.end())
        THROW_WARNING("Sound not found");

    return it->second.first;
}

void AudioManager::playSoundById(std::uint64_t id) {
    for (auto &[_, pair] : m_soundTable) {
        if (pair.first == id) {
            PlaySound(pair.second);
            return;
        }
    }
}

AudioManagerLocal::AudioManagerLocal(const std::string &soundFolder, const std::string &musicFolder)
    : AudioManager(soundFolder, musicFolder) {
}

void AudioManagerLocal::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&AudioManager::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&AudioManager::onStopEngine);
    subscribeToEvent<geg::event::RenderLoop>(&AudioManager::onMainLoop);
}

AudioManagerRemote::AudioManagerRemote(const std::string &soundFolder, const std::string &musicFolder)
    : AudioManager(soundFolder, musicFolder) {
}

void AudioManagerRemote::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&AudioManager::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&AudioManager::onStopEngine);
    subscribeToEvent<gengine::system::event::driver::output::Sound>(&AudioManager::onSound);
    subscribeToEvent<gengine::interface::event::SharedEvent<gengine::system::event::driver::output::SoundPlayed>>(
        &AudioManager::onSoundPlayed);
    subscribeToEvent<gengine::system::event::driver::output::Music>(&AudioManager::onMusic);
}
} // namespace gengine::system::driver::output
