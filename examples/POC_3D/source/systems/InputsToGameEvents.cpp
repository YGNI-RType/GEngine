/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsToGameEvents.cpp
*/

#include "systems/InputsToGameEvents.hpp"

namespace rtype::system {
void InputsToGameEvents::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&InputsToGameEvents::sendEvents);
    subscribeToEvent<geg::event::io::KeyWEvent>(&InputsToGameEvents::moveFwd);
    subscribeToEvent<geg::event::io::KeyAEvent>(&InputsToGameEvents::moveLeft);
    subscribeToEvent<geg::event::io::KeySEvent>(&InputsToGameEvents::moveBck);
    subscribeToEvent<geg::event::io::KeyDEvent>(&InputsToGameEvents::moveRight);
}

void InputsToGameEvents::sendEvents(gengine::system::event::GameLoop &e) {
    publishEvent<event::Movement>(getMovementState());
}

void InputsToGameEvents::moveFwd(geg::event::io::KeyWEvent &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::FWD, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::FWD, false);
        break;
    default:
        break;
    }
}
void InputsToGameEvents::moveLeft(geg::event::io::KeyAEvent &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::LEFT, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::LEFT, false);
        break;
    default:
        break;
    }
}
void InputsToGameEvents::moveBck(geg::event::io::KeySEvent &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::BCK, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::BCK, false);
        break;
    default:
        break;
    }
}
void InputsToGameEvents::moveRight(geg::event::io::KeyDEvent &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::RIGHT, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::RIGHT, false);
        break;
    default:
        break;
    }
}

void InputsToGameEvents::updateDirectionBitmask(event::Movement::State state, bool pressed) {
    char mask = 0;

    switch (state) {
    case event::Movement::FWD:
        mask = FWD_MASK;
        break;
    case event::Movement::RIGHT:
        mask = RIGHT_MASK;
        break;
    case event::Movement::BCK:
        mask = BCK_MASK;
        break;
    case event::Movement::LEFT:
        mask = LEFT_MASK;
        break;
    default:
        return;
    }

    if (pressed)
        m_directionBitmask |= mask;
    else
        m_directionBitmask &= ~mask;
}

event::Movement::State InputsToGameEvents::getMovementState(void) {
    switch (m_directionBitmask) {
    case FWD_MASK:
        return event::Movement::FWD;
    case BCK_MASK:
        return event::Movement::BCK;
    case LEFT_MASK:
        return event::Movement::LEFT;
    case RIGHT_MASK:
        return event::Movement::RIGHT;
    case FWD_MASK | LEFT_MASK:
        return event::Movement::FWD_LEFT;
    case FWD_MASK | RIGHT_MASK:
        return event::Movement::FWD_RIGHT;
    case BCK_MASK | LEFT_MASK:
        return event::Movement::BCK_LEFT;
    case BCK_MASK | RIGHT_MASK:
        return event::Movement::BCK_RIGHT;
    default:
        return event::Movement::STANDING;
    }
}
} // namespace rtype::system