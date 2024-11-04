/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Event
** ════════════════════════════════════════════════════════════════════════════
**  File        : Mouse.hpp
**  Create at   : 2024-10-15 05:13
**  Author      : AUTHOR
**  Description : These events transmit information related to the mouse,
                    they are emitted by the MouseCatcher system dedicated to
                    the DriverEngine.
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/driver/input/State.hpp"
#include "GEngine/libdev/tools/Math.hpp"

namespace gengine::system::event::driver::input {
class MouseButton : public Event {
public:
    MouseButton(state_t state, const Vect2 &cursorPos)
        : state(state)
        , cursorPos(cursorPos) {
    }
    MouseButton(state_t state, const Vect2 &&cursorPos)
        : state(state)
        , cursorPos(cursorPos) {
    }
    state_t state;
    Vect2 cursorPos;
};

class MouseMove : public Event {
public:
    MouseMove(const Vect2 &cursorPos)
        : cursorPos(cursorPos) {
    }
    MouseMove(const Vect2 &&cursorPos)
        : cursorPos(cursorPos) {
    }
    MouseMove(const Vect2 &cursorPos, const Vect2 &delta)
        : cursorPos(cursorPos)
        , delta(delta) {
    }
    MouseMove(const Vect2 &&cursorPos, const Vect2 &&delta)
        : cursorPos(cursorPos)
        , delta(delta) {
    }
    Vect2 cursorPos;
    Vect2 delta;
};

class MouseLeft : public MouseButton {
public:
    MouseLeft(state_t state, const Vect2 &cursorPos)
        : MouseButton(state, cursorPos) {
    }
    MouseLeft(state_t state, const Vect2 &&cursorPos)
        : MouseButton(state, cursorPos) {
    }
};
class MouseRight : public MouseButton {
public:
    MouseRight(state_t state, const Vect2 &cursorPos)
        : MouseButton(state, cursorPos) {
    }
    MouseRight(state_t state, const Vect2 &&cursorPos)
        : MouseButton(state, cursorPos) {
    }
};
class MouseMiddle : public MouseButton {
public:
    MouseMiddle(state_t state, const Vect2 &cursorPos)
        : MouseButton(state, cursorPos) {
    }
    MouseMiddle(state_t state, const Vect2 &&cursorPos)
        : MouseButton(state, cursorPos) {
    }
};
} // namespace gengine::system::event::driver::input
