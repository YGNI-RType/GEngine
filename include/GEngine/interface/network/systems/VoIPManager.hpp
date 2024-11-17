/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPManager
*/

#pragma once

#include "GEngine/BaseEngine.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/Components.hpp"

namespace gengine::interface::network::system {

class VoIPManager : public System<VoIPManager, interface::component::RemoteLocal,
                                  gengine::interface::network::system::ServerClientsHandler,
                                  geg::component::Transform3D, gengine::interface::component::RemoteLocal>,
                    public RemoteSystem {

public:
    VoIPManager(float distance = UNDEFINED_DISTANCE);
    void init(void) override;

private:
    static constexpr float UNDEFINED_DISTANCE = 0;

    float m_distance;
    float calculateDistance(uuids::uuid c1, uuids::uuid c2);
    void onMainLoop(gengine::system::event::MainLoop &);
};

} // namespace gengine::interface::network::system