/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Networked.hpp
*/

#pragma once

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Base.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"

#include "GEngine/interface/network/systems/ClientServer.hpp"
#include "GEngine/interface/network/systems/ServerClients.hpp"
#include "GEngine/interface/network/systems/Snapshot.hpp"
#include "GEngine/interface/network/systems/Updater.hpp"

#include "GEngine/net/events/connection.hpp"

#include "GEngine/net/events/disconnection.hpp"
#include "GEngine/net/msg.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/net/structs/msg_udp_structs.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#define ENTITY_ID_START_CLIENT (uint64_t(-1) / 2)

namespace gengine::interface::network {

class Networked : public Base {
public:
    Networked(BaseEngine &driverEngine, BaseEngine &gameEngine)
        : m_remote(gameEngine)
        , m_local(driverEngine) {
        m_local.setFirstEntity(ENTITY_ID_START_CLIENT);
        Network::NET::init();
#ifdef GEngine_Server
        Network::NET::initServer();
        m_remote.registerSystem<gengine::interface::network::system::Snapshot>(m_remote.getWorld());
        m_remote.registerSystem<gengine::interface::network::system::ServerClientsHandler>();
#elif GEngine_Client
        Network::NET::initClient();
        m_local.registerSystem<gengine::interface::network::system::Updater>(m_local.getWorld());
        m_local.registerSystem<gengine::interface::network::system::ClientServer>();

#endif
        Network::NET::start();

        m_remote.registerSystem<gengine::system::AutoMainLoop>(60, 40);
        m_local.registerSystem<gengine::system::AutoMainLoop>(60, 40);
    }

    ~Networked() {
        Network::NET::stop();
        Network::NET::getClient().disconnectFromServer(Network::Event::DT_WANTED);
    }

#ifdef GEngine_Server
    void run() override {
        m_remote.start();
        m_remote.compute();
    }
#else
    void run() override {
        m_local.start();
        m_local.compute();
    }
#endif

private:
    BaseEngine &m_remote;
    BaseEngine &m_local;
    // const std::string m_ip;
    // uint16_t m_port;
};

} // namespace gengine::interface::network
