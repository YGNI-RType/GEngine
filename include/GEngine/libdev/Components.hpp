/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** Components.hpp
*/

#pragma once

#include "GEngine/libdev/components/Accelerations.hpp"
#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/SpanLife.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/Box.hpp"

namespace geg::component {
using HitBoxSquare2D = gengine::component::HitBoxSquare2D;
using HitBoxSquare3D = gengine::component::HitBoxSquare3D;
using HitBoxCircle2D = gengine::component::HitBoxCircle2D;
using HitBoxCircle3D = gengine::component::HitBoxCircle2D;

using SpanLife = gengine::component::SpanLife;

using Transform2D = gengine::component::Transform2D;
using Transform3D = gengine::component::Transform3D;

using Velocity2D = gengine::component::Velocity2D;
using Velocity3D = gengine::component::Velocity3D;
using Acceleration2D = gengine::component::Acceleration2D;
using Acceleration3D = gengine::component::Acceleration3D;

template <size_t Size>
using Box = gengine::component::Box<Size>;
} // namespace geg::component

#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Model.hpp"
#include "GEngine/libdev/components/driver/output/RaylibTypes.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"
#include "GEngine/libdev/components/driver/output/Text.hpp"

namespace geg::component::io {
using AnimationTrack = gengine::component::driver::output::AnimationTrack;
using Animation = gengine::component::driver::output::Animation;

using Drawable = gengine::component::driver::output::Drawable;
using Drawable = gengine::component::driver::output::Drawable;

using Circle = gengine::component::driver::output::Circle;
using Rectangle = gengine::component::driver::output::Rectangle;

using Sprite = gengine::component::driver::output::Sprite;
using Model = gengine::component::driver::output::Model;

using Text = gengine::component::driver::output::Text;
} // namespace geg::component::io

#include "GEngine/libdev/components/gui/Button.hpp"
#include "GEngine/libdev/components/gui/SceneMember.hpp"
namespace geg::component::gui {
    using SceneMember = gengine::component::gui::SceneMember;
}

#include "GEngine/interface/network/components/NetSend.hpp"

namespace geg::component::network {
using NetSend = gengine::interface::network::component::NetSend;
} // namespace geg::component::network
