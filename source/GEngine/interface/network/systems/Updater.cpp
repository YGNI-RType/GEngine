/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** Updater.cpp
*/

#include "GEngine/interface/network/systems/Updater.hpp"

#include "GEngine/net/net.hpp"
#include "GEngine/net/net_client.hpp"
#include "GEngine/net/net_common.hpp"

namespace gengine::interface::network::system {

void Updater::init(void) {
    subscribeToEvent<gengine::system::event::MainLoop>(&Updater::onMainLoop);
}

// void Updater::onStartEngine(gengine::system::event::StartEngine &e) {
// TODO get base world ?
// }

void Updater::onMainLoop(gengine::system::event::MainLoop &e) {
    Network::UDPMessage msg(true, Network::SV_SNAPSHOT);
    size_t size = 0; // cl.getSizeIncommingData();
    for (size_t i = 0; i < size; i++) {
        Network::UDPMessage msg(false, 0);
        size_t readCount;
        if (1) //! cl.popIncommingData(msg, readCount))
            continue;
        handleSnapshotMsg(msg, readCount);
    }
}

void Updater::handleSnapshotMsg(Network::UDPMessage &msg, size_t readCount) {
    uint64_t nb;
    msg.readContinuousData(nb, readCount);
    for (int i = 0; i < nb; i++) {
        NetworkComponent c;
        msg.readContinuousData(c, readCount);
        // std::cout << c.entity << " -> name: [" << c.type << "] size: " << c.size << std::endl;
        std::vector<Network::byte_t> component(c.size);
        msg.readData(component.data(), readCount, c.size);
        auto &type = getTypeindex(c.typeId); // TODO array for opti
        if (c.size)
            setComponent(c.entity, type, toAny(type, component.data()));
        else
            unsetComponent(c.entity, type);
    }
}
} // namespace gengine::interface::network::system