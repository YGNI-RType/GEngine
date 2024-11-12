/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.hpp
*/

#pragma once

#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Model.hpp"
#include "components/Player.hpp"

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"

#include "GEngine/interface/events/SharedEvent.hpp"

#include "events/ChangeCameraMode.hpp"
#include "events/Crouch.hpp"
#include "events/Jump.hpp"
#include "events/LockPlayerEvent.hpp"
#include "events/Movement.hpp"
#include "events/Rotation.hpp"
#include "events/Sprint.hpp"

#include "GEngine/libdev/systems/driver/output/Draw.hpp"

namespace poc3d::system {
class PlayerMotion
    : public gengine::System<PlayerMotion, gengine::interface::component::RemoteLocal, gengine::component::Velocity3D,
                             gengine::component::driver::output::Model, geg::component::Transform3D,
                             geg::component::Acceleration3D, geg::component::io::Animation, component::Player,
                             gengine::system::driver::output::DrawModel> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void movePlayer(gengine::interface::event::SharedEvent<event::Movement> &e);
    void rotatePlayer(gengine::interface::event::SharedEvent<event::Rotation> &e);
    void jumpPlayer(gengine::interface::event::SharedEvent<event::Jump> &e);
    void changeCameraModePlayer(gengine::interface::event::SharedEvent<event::ChangeCameraMode> &e);
    void sprintPlayer(gengine::interface::event::SharedEvent<event::Sprint> &e);
    void crouchPlayer(gengine::interface::event::SharedEvent<event::Crouch> &e);
    void lockPlayer(gengine::interface::event::SharedEvent<event::LockPlayerEvent> &e);
};
} // namespace poc3d::system
