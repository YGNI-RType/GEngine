/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPManager
*/

#include "GEngine/interface/network/systems/VoIPManager.hpp"

#include "GEngine/net/msg.hpp"
#include "GEngine/net/net_client.hpp"
#include "GEngine/net/structs/msg_udp_structs.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

namespace gengine::interface::network::system {

VoIPManager::VoIPManager(float distance)
    : m_distance(distance) {
}

void VoIPManager::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&VoIPManager::onGameLoop);
}

// Fonction pour calculer la distance entre deux clients (exemple simple)
float VoIPManager::calculateDistance(uuids::uuid c1, uuids::uuid c2) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    auto &tranforms = getComponents<geg::component::Transform3D>();
    // Exemple de calcul de distance entre deux clients
    // Cela peut être remplacé par un calcul réel basé sur les positions des clients
    // par exemple client1.getPosition() et client2.getPosition()
    Vect3 pos1;
    Vect3 pos2;

    for (auto [entity, id, tranform] : gengine::Zip(remotes, tranforms)) {
        if (id.getUUIDBytes() == c1)
            pos1 = tranform.pos;
        if (id.getUUIDBytes() == c2)
            pos2 = tranform.pos;
    }
    return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.z - pos2.z, 2));
}

void VoIPManager::onGameLoop(gengine::system::event::GameLoop &) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();

    for (auto &[remote, client] : clientsSys.getClients()) {
        if (client.shouldDelete())
            continue;

        Network::UDPMessage msg(Network::UDPMessage::HEADER, Network::CL_VOIP);
        size_t readCount = 0;
        if (!client.getNet()->popIncommingData(msg, readCount))
            continue;

        std::vector<uint8_t> buffer(msg.getSize() - readCount);
        msg.readData(buffer.data(), readCount, buffer.size());

        for (auto &[remoteDest, clientDest] : clientsSys.getClients()) {
            if (clientDest.shouldDelete() || remote == remoteDest) // Ne pas envoyer au client lui-même
                continue;

            // Calculer la distance entre le client source et le client destination
            float distance = calculateDistance(remote, remoteDest);
            // float volume = 1.0f / (1.0f + distance); // Exemple de calcul du volume
            // Préparer le segment VoIP avec le volume
            Network::UDPG_VoIPSegment segment = {
                .size = static_cast<uint16_t>(buffer.size()),
                .volume =
                    static_cast<float>(m_distance ? std::clamp(m_distance - distance, 0.f, m_distance) / m_distance
                                                  : 1) // Ajouter le volume au segment
            };
            std::memcpy(&segment.playerIndex1, remote.as_bytes().data(), 8);
            std::memcpy(&segment.playerIndex2, remote.as_bytes().data() + 8, 8);

            // Créer le message à envoyer
            Network::UDPMessage msgSend(Network::UDPMessage::HEADER, Network::SV_VOIP);
            msgSend.appendData(segment);
            msgSend.appendData(buffer.data(), buffer.size());

            clientDest.getNet()->pushData(msgSend, false);
        }
    }
}

} // namespace gengine::interface::network::system
