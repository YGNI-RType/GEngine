/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MouseCatcher.cpp
*/

#include "GEngine/libdev/systems/driver/input/MouseCatcher.hpp"
#include <iostream>

#include "GEngine/libdev/components/driver/output/RaylibTypes.hpp"

namespace gengine::system::driver::input {
void MouseCatcher::init(void) {
    subscribeToEvent<gengine::system::event::RenderLoop>(&MouseCatcher::onRenderLoop);
    subscribeToEvent<gengine::system::event::WindowResized>(&MouseCatcher::onWindowResized);
}

void MouseCatcher::onWindowResized(gengine::system::event::WindowResized &e) {
    m_ratio = e.ratio;
}

void MouseCatcher::onRenderLoop(gengine::system::event::RenderLoop &e) {
    Vect2 mousePos = gengine::component::driver::output::toVect2(GetMousePosition());
    mousePos.x /= m_ratio.x;
    mousePos.y /= m_ratio.y;
    MouseButton button = MOUSE_BUTTON_LEFT;
    do {
        if (IsMouseButtonReleased(button))
            processMouseInput(button, InputState::RELEASE, mousePos);
        if (IsMouseButtonPressed(button))
            processMouseInput(button, InputState::PRESSED, mousePos);
        if (IsMouseButtonDown(button))
            processMouseInput(button, InputState::DOWN, mousePos);
        ++button;
    } while (button != MOUSE_BUTTON_LEFT);
    if (mousePos != m_prevMousePos) {
        publishEvent(MouseMoveEvent(mousePos));
        m_prevMousePos = mousePos;
    }
}

void MouseCatcher::processMouseInput(int button, InputState state, Vect2 mousePos) {
    switch (button) {
    case MOUSE_BUTTON_LEFT:
        publishEvent(MouseLeftEvent(state, mousePos));
        break;
    case MOUSE_BUTTON_RIGHT:
        publishEvent(MouseRightEvent(state, mousePos));
        break;
    case MOUSE_BUTTON_MIDDLE:
        publishEvent(MouseMiddleEvent(state, mousePos));
        break;
    default:
        break;
    }
}

MouseButton &operator++(MouseButton &key) {
    key = key < MOUSE_BUTTON_MIDDLE ? static_cast<MouseButton>(static_cast<int>(key) + 1) : MOUSE_BUTTON_LEFT;
    return key;
}
} // namespace gengine::system::driver::input
