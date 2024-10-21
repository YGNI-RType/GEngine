/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** entity::Manager.hpp
*/

#pragma once

#include <cstdint>
#include <queue>

#include "ecs/entity/Entity.hpp"

namespace ecs::entity {
/**
 * @brief Manages the creation of entities in the ECS system.
 *
 * The Manager class handles the lifecycle of entities, allowing for the creation of new entities.
 */
class Manager {
public:
    /**
     * @brief Constructs an Entity Manager.
     *
     * Initializes the manager for handling entity creation.
     *
     * @param Entity The entity at which the counter will start. Useful if the same ECS is used in two diferent
     * instances to avoid entity conflicts (ex: network).
     */
    Manager(Entity firstEntity = 0);

    /**
     * @brief Creates a new entity.
     *
     * Increment a counter, unstable if the counter overflow (which would take millions of years).
     *
     * @return The newly created entity.
     */
    Entity createEntity(void);

private:
    /// The current size of the entity pool, used to generate new entity IDs.
    Entity m_size;
};
} // namespace ecs::entity
