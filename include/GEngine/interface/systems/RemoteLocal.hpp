/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RemoteLocal.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"

#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/Events.hpp"

namespace gengine::interface::system {
class HandleRemoteLocal: public gengine::System<HandleRemoteLocal, gengine::interface::component::RemoteLocal>, public gengine::RemoteSystem {
public:

    void onGetRemoteLocalWhoIAm(gengine::interface::event::SharedEvent<gengine::interface::event::GetRemoteLocalWhoIAm> &e) {
        auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

        for (auto &[entity, remote]: remotes) {
            if (remote == e.remote) {
                remote.setWhoIAm(e->remote.getUUIDBytes());
                std::cout << uuids::to_string(remote.getWhoIAm()) << std::endl;
                return;
            }
        }
    }
    void init(void) override {
        subscribeToEvent<gengine::interface::event::SharedEvent<gengine::interface::event::GetRemoteLocalWhoIAm>>(&HandleRemoteLocal::onGetRemoteLocalWhoIAm);
    }
};

class HandleLocal: public gengine::System<HandleLocal, gengine::interface::component::RemoteLocal>, public gengine::LocalSystem {
public:

    void onStartEngine(gengine::system::event::StartEngine &e[[maybe_unused]]) {
        publishEvent(gengine::interface::event::GetRemoteLocalWhoIAm(m_remoteId));
    }

    void onGameLoop(geg::event::GameLoop &e[[maybe_unused]]) {
        auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
        for (auto &[entity, remote]: remotes) {
            std::cout << uuids::to_string(remote.getWhoIAm()) << ":" << uuids::to_string(m_remoteId.getUUIDBytes()) << std::endl;
            // if (remote.getUUIDString() == m_remoteId.getWhoIAm().getUUIDBytes()) {
            //     std::cout << "i am: " << remote.getUUIDString() << std::endl;
            //     return;
            // }
            // return;
        }
        publishEvent(gengine::interface::event::GetRemoteLocalWhoIAm(m_remoteId));
    }
    void init(void) override {
        subscribeToEvent<gengine::system::event::StartEngine>(&HandleLocal::onStartEngine);
        subscribeToEvent<geg::event::GameLoop>(&HandleLocal::onGameLoop);
    }
private:
    gengine::interface::component::RemoteLocal m_remoteId;
};
}