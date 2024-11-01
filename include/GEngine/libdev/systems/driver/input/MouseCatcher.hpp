/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : MouseCatcher.hpp
**  Create at   : 2024-10-15 04:59
**  Author      : AUTHOR
**  Description : This system is dedicated to the DriverEngine, it must with
                    the raylib publish all the events linked mouse.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "module/raylib_safe.h"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/RenderLoop.hpp"
#include "GEngine/libdev/systems/events/driver/input/Mouse.hpp"
#include "GEngine/libdev/systems/events/driver/output/Window.hpp"
#include "GEngine/libdev/tools/Math.hpp"

namespace gengine::system::driver::input {
using InputState = gengine::system::event::driver::input::state_t;
class MouseCatcher : public gengine::System<MouseCatcher>, public LocalSystem {
public:
    void init(void) override;

    void onWindowResized(gengine::system::event::WindowResized &e);
    void onRenderLoop(gengine::system::event::RenderLoop &e);

private:
    Vect2 m_prevMousePos = {0, 0};
    void processMouseInput(int button, InputState state, Vect2 mousePos);

    Vect2 m_ratio = {1, 1};
};

MouseButton &operator++(MouseButton &key);

using MouseMoveEvent = gengine::system::event::driver::input::MouseMove;
using MouseButtonEvent = gengine::system::event::driver::input::MouseButton;
using MouseLeftEvent = gengine::system::event::driver::input::MouseLeft;
using MouseRightEvent = gengine::system::event::driver::input::MouseRight;
using MouseMiddleEvent = gengine::system::event::driver::input::MouseMiddle;
} // namespace gengine::system::driver::input
