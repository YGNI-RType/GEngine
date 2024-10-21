/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsToGameEvents.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"
#include "GEngine/libdev/Events.hpp"

#include "events/Movement.hpp"
#include "events/Shoot.hpp"

namespace rtype::system {
class InputsToGameEvents : public gengine::System<InputsToGameEvents, gengine::system::driver::output::KeyboardCatcher> {
public:
    void init(void) override;
    void sendEvents(geg::event::GameLoop &e);

    void moveUp(geg::event::io::KeyUpEvent &e);
    void moveLeft(geg::event::io::KeyLeftEvent &e);
    void moveDown(geg::event::io::KeyDownEvent &e);
    void moveRight(geg::event::io::KeyRightEvent &e);

    void shoot(geg::event::io::KeySpaceEvent &e);

private:
    event::Shoot::State m_shootState;

    char m_directionBitmask = 0;
    static const char UP_MASK = 1 << 0;    // 0b0001
    static const char RIGHT_MASK = 1 << 1; // 0b1000
    static const char DOWN_MASK = 1 << 2;  // 0b0010
    static const char LEFT_MASK = 1 << 3;  // 0b0100
    void updateDirectionBitmask(event::Movement::State state, bool);
    event::Movement::State getMovementState(void);
};
} // namespace rtype::system
