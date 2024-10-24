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
Updater::Updater(const BaseEngine::world_t &localWorld)
    : m_localWorld(localWorld) {
}

void Updater::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&Updater::onGameLoop);
}

// void Updater::onStartEngine(gengine::system::event::StartEngine &e) {
// TODO get base world ?
// }

void Updater::onGameLoop(gengine::system::event::GameLoop &e) {
    Network::CLNetClient &cl = Network::NET::getClient();

    size_t size = cl.getSizeIncommingData(Network::SV_SNAPSHOT, true);
    for (size_t i = 0; i < size; i++) {
        Network::UDPMessage msg(true, Network::SV_SNAPSHOT);
        size_t readCount;
        if (!cl.popIncommingData(msg, readCount, true))
            continue;
        handleSnapshotMsg(msg, readCount);
    }

    auto &sends = getComponents<component::NetSend>();
    for (auto &[entity, send] : sends) { // TODO kill with PING
        killEntity(entity);
    }
}

int getNthBit(uint8_t byte, int n) {
    return (byte >> n) & 1;
}

void Updater::handleSnapshotMsg(Network::UDPMessage &msg, size_t readCount) {
    int nbComp = m_localWorld.size();
    uint32_t nbEntity;
    msg.readContinuousData(nbEntity, readCount);
    // std::cout << "RECV: " << msg.getSize() << " snap " << nbEntity << std::endl;
    for (int i = 0; i < nbEntity; i++) {
        uint32_t entity;
        msg.readContinuousData(entity, readCount);
        std::vector<uint8_t> bytes((nbComp * 2) / 8 + (nbComp * 2 % 8 ? 1 : 0), 0);
        int j = 0;
        for (auto &byte : bytes) {
            msg.readContinuousData(byte, readCount);
        }

        for (int typeId = 0; typeId < nbComp; typeId++) {
            if (getNthBit(bytes[typeId / 8], typeId % 8)) {
                auto &type = getTypeindex(typeId);
                auto size = getComponentSize(type);
                std::vector<Network::byte_t> component(size);
                msg.readData(component.data(), readCount, size);
                readCount += size;
                setComponent(entity, type, toAny(type, component.data()));
            } else if (getNthBit(bytes[(typeId + nbComp) / 8], (typeId + nbComp) % 8)) {
                auto &type = getTypeindex(typeId);
                unsetComponent(entity, type);
            }
        }
    }
}
} // namespace gengine::interface::network::system