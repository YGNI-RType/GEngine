/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** MusicManager.cpp
*/

#include "GEngine/libdev/systems/MusicManager.hpp"

#include <filesystem>

namespace gengine::system {
    MusicManager::MusicManager(const std::string &musicFolder)
        : m_musicFolder(musicFolder) {
        }

    void MusicManager::init(void) {
        subscribeToEvent<gengine::system::event::StartEngine>(&MusicManager::onStartEngine);
        subscribeToEvent<gengine::system::event::game::PlayMusic>(&MusicManager::onPlayMusic);
        subscribeToEvent<gengine::system::event::game::StopMusic>(&MusicManager::onStopMusic);
    }

    void MusicManager::onStartEngine(gengine::system::event::StartEngine &e) {
        spawnEntity(gengine::component::Music(0));
        for (const auto &entry : std::filesystem::recursive_directory_iterator(m_musicFolder)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();

                // Check if the file has a valid image extension
                // if (isValidImageFile(filePath)) {
                m_musicTable.insert(std::make_pair(std::filesystem::relative(entry.path(), m_musicFolder).string(), m_musicId));
                m_musicId++;
                // }
            }
        }
    }

    void MusicManager::onPlayMusic(gengine::system::event::game::PlayMusic &e) {
        auto it = m_musicTable.find(e.path);

        if (it == m_musicTable.end())
            throw std::runtime_error("not found"); // TODO
        getMusicComponent().musicId = it->second;
    }

    void MusicManager::onStopMusic(gengine::system::event::game::StopMusic &e) {
        getMusicComponent().musicId = 0;
    }

    gengine::component::Music &MusicManager::getMusicComponent(void) { // duplication from AudioDriver.cpp
        auto &components = getComponents<gengine::component::Music>();

        if (components.size() != 1) {
            throw std::runtime_error("empty"); // TODO
        }
        for (auto &[entity, component]: components) {
            return component;
        }
        throw std::runtime_error("alala");
    }
}