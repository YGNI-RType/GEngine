/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** Systems.hpp
*/

#pragma once

#include "GEngine/libdev/systems/AutoKiller.hpp"
#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/Logger.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/Motions.hpp"

/* TODO: Thomas: these includes should never exists, it should be the developer to get them individually */
namespace gengine::system {
class CLI;
}

namespace geg::system {
using AutoKiller = gengine::system::AutoKiller;

using Collision2D = gengine::system::Collision2D;
using Collision2D = gengine::system::Collision2D;

using Logger = gengine::system::Logger;

using AutoMainLoop = gengine::system::AutoMainLoop;

using Motion2D = gengine::system::Motion2D;
using Motion3D = gengine::system::Motion3D;

using CLI = gengine::system::CLI;

using MotionAcceleration2D = gengine::system::MotionAcceleration2D;
using MotionAcceleration3D = gengine::system::MotionAcceleration3D;
} // namespace geg::system

#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/input/MouseCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/AudioManager.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/FontManager.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

namespace geg::system::io {
using KeyboardCatcher = gengine::system::driver::input::KeyboardCatcher;

using MouseCatcher = gengine::system::driver::input::MouseCatcher;

using AnimationManager = gengine::system::driver::output::AnimationManager;
using Animate = gengine::system::driver::output::Animate;
using AnimateModel = gengine::system::driver::output::AnimateModel;

using Draw = gengine::system::driver::output::Draw;
using DrawSprite = gengine::system::driver::output::DrawSprite;
using DrawText = gengine::system::driver::output::DrawText;
using DrawRectangle = gengine::system::driver::output::DrawRectangle;
using DrawCircle = gengine::system::driver::output::DrawCircle;

using FontManager = gengine::system::driver::output::FontManager;

using RenderWindow = gengine::system::driver::output::RenderWindow;

using AudioManager = gengine::system::driver::output::AudioManager;

using TextureManager = gengine::system::driver::output::TextureManager;
}; // namespace geg::system::io
