/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ModelManager.cpp
*/

#include "GEngine/libdev/systems/driver/output/ModelManager.hpp"

namespace gengine::system::driver::output {
ModelManager::ModelManager(const std::string &folder)
    : m_folder(folder) {
}

void ModelManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&ModelManager::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&ModelManager::onStopEngine);
}

void ModelManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();

            // Check if the file has a valid image extension
            // if (isValidImageFile(filePath)) {
            std:: cout << "begin" << std::endl;
            Model model = LoadModel(filePath.c_str());
            std:: cout << "end" << std::endl;
            std::string path = std::filesystem::relative(entry.path(), m_folder).string();
            m_modelTable.emplace(path, model);
            // }
        }
    }
}

void ModelManager::onStopEngine(gengine::system::event::StopEngine &e) {
    for (auto &[path, model] : m_modelTable) {
        UnloadModel(model);
    }
}

const Model &ModelManager::get(const std::string &path) {
    const auto &model = m_modelTable.find(path);
    if (model == m_modelTable.cend())
        THROW_ERROR("Out of range: This model does not exist.");

    return model->second;
}
} // namespace gengine::system::driver::output