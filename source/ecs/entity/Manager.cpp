/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** entity::Manager.cpp
*/

#include "ecs/entity/Manager.hpp"

using namespace ecs;

entity::Manager::Manager(uint64_t firstEntity)
    : m_size(firstEntity) {
}

entity::Entity entity::Manager::createEntity(void) {
    return ++m_size;
}
