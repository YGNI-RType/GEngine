/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** CommandManager
*/

#pragma once

#include "GEngine/cvar/cvar.hpp"

#include "GEngine/BaseEngine.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/libdev/systems/events/CLI.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include <nlohmann/json.hpp>

namespace gengine::interface::network::system {

class CommandManager {
public:
    void onStartEngine(gengine::system::event::StartEngine &);

protected:
    using json = nlohmann::json;

    enum CVarResult { OK, NOT_FOUND, NOT_ALLOWED };
};

class SVCommandManager : public CommandManager,
                         public System<SVCommandManager, interface::component::RemoteLocal,
                                       gengine::interface::network::system::ServerClientsHandler>,
                         public RemoteSystem {

public:
    SVCommandManager(const std::string &path);

    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void onCLI(gengine::system::event::CLINewInput &);

private:
    void loadConfig(const std::string &path);
    void createConfig(const std::string &path);

    bool isAdmin(ServerClient &client);

    std::string com_status(const std::string &);

    CVar::ConVar m_status;
    json m_jsonData;
};

class CLCommandManager : public CommandManager, public System<CLCommandManager>, public LocalSystem {

public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void onCLI(gengine::system::event::CLINewInput &);
};

} // namespace gengine::interface::network::system