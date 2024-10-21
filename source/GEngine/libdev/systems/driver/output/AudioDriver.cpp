/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** AudioDriver.cpp
*/

#include "GEngine/libdev/systems/driver/output/AudioDriver.hpp"

namespace gengine::system::driver::output {
AudioDriver::AudioDriver(const std::string &soundFolder, const std::string &musicFolder)
    : m_soundFolder(soundFolder)
    , m_musicFolder(musicFolder) {
}

void AudioDriver::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&AudioDriver::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&AudioDriver::onStopEngine);
    subscribeToEvent<gengine::system::event::driver::output::Sound>(&AudioDriver::onSound);
    subscribeToEvent<gengine::system::event::GameLoop>(&AudioDriver::onGameLoop);
}

void AudioDriver::onStartEngine(gengine::system::event::StartEngine &e) {
    InitAudioDevice();
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_soundFolder)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();

            // Check if the file has a valid image extension
            // if (isValidImageFile(filePath)) {
            Sound sound = LoadSound(filePath.c_str());
            std::string path = std::filesystem::relative(entry.path(), m_soundFolder).string();
            m_soundTable.emplace(path, sound);
            // }
        }
    }
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_musicFolder)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();

            // Check if the fkile has a valid image extension
            // if (isValidImageFile(filePath)) {
            Music m = LoadMusicStream(filePath.c_str());
            std::cout << filePath << std::endl;
            PlayMusicStream(m);
            m_musicTable.insert(std::make_pair(m_musicId, LoadMusicStream(filePath.c_str())));
            m_musicId++;
            // }
        }
    }
}

void AudioDriver::onStopEngine(gengine::system::event::StopEngine &e) {
    for (auto &[path, sound] : m_soundTable) {
        UnloadSound(sound);
    }
    for (auto &[path, music] : m_musicTable) {
        UnloadMusicStream(music);
    }
}

const Sound &AudioDriver::getSound(const std::string &path) {
    const auto &sound = m_soundTable.find(path);
    if (sound == m_soundTable.cend())
        THROW_ERROR("Out of range: This sound does not exist.");

    return sound->second;
}

const Music &AudioDriver::getMusic(const std::uint64_t musicId) {
    const auto &music = m_musicTable.find(musicId);
    if (music == m_musicTable.cend())
        THROW_ERROR("Out of range: This music does not exist.");

    return music->second;
}

void AudioDriver::onSound(gengine::system::event::driver::output::Sound &e) {
    PlaySound(getSound(e.getPath()));
}

gengine::component::Music &AudioDriver::getMusicComponent(void) {
    auto &components = getComponents<gengine::component::Music>();

    if (components.size() != 1) {
        throw std::runtime_error("abcd"); // TODO
    }
    for (auto &[entity, component]: components) {
        return component;
    }
}

void AudioDriver::onGameLoop(gengine::system::event::GameLoop &e) {
    // const auto &music = getMusicComponent();
    for (auto &[_, music]: getComponents<gengine::component::Music>()) {
        if (music.musicId != m_currentMusic) {
            if (m_currentMusic) {
                auto it = m_musicTable.find(m_currentMusic);
                if (it == m_musicTable.end())
                    throw std::runtime_error("efh"); // TODO
                const Music &current = it->second;
                std::cout << "stop music" << std::endl;
                StopMusicStream(current);
            }
            m_currentMusic = music.musicId;
            if (!m_currentMusic)
                return;
            auto it = m_musicTable.find(m_currentMusic);
            if (it == m_musicTable.end())
                throw std::runtime_error("alalabis"); // TODO
            std::cout << "play music" << std::endl;
            PlayMusicStream(it->second);
        }
        break;
    }

}
} // namespace gengine::system::driver::output
