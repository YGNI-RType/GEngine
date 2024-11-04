/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Guess.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/driver/output/Model.hpp"
#include "components/Player.hpp"

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"

#include "GEngine/interface/events/SharedEvent.hpp"

#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/ModelManager.hpp"

#include "events/GuessEvent.hpp"

namespace poc3d::system {
class Guess
    : public gengine::System<Guess, gengine::interface::component::RemoteLocal,
                             gengine::component::driver::output::Model, geg::component::Transform3D, component::Player,
                             gengine::system::driver::output::DrawModel, gengine::system::driver::output::ModelManager>,
      public gengine::RemoteSystem {
public:
    void init(void) override;

    void guessWho(gengine::interface::event::SharedEvent<event::GuessEvent> &e);
};
} // namespace poc3d::system
