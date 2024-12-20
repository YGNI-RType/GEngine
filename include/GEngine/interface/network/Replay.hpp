#pragma once

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Base.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"

#include "GEngine/interface/network/systems/RecordManager.hpp"
#include "GEngine/interface/network/systems/Updater.hpp"

#include "GEngine/net/events/disconnection.hpp"
#include "GEngine/net/events/record.hpp"
#include "GEngine/net/msg.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/net/structs/msg_udp_structs.hpp"

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#define ENTITY_ID_START_CLIENT (uint64_t(-1) / 2)

namespace gengine::interface::network {

class Replay : public Base {
public:
    Replay(BaseEngine &driverEngine, BaseEngine &gameEngine, const std::string &demoPath)
        : m_remote(gameEngine)
        , m_local(driverEngine)
        , m_demoPath(demoPath) {

        if (!std::filesystem::exists(demoPath))
            THROW_INFO("file: " + demoPath + " does not exist.");

        m_local.setFirstEntity(ENTITY_ID_START_CLIENT);
        Network::NET::init();
        Network::NET::initClient();

        m_local.registerSystem<gengine::interface::network::system::Updater>(m_local.getWorld());
        Network::NET::start();

        m_remote.registerSystem<gengine::system::AutoMainLoop>(60, 40);
        m_local.registerSystem<gengine::system::AutoMainLoop>(60, 40);

        m_local.registerSystem<gengine::interface::network::system::RecordManager>();
    }

    ~Replay() {
        Network::NET::stop();
    }

    void run() override {
        m_local.start();

        auto e = gengine::interface::network::event::WatchReplay(m_demoPath);
        m_local.publishEvent(e);

        m_local.compute();
    }

private:
    BaseEngine &m_remote;
    BaseEngine &m_local;
    std::string m_demoPath;
};

} // namespace gengine::interface::network
