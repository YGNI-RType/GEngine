/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BaseEngine.cpp
*/

#include "GEngine/BaseEngine.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "exceptions/Base.hpp"
#include <chrono>
#include <thread>

namespace gengine {
void BaseEngine::compute(void) {
    try {
        while (m_ecs.hasEvent())
            m_ecs.executeEvent();
        m_ecs.publishEvent(system::event::StopEngine());
        m_ecs.executeEvent();
    } catch (const gengine::Exception &e) {
        std::cerr << e.what() << std::endl;
    };
}

void BaseEngine::start(void) {
    m_ecs.publishEvent(system::event::StartEngine(params));
}

const BaseEngine::world_t &BaseEngine::getWorld(void) {
    return m_ecs.getComponentMap();
} // TODO keep ?

void BaseEngine::setFirstEntity(ecs::entity::Entity start) {
    m_ecs.setFirstEntity(start);
}

void BaseEngine::setParams(const char **argv, int size) {
    if (!argv)
        return;
    for (int i = 0; i < size; i++)
        params.emplace_back(argv[i]);
}
} // namespace gengine
