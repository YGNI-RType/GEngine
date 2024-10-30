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
    subscribeToEvent<gengine::system::event::GameLoop>(&VoIPManager::onGameLoop);
}

void VoIPManager::onGameLoop(gengine::system::event::GameLoop &) {
    auto &clientsSys = getSystem<gengine::interface::network::system::ServerClientsHandler>();
    std::vector<Network::UDPMessage> udpMsgs;
    Network::UDPMessage msgSend(Network::UDPMessage::HEADER, Network::SV_VOIP);
    const size_t untouchedMsgSz = msgSend.getSize();

    for (auto &[remote, client] : clientsSys.getClients()) {
        size_t readCount = 0;
        if (client.shouldDelete())
            continue;

        Network::UDPMessage msg(Network::UDPMessage::HEADER, Network::CL_VOIP);
        if (!client.getNet()->popIncommingData(msg, readCount))
            continue;

        /*** TODO (if time) for chat proximity, the hook is called here to tell if it should send, if it's done i have
         * to recode for each client the diffrent messages ****/

        std::array<uint8_t, 1400> buffer;
        size_t bufferSize = msg.getSize() - readCount;
        msg.readData(buffer.data(), readCount, bufferSize);

        if (msgSend.getSize() + sizeof(Network::UDPG_VoIPSegment) + bufferSize > 1400) { /* this shit is too big, sending the actual n=message now */
            udpMsgs.push_back(msgSend);
            msgSend = Network::UDPMessage(Network::UDPMessage::HEADER, Network::SV_VOIP);
        }
        Network::UDPG_VoIPSegment segment = {.size = static_cast<uint16_t>(bufferSize)};
        std::memcpy(&segment.playerIndex, remote.getUUIDBytes().as_bytes().data(), 16);

        msgSend.appendData(segment);
        msgSend.appendData((const void *)buffer.data(), bufferSize);
    }

    for (auto &[remote, client] : clientsSys.getClients()) {
        if (client.shouldDelete())
            continue;

        if (untouchedMsgSz < msgSend.getSize())
            client.getNet()->pushData(msgSend, false);
        for (const Network::UDPMessage &message : udpMsgs) {
            if (untouchedMsgSz == msgSend.getSize())
                continue;
            client.getNet()->pushData(message, false);
        }
    }
}

} // namespace gengine::interface::network::system