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
    createSnapshots();
    getAndSendDeltaDiff();
}

void Snapshot::registerSnapshot(gengine::interface::event::NewRemoteLocal &e) {
    interface::component::RemoteLocal r(e.remote);
    m_clientSnapshots.insert(std::make_pair(r, std::make_pair(m_currentSnapshotId, snapshots_t())));
    m_clientSnapshots[r].second[m_currentSnapshotId % MAX_SNAPSHOT] = m_dummySnapshot;
}

void Snapshot::destroySnapshot(gengine::interface::event::DeleteRemoteLocal &e) {
    m_clientSnapshots.erase(e.remote);
}

void Snapshot::createSnapshots(void) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    for (auto &[remote, client] : clientsSys.getClients()) {
        if (client.shouldDelete())
            continue;
        auto it = m_clientSnapshots.find(remote);
        if (it == m_clientSnapshots.end())
            continue;
        it->second.second[m_currentSnapshotId % MAX_SNAPSHOT] = m_currentWorld;
    }
}
void Snapshot::getAndSendDeltaDiff(void) {
    auto &server = Network::NET::getServer();
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    for (auto &[remote, client] : clientsSys.getClients()) {
        if (client.shouldDelete() || !client.isReady())
            continue;
        auto it = m_clientSnapshots.find(remote);
        if (it == m_clientSnapshots.end())
            continue;

        auto &[firstSnapshotId, snapshots] = it->second;
        auto lastReceived = client.getLastAck();
        auto lastId = firstSnapshotId + lastReceived;
        auto diff = m_currentSnapshotId - lastId;
        // std::cout << "client : " << remote.getUUIDString() << " | diff: " << diff << " | m_currentSnapshotId: " <<
        // m_currentSnapshotId << " last id: " << lastId
        // << " UDP Last ACK: " << lastReceived << std::endl;

        auto &current = snapshots[m_currentSnapshotId % MAX_SNAPSHOT];
        auto &last = diff > MAX_SNAPSHOT ? m_dummySnapshot : snapshots[lastId % MAX_SNAPSHOT];

        auto &currentNetSends = getComponents<component::NetSend>();
        auto &lastNetSends = std::any_cast<ecs::component::SparseArray<component::NetSend> &>(
            last[std::type_index(typeid(component::NetSend))]);

        auto &remotes = getComponents<interface::component::RemoteLocal>();
        for (auto [e, r, netsend] : Zip(remotes, currentNetSends))
            netsend.update();

        try {
            Network::UDPMessage msg(Network::UDPMessage::HEADER | Network::UDPMessage::ACK |
                                        Network::UDPMessage::COMPRESSED,
                                    Network::SV_SNAPSHOT);
            uint32_t nbEntity = 0;
            msg.appendData(nbEntity);
            size_t realSize = msg.getSize();
            msg.startCompressingSegment(false);
            for (auto [entity, currentNetSend] : currentNetSends) {
                if (!lastNetSends.contains(entity) || lastNetSends.get(entity) != currentNetSend) {
                    auto [bytes, comps] = getDeltaDiff(entity, current, last);
                    msg.appendData(uint32_t(entity), &realSize);
                    for (auto &byte : bytes)
                        msg.appendData(byte, &realSize);
                    for (auto &[typeId, comp] : comps) {
                        auto &type = getTypeindex(typeId);
                        msg.appendData(toVoid(type, comp), getComponentSize(type), &realSize);
                    }
                    nbEntity++;
                }
            }
            for (auto [entity, lastNetSend] : lastNetSends) {
                if (!currentNetSends.contains(entity)) {
                    auto [bytes, comps] = getDeltaDiff(entity, current, last);
                    msg.appendData(uint32_t(entity), &realSize);
                    for (auto &byte : bytes)
                        msg.appendData(byte, &realSize);
                    for (auto &[typeId, comp] : comps) {
                        auto &type = getTypeindex(typeId);
                        msg.appendData(toVoid(type, comp), getComponentSize(type), &realSize);
                    }
                    nbEntity++;
                }
            } // TODO cleaner
            msg.stopCompressingSegment(false);
            msg.writeData(nbEntity, sizeof(Network::UDPG_NetChannelHeader), 0, false);

            if (!server.isRunning())
                continue;
            client.getNet()->pushData(msg, true);
        } catch (const Network::MsgError &e) {
            std::cerr << e.what() << std::endl;

            /* TODO : do something when something happens... */
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
