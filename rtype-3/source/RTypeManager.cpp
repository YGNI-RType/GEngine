/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** RTypeManager.cpp
*/

#include "RTypeManager.hpp"
#include <fstream>

namespace rtype {
RTypeManager::RTypeManager(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("r-type.json: No such file");

    file >> jsonData;
}

std::string RTypeManager::getManagerPath(const std::string managerName) const {
    return jsonData[managerName];
}
} // namespace rtype
