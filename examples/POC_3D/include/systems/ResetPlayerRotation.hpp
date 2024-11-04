/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ResetPlayerRotation.hpp
*/

#pragma once

#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "components/Player.hpp"

#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"

#include "events/ResetPlayerRotationEvent.hpp"

namespace poc3d::system {
class ResetPlayerRotation : public gengine::System<ResetPlayerRotation, gengine::interface::component::RemoteLocal,
                                                   gengine::component::driver::output::Model, component::Player,
                                                   geg::component::Transform3D> {
public:
    void init(void) override;
    void resetPlayerRotation(gengine::interface::event::SharedEvent<event::ResetPlayerRotationEvent> &e);
};
} // namespace poc3d::system
