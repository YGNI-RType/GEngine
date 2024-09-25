/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Manager.inl
*/

#pragma once

namespace ecs::system {
template <class T, class... Params> void Manager::registerSystem(Params &&...p) {
    static_assert(std::is_base_of<IsSystem, T>::value, "T must inherit from system::Base");
    m_systemTable[std::type_index(typeid(T))] = std::make_any<T>(std::forward<Params>(p)...);
    T &system = (std::any_cast<T &>(m_systemTable[std::type_index(typeid(T))]));
    system.m_eventBus = m_eventBus;
    system.m_ecs = m_ecs;
    system.init();
}

template <class T> T &Manager::getSystem(void) {
    auto it = m_systemTable.find(std::type_index(typeid(T)));

    if (it == m_systemTable.end())
        throw "";
    return std::any_cast<T &>(it->second);
}

template <class T> void Manager::publishEvent(T &event) { m_eventBus.publish<T>(event); }

template <class T> void Manager::publishEvent(T &&event) { m_eventBus.publish<T>(event); }
} // namespace ecs::system