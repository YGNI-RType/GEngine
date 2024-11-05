/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Widgets.cpp
*/

#include "GEngine/libdev/systems/gui/Widgets.hpp"

namespace gengine::system::gui {
void ButtonHandler::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&ButtonHandler::changeTint);
    subscribeToEvent<geg::event::io::MouseLeft>(&ButtonHandler::onMouseLeft);
    subscribeToEvent<geg::event::io::MouseMoveEvent>(&ButtonHandler::onMouseMove);
}

void ButtonHandler::changeTint(geg::event::GameLoop &e) {
    auto &buttons = getComponents<gengine::component::gui::Button>();
    auto &tints = getComponents<gengine::component::gui::ButtonSpriteTint>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto [entity, button, tint, sprite] : Zip(buttons, tints, sprites))
        sprite.tint = tint.tints[button.state];
}

void ButtonHandler::onMouseMove(geg::event::io::MouseMoveEvent &e) {
    auto &buttons = getComponents<gengine::component::gui::Button>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, button, sprite, tr] : Zip(buttons, sprites, transforms)) {

        if (CheckCollisionPointRec({e.cursorPos.x, e.cursorPos.y}, {tr.pos.x, tr.pos.y, sprite.src.width * tr.scale.x,
                                                                    sprite.src.height * tr.scale.y})) {
            if (button.state == component::gui::Button::RELEASED)
                button.state = component::gui::Button::HOVERED;
        } else {
            button.state = component::gui::Button::RELEASED;
        }
    }
}

void ButtonHandler::onMouseLeft(geg::event::io::MouseLeft &e) {
    auto &buttons = getComponents<gengine::component::gui::Button>();
    auto &fs = getComponents<gengine::component::gui::onClick>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, button, sprite, tr] : Zip(buttons, sprites, transforms)) {
        if (CheckCollisionPointRec({e.cursorPos.x, e.cursorPos.y}, {tr.pos.x, tr.pos.y, sprite.src.width * tr.scale.x,
                                                                    sprite.src.height * tr.scale.y})) {
            if (e.state == geg::event::io::InputState::PRESSED) {
                button.state = component::gui::Button::PRESSED;
                if (fs.contains(entity))
                    fs.get(entity).callback();
            } else if (e.state == geg::event::io::InputState::RELEASE)
                button.state = component::gui::Button::HOVERED;
        }
    }
}

void ToggleButtonHandler::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&ToggleButtonHandler::changeTint);
    subscribeToEvent<geg::event::io::MouseLeft>(&ToggleButtonHandler::onMouseLeft);
    subscribeToEvent<geg::event::io::MouseMoveEvent>(&ToggleButtonHandler::onMouseMove);
}

void ToggleButtonHandler::changeTint(geg::event::GameLoop &e) {
    auto &buttons = getComponents<gengine::component::gui::ToggleButton>();
    auto &tints = getComponents<gengine::component::gui::ButtonSpriteTint>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto [entity, button, tint, sprite] : Zip(buttons, tints, sprites))
        sprite.tint = tint.tints[button.state];
}

void ToggleButtonHandler::onMouseMove(geg::event::io::MouseMoveEvent &e) {
    auto &buttons = getComponents<gengine::component::gui::ToggleButton>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, button, sprite, tr] : Zip(buttons, sprites, transforms)) {
        if (CheckCollisionPointRec({e.cursorPos.x, e.cursorPos.y}, {tr.pos.x, tr.pos.y, sprite.src.width * tr.scale.x,
                                                                    sprite.src.height * tr.scale.y})) {
            if (button.state == component::gui::Button::RELEASED)
                button.state = component::gui::Button::HOVERED;
        } else if (button.state != component::gui::Button::PRESSED) {
            button.state = component::gui::Button::RELEASED;
        }
    }
}

