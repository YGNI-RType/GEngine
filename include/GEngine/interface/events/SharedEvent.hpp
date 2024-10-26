/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** SharedEvent.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/System.hpp"

namespace gengine::interface::event {

template <class T>
class SharedEvent : public Event {
public:
    explicit SharedEvent(T &event, uuids::uuid &remoteUUID)
        : remoteUUID(remoteUUID)
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

    uuids::uuid remoteUUID;

private:
    T m_event;
};
} // namespace gengine::interface::event