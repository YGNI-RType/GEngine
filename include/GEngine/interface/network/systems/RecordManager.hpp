/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RecordManager
*/

#pragma once

#include "GEngine/BaseEngine.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/network/systems/ServerClient.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"

namespace gengine::interface::network::system {

class RecordManager : public System<RecordManager>,
                 public LocalSystem {

public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);

    void toggleCapture(gengine::system::driver::input::KeyPEvent &e);
private:
    bool m_started = false;
};

} // namespace gengine::interface::network::system
