/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** CameraRotation.hpp
*/

#pragma once

#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Model.hpp"

#include "components/Player.hpp"

#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"

#include "events/Movement.hpp"
#include "events/ResetPlayerRotationCameraEvent.hpp"
#include "events/Rotation.hpp"

#include "GEngine/libdev/systems/driver/output/Draw.hpp"

#include "Constants.hpp"

namespace poc3d::system {
class CameraRotation
    : public gengine::System<CameraRotation, gengine::interface::component::RemoteLocal, gengine::component::Velocity3D,
                             gengine::component::driver::output::Model, component::Player, geg::component::Transform3D,
                             gengine::system::driver::output::DrawModel>,
      public gengine::LocalSystem {
public:
    void init(void) override;
    void onStart(gengine::system::event::StartEngine &e);
    void onMovement(gengine::interface::event::SharedEvent<event::Movement> &e);
    void rotateCamera(event::Rotation &e);
    void resetPlayerRotation(event::ResetPlayerRotationCameraEvent &e);

private:
    void moveCamera(geg::component::Transform3D &transform, gengine::Vect3 &forward);
    gengine::Entity m_cameraId;
};
} // namespace poc3d::system
