/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Widgets.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/components/gui/Button.hpp"
#include "GEngine/libdev/components/gui/InputBox.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

namespace gengine::system::gui {
class ButtonHandler : public System<ButtonHandler, gengine::component::gui::Button, geg::component::io::Sprite,
                                    geg::component::HitBoxSquare2D, geg::component::Transform2D,
                                    gengine::component::gui::ButtonSpriteTint, gengine::component::gui::onClick>,
                      public LocalSystem {
public:
    void init(void) final;

    void changeTint(geg::event::GameLoop &e);

    void onMouseMove(geg::event::io::MouseMoveEvent &e);

    void onMouseLeft(geg::event::io::MouseLeft &e);
};

class ToggleButtonHandler
    : public System<ToggleButtonHandler, gengine::component::gui::ToggleButton, geg::component::io::Sprite,
                    geg::component::HitBoxSquare2D, geg::component::Transform2D,
                    gengine::component::gui::ButtonSpriteTint, gengine::component::gui::onClick>,
      public LocalSystem {
public:
    void init(void) final;

    void changeTint(geg::event::GameLoop &e);

    void onMouseMove(geg::event::io::MouseMoveEvent &e);

    void onMouseLeft(geg::event::io::MouseLeft &e);
};

class SelectButtonHandler
    : public System<SelectButtonHandler, gengine::component::gui::SelectButton, geg::component::io::Sprite,
                    geg::component::HitBoxSquare2D, geg::component::Transform2D,
                    gengine::component::gui::ButtonSpriteTint, gengine::component::gui::onClick>,
      public LocalSystem {
public:
    void init(void) final;

    void changeTint(geg::event::GameLoop &e);

    void onMouseMove(geg::event::io::MouseMoveEvent &e);

    void onMouseLeft(geg::event::io::MouseLeft &e);
};

class InputBoxHandler
    : public System<InputBoxHandler, geg::component::io::Text, gengine::component::gui::InputBox, gengine::component::gui::SelectButton, geg::component::Box<1>>,
      public LocalSystem {
public:
    void init(void) final;

    void onKeyPressed(geg::event::io::KeyPressedEvent &e);
};
} // namespace gengine::system::gui