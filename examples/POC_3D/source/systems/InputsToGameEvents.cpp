/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsToGameEvents.cpp
*/

#include "systems/InputsToGameEvents.hpp"
#include "Constants.hpp"

namespace poc3d::system {
void InputsToGameEvents::init(void) {
    // Send movements + rotation
    subscribeToEvent<gengine::system::event::GameLoop>(&InputsToGameEvents::sendEvents);
    subscribeToEvent<geg::event::io::MouseMoveEvent>(&InputsToGameEvents::rotatePlayer);
    subscribeToEvent<geg::event::io::KeyF5Event>(&InputsToGameEvents::changeCameraMode);
    // Movements
    subscribeToEvent<geg::event::io::KeyWEvent>(&InputsToGameEvents::moveFwd);
    subscribeToEvent<geg::event::io::KeyAEvent>(&InputsToGameEvents::moveLeft);
    subscribeToEvent<geg::event::io::KeySEvent>(&InputsToGameEvents::moveBck);
    subscribeToEvent<geg::event::io::KeyDEvent>(&InputsToGameEvents::moveRight);
    subscribeToEvent<geg::event::io::KeySpaceEvent>(&InputsToGameEvents::jump);
    subscribeToEvent<geg::event::io::KeyLeftControlEvent>(&InputsToGameEvents::sprint);

    subscribeToEvent<geg::event::io::MouseLeft>(&InputsToGameEvents::guess);
    subscribeToEvent<geg::event::io::KeyEEvent>(&InputsToGameEvents::morph);
    subscribeToEvent<geg::event::io::KeyLeftShiftEvent>(&InputsToGameEvents::lockPlayer);

    subscribeToEvent<geg::event::io::KeyOneEvent>(&InputsToGameEvents::setPlayerModelCoraline);
    subscribeToEvent<geg::event::io::KeyTwoEvent>(&InputsToGameEvents::setPlayerModelHunter);
    subscribeToEvent<geg::event::io::KeyThreeEvent>(&InputsToGameEvents::setPlayerModelChair);
    subscribeToEvent<geg::event::io::KeyFourEvent>(&InputsToGameEvents::setPlayerModelLamp);
    subscribeToEvent<geg::event::io::KeyFiveEvent>(&InputsToGameEvents::setPlayerModelStick);
}

void InputsToGameEvents::sendEvents(gengine::system::event::GameLoop &e) {
    publishEvent(event::Movement(getMovementState()));
}

void InputsToGameEvents::rotatePlayer(geg::event::io::MouseMoveEvent &e) {
    gengine::Vect3 rotation = {0, 0, 0};
    rotation.y -= MOUSE_SENSITIVITY * e.delta.x;
    rotation.z -= MOUSE_SENSITIVITY * e.delta.y;
    publishEvent(event::Rotation(rotation));
}

void InputsToGameEvents::changeCameraMode(geg::event::io::KeyF5Event &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangeCameraMode());
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

void InputsToGameEvents::jump(geg::event::io::KeySpaceEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED || e.state == geg::event::io::InputState::DOWN)
        publishEvent(event::Jump(0.12));
}

void InputsToGameEvents::sprint(geg::event::io::KeyLeftControlEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED || e.state == geg::event::io::InputState::DOWN)
        publishEvent(event::Sprint(true));
    else
        publishEvent(event::Sprint(false));
}

void InputsToGameEvents::guess(geg::event::io::MouseLeft &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::GuessEvent(0.4));
}

void InputsToGameEvents::morph(geg::event::io::KeyEEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::MorphToPropEvent());
}

void InputsToGameEvents::lockPlayer(geg::event::io::KeyLeftShiftEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::LockPlayerEvent(true));
    if (e.state == geg::event::io::InputState::RELEASE) {
        publishEvent(event::LockPlayerEvent(false));
        publishEvent(event::ResetPlayerRotationCameraEvent());
    }
}

void InputsToGameEvents::setPlayerModelCoraline(geg::event::io::KeyOneEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("player/", "player.json/idle", 0.04f));
}

void InputsToGameEvents::setPlayerModelHunter(geg::event::io::KeyTwoEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/incendie.glb"));
}

void InputsToGameEvents::setPlayerModelChair(geg::event::io::KeyThreeEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/wooden_cabinet.glb"));
}

void InputsToGameEvents::setPlayerModelLamp(geg::event::io::KeyFourEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/desk_lamp.glb"));
}

void InputsToGameEvents::setPlayerModelStick(geg::event::io::KeyFiveEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/stick.glb"));
}

void InputsToGameEvents::setPlayerModelTv(geg::event::io::KeySixEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/tv_stand.glb"));
}

void InputsToGameEvents::setPlayerModelTable(geg::event::io::KeySevenEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/nintendo_switch_diorama.glb"));
}

void InputsToGameEvents::setPlayerModelCouch(geg::event::io::KeyEightEvent &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(event::ChangePlayerModelEvent("props/lowpoly_people.glb"));
}

} // namespace poc3d::system
