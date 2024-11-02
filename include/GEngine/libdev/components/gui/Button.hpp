/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Button.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/components/driver/output/RaylibTypes.hpp"

namespace gengine::component::gui {
struct Button : public Component<Button> {
    enum ButtonState { RELEASED, HOVERED, PRESSED };
    ButtonState state;
    Button(ButtonState baseState = RELEASED)
        : state(baseState) {
    }

    bool operator==(const Button &) const = default;
};

struct ToggleButton : public Button, public Component<ToggleButton> {
    ToggleButton(ButtonState baseState = RELEASED)
        : Button(baseState) {
    }

    bool operator==(const ToggleButton &) const = default;
};

struct SelectButton : public Button, public Component<SelectButton> {
    SelectButton(ButtonState baseState = RELEASED)
        : Button(baseState) {
    }

    bool operator==(const SelectButton &) const = default;
};

struct onClick : public Component<onClick> { // TODO better way ??
    std::function<void()> callback;
    onClick(const std::function<void()> callback)
        : callback(callback) {
    }

    bool operator==(const onClick &) const {
        return true;
    }
};

struct ButtonSpriteTint : public Component<ButtonSpriteTint> {
    std::array<driver::output::Clr, 3> tints;

    ButtonSpriteTint(const driver::output::Clr &hover = YELLOW, const driver::output::Clr &pressed = GRAY,
                     const driver::output::Clr &released = WHITE)
        : tints({released, hover, pressed}) {
    }

    bool operator==(const ButtonSpriteTint &) const = default;
};
} // namespace gengine::component::gui
