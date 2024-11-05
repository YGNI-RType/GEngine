/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** InputBox.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace gengine::component::gui {
struct InputBox : public Component<InputBox> {
    std::string &strRef;
    uint8_t max;
    InputBox(std::string &str, uint8_t max = 21)
        : strRef(str)
        , max(max) {
    }

    bool operator==(const InputBox &) const {
        return true;
    }
    InputBox operator=(const InputBox &) const {
        return InputBox(strRef, max);
    }
};
} // namespace gengine::component::gui