/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cvar
*/

#include "GEngine/cvar/cvar.hpp"

namespace CVar {

ConVar sv_cheats("sv_cheats", "0", ConVar::CHEAT | ConVar::SERVER);

////////////////////////////////////////

ConVar::ConVar(const std::string &name, const std::string &value, short flags, const std::string &description)
    : m_value(value)
    , m_intValue(std::stoi(value))
    , m_description(description)
    , m_callback(nullptr)
    , m_flags(flags) {
    getAllCommands()[name] = this;

    if (flags & ROM || flags & CMD)
        return;
    getPublicCommands()[name] = this;
}

ConVar::ConVar(const std::string &name, const std::function<std::string(const std::string &)> &function, short flags,
               const std::string &description)
    : m_callback(function)
    , m_flags(flags)
    , m_description(description) {
    getAllCommands()[name] = this;

    if (flags & ROM || flags & CMD)
        return;
    getPublicCommands()[name] = this;
}

void ConVar::setValue(const std::string &value, bool isUserInput) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (isUserInput) {
        if (m_flags & ROM || m_flags & CMD || m_flags & CALLBACK)
            return;
    }

    m_intValue = std::stoi(value);
    m_value = value;
}

ConVar *ConVar::findCvar(const std::string &name) {
    auto &allCommands = getAllCommands();
    auto it = allCommands.find(name);
    if (it == allCommands.end())
        return nullptr;

    return it->second;
}

} // namespace CVar