void ToggleButtonHandler::onMouseLeft(geg::event::io::MouseLeft &e) {
    auto &buttons = getComponents<gengine::component::gui::ToggleButton>();
    auto &fs = getComponents<gengine::component::gui::onClick>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, button, sprite, tr] : Zip(buttons, sprites, transforms)) {
        if (CheckCollisionPointRec({e.cursorPos.x, e.cursorPos.y}, {tr.pos.x, tr.pos.y, sprite.src.width * tr.scale.x,
                                                                    sprite.src.height * tr.scale.y})) {
            if (e.state == geg::event::io::InputState::PRESSED) {
                if (button.state == component::gui::Button::PRESSED)
                    button.state = component::gui::Button::HOVERED;
                else {
                    button.state = component::gui::Button::PRESSED;
                    if (fs.contains(entity))
                        fs.get(entity).callback();
                }
            } else if (e.state == geg::event::io::InputState::RELEASE) {
                if (button.state != component::gui::Button::PRESSED)
                    button.state = component::gui::Button::HOVERED;
            }
        }
    }
}

void SelectButtonHandler::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&SelectButtonHandler::changeTint);
    subscribeToEvent<geg::event::io::MouseLeft>(&SelectButtonHandler::onMouseLeft);
    subscribeToEvent<geg::event::io::MouseMoveEvent>(&SelectButtonHandler::onMouseMove);
}

void SelectButtonHandler::changeTint(geg::event::GameLoop &e) {
    auto &buttons = getComponents<gengine::component::gui::SelectButton>();
    auto &tints = getComponents<gengine::component::gui::ButtonSpriteTint>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto [entity, button, tint, sprite] : Zip(buttons, tints, sprites))
        sprite.tint = tint.tints[button.state];
}

void SelectButtonHandler::onMouseMove(geg::event::io::MouseMoveEvent &e) {
    auto &buttons = getComponents<gengine::component::gui::SelectButton>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, button, sprite, tr] : Zip(buttons, sprites, transforms)) {
        if (CheckCollisionPointRec({e.cursorPos.x, e.cursorPos.y}, {tr.pos.x, tr.pos.y, sprite.src.width * tr.scale.x,
                                                                    sprite.src.height * tr.scale.y})) {
            if (button.state == component::gui::Button::RELEASED)
                button.state = component::gui::Button::HOVERED;
        } else if (button.state != component::gui::Button::PRESSED) {
            button.state = component::gui::Button::RELEASED;
        }
    }
}

void SelectButtonHandler::onMouseLeft(geg::event::io::MouseLeft &e) {
    auto &buttons = getComponents<gengine::component::gui::SelectButton>();
    auto &fs = getComponents<gengine::component::gui::onClick>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, button, sprite, tr] : Zip(buttons, sprites, transforms)) {
        if (CheckCollisionPointRec({e.cursorPos.x, e.cursorPos.y}, {tr.pos.x, tr.pos.y, sprite.src.width * tr.scale.x,
                                                                    sprite.src.height * tr.scale.y})) {
            if (e.state == geg::event::io::InputState::PRESSED) {
                if (button.state == component::gui::Button::PRESSED)
                    button.state = component::gui::Button::HOVERED;
                else {
                    button.state = component::gui::Button::PRESSED;
                    if (fs.contains(entity))
                        fs.get(entity).callback();
                }
            }
        } else if (e.state == geg::event::io::InputState::PRESSED) {
            button.state = component::gui::Button::RELEASED;
        }
    }
}

void InputBoxHandler::init(void) {
    subscribeToEvent<geg::event::io::KeyPressedEvent>(&InputBoxHandler::onKeyPressed);
}

void InputBoxHandler::onKeyPressed(geg::event::io::KeyPressedEvent &e) {
    auto &inputs = getComponents<gengine::component::gui::InputBox>();
    auto &buttons = getComponents<gengine::component::gui::SelectButton>();
    auto &boxes = getComponents<geg::component::Box<1>>();
    auto &texts = getComponents<geg::component::io::Text>();

    for (auto [entity, input, button, box] : Zip(inputs, buttons, boxes)) {
        if (button.state == button.PRESSED) {
            int index = input.strRef.size();
            if (e.key >= ' ' && e.key <= '}') {
                if (index < input.max - 1)
                    input.strRef.push_back(e.key);
            } else if (e.key == KEY_BACKSPACE) {
                if (!input.strRef.empty())
                    input.strRef.pop_back();
            } else if (e.key == KEY_ENTER) {
                button.state = button.RELEASED;
            }
            if (texts.contains(box.entities[0]))
                texts.get(box.entities[0]).str = input.strRef;
        }
    }
}
} // namespace gengine::system::gui