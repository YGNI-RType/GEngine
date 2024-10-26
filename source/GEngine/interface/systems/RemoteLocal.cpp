/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** RemoteLocal.cpp
*/

#include "GEngine/interface/systems/RemoteLocal.hpp"

namespace gengine::interface::system {
void HandleRemoteLocal::onGetRemoteLocalWhoIAm(event::SharedEvent<event::GetRemoteLocalWhoIAm> &e) {
    auto &remotes = getComponents<component::RemoteLocal>();

    for (auto &[entity, remote] : remotes) {
        if (remote.getUUIDBytes() == e.remoteUUID) {
            remote.setWhoIAm(e->localUUID);
            return;
        }
    }
}
void HandleRemoteLocal::init(void) {
    subscribeToEvent<event::SharedEvent<event::GetRemoteLocalWhoIAm>>(&HandleRemoteLocal::onGetRemoteLocalWhoIAm);
}

void HandleLocal::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&HandleLocal::onStartEngine);
    subscribeToEvent<geg::event::GameLoop>(&HandleLocal::onGameLoop);
}
void HandleLocal::onStartEngine(gengine::system::event::StartEngine &e [[maybe_unused]]) {
    component::RemoteLocal::generateUUID(m_whoIAm);
    publishEvent(event::GetRemoteLocalWhoIAm(m_whoIAm));
}

void HandleLocal::onGameLoop(geg::event::GameLoop &e [[maybe_unused]]) {
    if (m_stop)
        return;
    auto &remotes = getComponents<component::RemoteLocal>();
    for (auto &[entity, remote] : remotes) {
        if (remote.getWhoIAm() == m_whoIAm) {
            m_whoIAm = remote.getUUIDBytes();

            std::cout << "me:" << uuids::to_string(m_whoIAm) << std::endl;
            publishEvent<event::ItsMe>(m_whoIAm);
            // TODO unsubscribe ??
            m_stop = true;
            return;
        }
    }
    publishEvent(event::GetRemoteLocalWhoIAm(m_whoIAm));
}
} // namespace gengine::interface::system