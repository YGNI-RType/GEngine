/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ExComponent.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct ExComponent : public gengine::Component<ExComponent> {
    ExComponent() {
    }
};
} // namespace rtype::component
