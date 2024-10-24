/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** Updater.hpp
*/

#pragma once

#include <array>
#include <functional>

#include "GEngine/BaseEngine.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/interface/network/components/NetSend.hpp"
#include "GEngine/net/msg.hpp"


namespace gengine::interface::network::system {

class Updater : public System<Updater, component::NetSend> {
public:
    Updater(const BaseEngine::world_t &currentWorld);
    void init(void) override;
    // void onStartEngine(gengine::system::event::StartEngine &);
    void onGameLoop(gengine::system::event::GameLoop &);

private:
    // std::vector<std::type_index> m_typeTable; // TODO opti
    const BaseEngine::world_t &m_localWorld;
    void handleSnapshotMsg(Network::UDPMessage &msg, size_t readCount);
};
} // namespace gengine::interface::network::system