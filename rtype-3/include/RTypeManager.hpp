/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** RTypeManager
*/

#pragma once

#include "nlohmann/json.hpp"

namespace rtype {
class RTypeManager {
public:
    RTypeManager(const std::string &file);

    std::string getManagerPath(const std::string managerName) const;

private:
    nlohmann::json jsonData;
};
} // namespace rtype
