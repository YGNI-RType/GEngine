/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cvar
*/

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

namespace CVar {

// Templating may result issues on convar indexing
class ConVar {
public:
    enum flag {
        NONE = 0x00,
        CHEAT = 0x01,       // used only when cheat enabled
        CMD = 0x02,         // only in cmd (starting the software)
        ROM = 0x04,         // nobody can change the value
        SYSTEM = 0x08,      // System commands
        SERVER = 0x10,      // Server commands, needs to transmit to the server
        CLIENT = 0x20,      // Client commands, based on self process
        NET_CLIENT = 0x40,  // Linked to the client that is asking to change the value
        CON_CALLBACK = 0x80 // Callback function
    };

public:
    ConVar(const std::string &name, const std::string &value, short flags, const std::string &description = "");
    ConVar(const std::string &name, const std::function<std::string(const std::string &)> &function, short flags,
           const std::string &description = "");
    ~ConVar() = default;

    const std::string &getRawValue(void) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_value;
    }
    const int getIntValue(void) const {
        return m_intValue;
    }
    const std::string &getDescription(void) const {
        return m_description;
    }

    const short getFlags(void) const {
        return m_flags;
    }
    void setValue(const std::string &value, bool isUserInput = false);

    static ConVar *findCvar(const std::string &name);
    std::string applyCallback(const std::string &value) const {
        return m_callback(value);
    }

private:
    static std::unordered_map<std::string, ConVar *> &getAllCommands() {
        static std::unordered_map<std::string, ConVar *> instance = {};
        return instance;
    }

    static std::unordered_map<std::string, ConVar *> &getPublicCommands() {
        static std::unordered_map<std::string, ConVar *> instance = {};
        return instance;
    }

private:
    std::string m_value;
    std::string m_description;
    std::atomic_int m_intValue;
    short m_flags;
    std::function<std::string(const std::string &)> m_callback;

    mutable std::mutex m_mutex;
};

extern ConVar sv_cheats;

} // namespace CVar
