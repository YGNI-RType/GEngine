/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Box.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/Entity.hpp"

namespace gengine::component {
template <size_t Size>
struct Box : public Component<Box<Size>> {
    std::array<Entity, Size> entities;

    Box(const std::array<Entity, Size> &entities)
        : entities(entities) {
    }
};
} // namespace gengine::component