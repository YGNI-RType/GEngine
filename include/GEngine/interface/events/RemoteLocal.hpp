/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RemoteLocal.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/System.hpp"

namespace gengine::interface::event {
struct NewRemoteLocal : gengine::Event {
    component::RemoteLocal remote;
    NewRemoteLocal(component::RemoteLocal &remote)
        : remote(remote) {
    }
    NewRemoteLocal(const component::RemoteLocal &&remote)
        : remote(remote) {
    }
};

struct DeleteRemoteLocal : gengine::Event {
    component::RemoteLocal remote;
    DeleteRemoteLocal(component::RemoteLocal &remote)
        : remote(remote) {
    }
    DeleteRemoteLocal(const component::RemoteLocal &remote)
        : remote(remote) {
    }
};
} // namespace gengine::interface::event
