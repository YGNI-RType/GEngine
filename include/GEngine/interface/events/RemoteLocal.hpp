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
    uuids::uuid uuid;
    NewRemoteLocal(uuids::uuid &uuid)
        : uuid(uuid) {
    }
    NewRemoteLocal(const uuids::uuid &&uuid)
        : uuid(uuid) {
    }
};

struct DeleteRemoteLocal : gengine::Event {
    uuids::uuid uuid;
    DeleteRemoteLocal(uuids::uuid &uuid)
        : uuid(uuid) {
    }
    DeleteRemoteLocal(const uuids::uuid &uuid)
        : uuid(uuid) {
    }
};

struct GetRemoteLocalWhoIAm : gengine::Event {
    uuids::uuid localUUID;
    GetRemoteLocalWhoIAm(uuids::uuid &localUUID)
        : localUUID(localUUID) {
    }
    GetRemoteLocalWhoIAm(const uuids::uuid &localUUID)
        : localUUID(localUUID) {
    }
};

struct ItsMe : gengine::Event {
    uuids::uuid myUUID;
    ItsMe(uuids::uuid &myUUID)
        : myUUID(myUUID) {
    }
    ItsMe(const uuids::uuid &myUUID)
        : myUUID(myUUID) {
    }
};
} // namespace gengine::interface::event
