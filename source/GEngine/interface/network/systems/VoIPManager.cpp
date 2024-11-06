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

#include <iostream>

namespace gengine::interface::network::system {

void VoIPManager::init(void) {
    subscribeToEvent<gengine::system::event::MainLoop>(&VoIPManager::onMainLoop);
}

void VoIPManager::onMainLoop(gengine::system::event::MainLoop &) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();

    for (auto &[remote, client] : clientsSys.getClients()) {
        if (client.shouldDelete())
            continue;

        Network::UDPMessage msg(Network::UDPMessage::HEADER, Network::CL_VOIP);
        size_t readCount = 0;
        if (!client.getNet()->popIncommingData(msg, readCount))
            continue;

        std::array<uint8_t, 1400> buffer;
        size_t bufferSize = msg.getSize() - readCount;
        msg.readData(buffer.data(), readCount, bufferSize);

        Network::UDPG_VoIPSegment segment = {.size = static_cast<uint16_t>(bufferSize)};
        std::memcpy(&segment.playerIndex1, remote.as_bytes().data(), 8);
        std::memcpy(&segment.playerIndex2, remote.as_bytes().data() + 8, 8);

        Network::UDPMessage msgSend(Network::UDPMessage::HEADER, Network::SV_VOIP);
        msgSend.appendData(segment);
        msgSend.appendData((const void *)buffer.data(), bufferSize);

        for (auto &[remoteDest, clientDest] : clientsSys.getClients()) {
            if (clientDest.shouldDelete() || remote == remoteDest) // Skip sending to the source client
                continue;

            clientDest.getNet()->pushData(msgSend, false);
        }
    }
}

} // namespace gengine::interface::network::system
