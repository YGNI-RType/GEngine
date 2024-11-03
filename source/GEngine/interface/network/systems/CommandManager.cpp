/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** CommandManager
*/

#include "GEngine/interface/network/systems/CommandManager.hpp"

#include "GEngine/net/net.hpp"
#include "GEngine/net/net_client.hpp"
#include "GEngine/net/structs/msg_tcp_structs.hpp"

#include "GEngine/cvar/cvar.hpp"

#include <iostream>

namespace gengine::interface::network::system {

void CommandManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (auto p = e.params.begin(); p != e.params.end(); p++) {
        if (p->empty())
            continue;
        auto c = p->at(0);
        if (c != '+')
            continue;

        auto value = p + 1;
        if (value == e.params.end())
            continue;

        /* from now, this is a convar comamnd */

        auto convar = CVar::ConVar::findCvar(p->substr(1));
        if (!convar || !(convar->getFlags() & CVar::ConVar::CMD))
            continue;

        convar->setValue(*value, false);
    }
}

/*************************************/

SVCommandManager::SVCommandManager(const std::string &path)
    : m_status(
          "status", [this](const std::string &value) { return com_status(value); },
          CVar::ConVar::SERVER | CVar::ConVar::CON_CALLBACK) {
    loadConfig(path);
}

void SVCommandManager::loadConfig(const std::string &path) {
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        std::cerr << "Could not open the file: " << path << ". Creating it..." << std::endl;
        createConfig(path);
        return;
    }

    inputFile >> m_jsonData;
    inputFile.close();
}

void SVCommandManager::createConfig(const std::string &path) {
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    std::ofstream outputFile(path);
    if (!outputFile.is_open()) {
        std::cerr << "Could not create the file: " << path << std::endl;
        return;
    }

    m_jsonData["admin"] = json::array();
    m_jsonData["admin"].push_back({{"ip", "127.0.0.1"}});
    outputFile << m_jsonData.dump(4);
    outputFile.close();
}

bool SVCommandManager::isAdmin(ServerClient &client) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    auto &admins = m_jsonData["admin"];
    for (auto &admin : admins)
        if (admin["ip"] == client.getNet()->getAddress_TS())
            return true;
    return false;
}

void SVCommandManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&CommandManager::onStartEngine);
    subscribeToEvent<gengine::system::event::GameLoop>(&SVCommandManager::onGameLoop);
    subscribeToEvent<gengine::system::event::CLINewInput>(&SVCommandManager::onCLI);
}

void SVCommandManager::onGameLoop(gengine::system::event::GameLoop &) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    for (auto &[remote, client] : clientsSys.getClients()) {
        if (client.shouldDelete())
            continue;

        size_t readCount = 0;
        Network::TCPMessage recvMsg(Network::CL_CVAR);
        if (!client.getNet()->popIncommingStream(recvMsg, readCount))
            continue;

        Network::TCPCL_CVar cvar;
        recvMsg.readData(cvar);

        Network::TCPMessage sendMsg(Network::SV_CVAR);
        Network::TCPSV_CVar sendCvar;
        std::memset(&sendCvar, 0, sizeof(sendCvar));

        auto convar = CVar::ConVar::findCvar(cvar.name);
        if (!convar) {
            sendCvar.result = NOT_FOUND;
            sendMsg.writeData(sendCvar);
            client.getNet()->pushStream(sendMsg);
            continue;
        }

        auto flags = convar->getFlags();
        if (flags & CVar::ConVar::CHEAT && !isAdmin(client)) {
            sendCvar.result = NOT_ALLOWED;
            sendMsg.writeData(sendCvar);
            client.getNet()->pushStream(sendMsg);
            continue;
        }

        sendCvar.result = OK;
        if (flags & CVar::ConVar::CON_CALLBACK) {
            std::string value = convar->applyCallback(cvar.value);
            std::memcpy(sendCvar.output, value.c_str(), CF_NET_MIN(Network::MAX_CONCOMMAND_OUTPUT_LEN, value.size()));
        } else
            convar->setValue(cvar.value, false);
        sendMsg.writeData(sendCvar);
        client.getNet()->pushStream(sendMsg);
    }
}

void SVCommandManager::onCLI(gengine::system::event::CLINewInput &e) {
    if (e.prompt.size() == 0)
        return;

    auto convar = CVar::ConVar::findCvar(e.prompt[0]);
    if (!convar)
        return;

    auto flags = convar->getFlags();
    if (!(flags & CVar::ConVar::SERVER))
        return;

    if (flags & CVar::ConVar::CON_CALLBACK)
        std::cout << convar->applyCallback(e.prompt[1]) << std::endl;
    else
        convar->setValue(e.prompt[1], true);
}

std::string SVCommandManager::com_status(const std::string &) {
    std::string buffer;
    size_t i = 0;
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    auto &server = Network::NET::getServer();

    buffer += "Server status:\n";
    buffer += "----------------\n";
    buffer += "Clients connected: " + std::to_string(clientsSys.getClients().size()) + "/" +
              std::to_string(server.getMaxClients()) + "\n";
    for (auto &[remote, client] : clientsSys.getClients()) {
        buffer += std::to_string(i++);
        buffer += ": Ping: ";
        buffer += std::to_string((client.getNet()->getPing_TS()));
        buffer += "\n";
    }
    buffer += "----------------";
    return buffer;
}

/*************************************/

void CLCommandManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&CommandManager::onStartEngine);
    subscribeToEvent<gengine::system::event::GameLoop>(&CLCommandManager::onGameLoop);
    subscribeToEvent<gengine::system::event::CLINewInput>(&CLCommandManager::onCLI);
}

void CLCommandManager::onCLI(gengine::system::event::CLINewInput &e) {
    if (e.prompt.size() == 0)
        return;

    auto convar = CVar::ConVar::findCvar(e.prompt[0]);
    if (!convar)
        return;

    auto flags = convar->getFlags();
    if (flags & CVar::ConVar::CLIENT)
        convar->setValue(e.prompt[1], true);
    else if (flags & CVar::ConVar::SERVER) {
        auto &net = Network::NET::getClient();
        Network::TCPMessage msg(Network::CL_CVAR);

        Network::TCPCL_CVar cvar;
        std::memset(&cvar, 0, sizeof(cvar));
        std::memcpy(cvar.name, e.prompt[0].c_str(), CF_NET_MIN(Network::MAX_CONCOMMAND_NAME_LEN, e.prompt[0].size()));
        if (e.prompt.size() > 1)
            std::memcpy(cvar.value, e.prompt[1].c_str(),
                        CF_NET_MIN(Network::MAX_CONCOMMAND_VAL_LEN, e.prompt[1].size()));

        msg.writeData(cvar);
        net.pushStream(msg);
    }
}

void CLCommandManager::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &net = Network::NET::getClient();
    size_t readCount = 0;
    Network::TCPMessage recvMsg(Network::SV_CVAR);
    if (!net.popIncommingStream(recvMsg, readCount))
        return;

    Network::TCPSV_CVar cvar;
    recvMsg.readData(cvar);

    if (cvar.result == NOT_FOUND)
        std::cerr << "Convar not found" << std::endl;
    else if (cvar.result == NOT_ALLOWED)
        std::cerr << "Convar not allowed" << std::endl;
    else
        std::cout << cvar.output << std::endl;
}

} // namespace gengine::interface::network::system