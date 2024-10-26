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
    void init(void) override;

    void onGetRemoteLocalWhoIAm(event::SharedEvent<event::GetRemoteLocalWhoIAm> &e);
};

class HandleLocal : public gengine::System<HandleLocal, component::RemoteLocal>, public gengine::LocalSystem {
public:
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &e [[maybe_unused]]);
    void onGameLoop(geg::event::GameLoop &e [[maybe_unused]]);

private:
    uuids::uuid m_whoIAm;
    bool m_stop = false;
};
} // namespace gengine::interface::system