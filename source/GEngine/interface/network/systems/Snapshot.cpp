/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Snapshot.cpp
*/

#include "GEngine/interface/network/systems/Snapshot.hpp"

#include "GEngine/libdev/Components.hpp"
#include "GEngine/net/msg.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/net/net_client.hpp"

namespace gengine::interface::network::system {

Snapshot::Snapshot(const snapshot_t &currentWorld)
    : m_currentWorld(currentWorld)
    , m_dummySnapshot(currentWorld) { // TODO dummy must created after components registered
}

void Snapshot::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&Snapshot::onGameLoop);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&Snapshot::registerSnapshot);
}

void Snapshot::onGameLoop(gengine::system::event::GameLoop &e) {
    m_currentSnapshotId++;
    getAndSendDeltaDiff();
}

void Snapshot::registerSnapshot(gengine::interface::event::NewRemoteLocal &e) {
    m_clientSnapshots.insert(std::make_pair(e.uuid, std::make_pair(m_currentSnapshotId, snapshots_t())));
    m_clientSnapshots[e.uuid].second.fill(m_dummySnapshot);
}

void Snapshot::destroySnapshot(gengine::interface::event::DeleteRemoteLocal &e) {
    m_clientSnapshots.erase(e.uuid);
}

void Snapshot::getAndSendDeltaDiff(void) {
    auto &currentNetSends = getComponents<component::NetSend>();
    auto &remotes = getComponents<interface::component::RemoteLocal>();
    for (auto [e, r, netsend] : Zip(remotes, currentNetSends))
        netsend.update();

    auto &server = Network::NET::getServer();
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    for (auto &[uuid, client] : clientsSys.getClients()) {
        if (client.shouldDelete() || !client.isReady())
            continue;
        auto it = m_clientSnapshots.find(uuid);
        if (it == m_clientSnapshots.end())
            continue;

        auto &[firstSnapshotId, snapshots] = it->second;
        auto lastReceived = client.getLastAck();
        auto lastId = firstSnapshotId + lastReceived;
        auto diff = m_currentSnapshotId - lastId;

        if (diff % MAX_SNAPSHOT)
            snapshots[m_currentSnapshotId % MAX_SNAPSHOT] = m_currentWorld; // does not erase lastsnapshot received
        // std::cout << "client : " << uuids::to_string(uuid) << " | diff: " << diff << " | m_currentSnapshotId: " <<
        // m_currentSnapshotId << " last id: " << lastId
        // << " UDP Last ACK: " << lastReceived << std::endl;

        bool fullSnapshot = !lastReceived;
        auto &current = snapshots[m_currentSnapshotId % MAX_SNAPSHOT];
        auto &last = fullSnapshot ? m_dummySnapshot : snapshots[lastId % MAX_SNAPSHOT];

        try {
            auto &lastNetSends = std::any_cast<ecs::component::SparseArray<component::NetSend> &>(
                last[std::type_index(typeid(component::NetSend))]);

            Network::UDPMessage msg(Network::UDPMessage::HEADER | Network::UDPMessage::ACK |
                                        Network::UDPMessage::COMPRESSED,
                                    Network::SV_SNAPSHOT);
            msg.setFullAck(fullSnapshot);

            uint32_t nbEntity = 0;
            msg.appendData(nbEntity);
            msg.startCompressingSegment(false);
            for (auto [entity, currentNetSend] : currentNetSends) {
                if (!lastNetSends.contains(entity) || lastNetSends.get(entity) != currentNetSend) {
                    auto [bytes, comps] = getDeltaDiff(entity, current, last);
                    msg.appendData(uint32_t(entity));
                    for (auto &byte : bytes)
                        msg.appendData(byte);
                    for (auto &[typeId, comp] : comps) {
                        auto &type = getTypeindex(typeId);
                        msg.appendData(toVoid(type, comp), getComponentSize(type));
                    }
                    nbEntity++;
                }
            }
            for (auto [entity, lastNetSend] : lastNetSends) {
                if (!currentNetSends.contains(entity)) {
                    auto [bytes, comps] = getDeltaDiff(entity, current, last);
                    msg.appendData(uint32_t(entity));
                    for (auto &byte : bytes)
                        msg.appendData(byte);
                    for (auto &[typeId, comp] : comps) {
                        auto &type = getTypeindex(typeId);
                        msg.appendData(toVoid(type, comp), getComponentSize(type));
                    }
                    nbEntity++;
                }
            } // TODO cleaner
            msg.stopCompressingSegment(false);
            msg.writeData(nbEntity, sizeof(Network::UDPG_NetChannelHeader), 0, false);
            std::cout << "SEND:" << msg.getSize() << std::endl;
            if (!server.isRunning())
                continue;
            client.getNet()->pushData(msg, true);
        } catch (std::bad_any_cast &e) {
            continue;
        }
    }
}

std::pair<std::vector<uint8_t>, std::map<ecs::component::ComponentTools::component_id_t, const std::any>>
Snapshot::getDeltaDiff(ecs::entity::Entity entity, const snapshot_t &snap1, const snapshot_t &snap2) const {
    std::map<ecs::component::ComponentTools::component_id_t, const std::any> comps;
    int nbComp = snap2.size();
    std::vector<uint8_t> bits(nbComp * 2 / 8 + (nbComp * 2 % 8 ? 1 : 0), 0);

    for (auto &[type, anySparse] : snap1) {
        if (type == std::type_index(typeid(component::NetSend)) || !snap2.contains(type))
            continue;
        auto opt = compareComponentsEntity(entity, type, anySparse, snap2.find(type)->second);
        if (!opt.has_value())
            continue;
        auto &[e, t, set, any] = opt.value();
        auto id = getComponentId(type);
        if (set) {
            bits[id / 8] |= (1 << id % 8);
            comps.emplace(id, any);
        } else
            bits[(id + nbComp) / 8] |= (1 << (id + nbComp) % 8);
    }
    return std::make_pair(bits, comps);
}
} // namespace gengine::interface::network::system
