/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** SharedEvent.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/interface/components/RemoteLocal.hpp"

namespace gengine::interface::event {

template <class T>
class SharedEvent : public Event {
public:
    explicit SharedEvent(T &event, interface::component::RemoteLocal &remote)
        : remote(remote)
        , m_event(event) {
    }

    T *operator->() {
        return &m_event;
    }

    const T *operator->() const {
        return &m_event;
    }

    T &operator*() {
        return m_event;
    }

    const T &operator*() const {
        return m_event;
    }

    interface::component::RemoteLocal remote;

private:
    T m_event;
};
} // namespace gengine::interface::network::event