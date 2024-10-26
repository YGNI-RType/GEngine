/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RemoteLocal.hpp
*/

#pragma once

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/System.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"

#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

namespace gengine::interface::system {
class HandleRemoteLocal : public gengine::System<HandleRemoteLocal, component::RemoteLocal>,
                          public gengine::RemoteSystem {
public:
    void onGetRemoteLocalWhoIAm(event::SharedEvent<event::GetRemoteLocalWhoIAm> &e) {
        auto &remotes = getComponents<component::RemoteLocal>();

        for (auto &[entity, remote] : remotes) {
            if (remote.getUUIDBytes() == e.remoteUUID) {
                remote.setWhoIAm(e->localUUID);
                return;
            }
        }
    }
    void init(void) override {
        subscribeToEvent<event::SharedEvent<event::GetRemoteLocalWhoIAm>>(&HandleRemoteLocal::onGetRemoteLocalWhoIAm);
    }
};

class HandleLocal : public gengine::System<HandleLocal, component::RemoteLocal>, public gengine::LocalSystem {
public:
    void init(void) override {
        subscribeToEvent<gengine::system::event::StartEngine>(&HandleLocal::onStartEngine);
        subscribeToEvent<geg::event::GameLoop>(&HandleLocal::onGameLoop);
    }
    void onStartEngine(gengine::system::event::StartEngine &e [[maybe_unused]]) {
        component::RemoteLocal::generateUUID(m_whoIAm);
        publishEvent(event::GetRemoteLocalWhoIAm(m_whoIAm));
    }

    void onGameLoop(geg::event::GameLoop &e [[maybe_unused]]) {
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

private:
    uuids::uuid m_whoIAm;
    bool m_stop = false;
};
} // namespace gengine::interface::system